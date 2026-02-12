//**************************************************
// 
// 編集
// Author Tesuji Yamamoto
// 
//**************************************************
#include "edit.h"
#include "manager.h"
#include "texture.h"
#include "myMath.h"
#include "camera.h"
#include "debugproc.h"
#include "range2D.h"
#include "functionBase.h"
#include "functionEdit.h"
#include "component.h"
#include "compWeapon.h"
#include "compCollisionModel.h"

using namespace std;
using namespace nlohmann;

//**************************************************
// 編集コンストラク
//**************************************************
CEdit::CEdit()
{	
	m_model.pMesh = nullptr;		// メッシュ
	m_model.pBuffMat = nullptr;	// マテリアル
	m_model.dwNumMat = 0;			// マテリアルの数
	m_model.pos= InitValu::VEC3_NULL ;		// 位置
	m_model.rot= InitValu::VEC3_NULL ;		// 向き
	m_model.mtxWorld = InitValu::MTX_NULL;	// ワールドマトリックス
	m_nNumVtx = 0;						// 頂点数
	m_nEditType = 0; 					// 編集タイプ
	m_nNumBone = 0;						// ボーンの総数
	m_fSetWaght = 0.0f;					// 設定する重さ
	m_nSelectBone = 0;					// 今選択しているボーン
	m_pTouchBone = NULL;				// 触れているボーン
	m_pTouchVtxGroup = NULL;			// 触れいている頂点グループ
	m_bDisp = true;						// 映すか否か
	m_pRange2D = nullptr;				// 範囲選択用ポリゴン
	m_posRangeA = InitValu::VEC3_NULL;	// 範囲位置A
	m_posRangeB = InitValu::VEC3_NULL;	// 範囲位置B
	m_fMovementBone = m_fMovementBone;		// ボーンの移動量
	m_pFunctionEdtBase = nullptr;		// 編集機能
	m_pPlayer = nullptr;				// エンティティ
	m_bZTest = false;					// Zテストの有無
	m_nSelectWeapon = -1;				// 今選択している武器
	m_pWeaponOperate = nullptr;			// 操る武器
	m_nSelectCollModel = -1;			// 今選択している当たり判定用モデル
	m_pCollModelOperate = nullptr;		// 操る当たり判定用モデル
}

//**************************************************
// 編集デストラクタ
//**************************************************
CEdit::~CEdit()
{
}

//**************************************************
// 編集初期化
//**************************************************
HRESULT CEdit::Init(const char* pEditModelFilePass)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// エンティティ生成
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CEntity;
	}

	// Xファイル読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		pEditModelFilePass,
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.dwNumMat,
		&m_model.pMesh
	);

	if (FAILED(hr))
	{// 失敗したら
		MessageBox(NULL, "CEdit::Init() > モデルを読み込めませんでした", "Error", MB_OK);

		// 結果を返す
		return E_FAIL;
	}

	// ファイル名保存
	m_sXFilePass = pEditModelFilePass;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_model.mtxWorld);

	// 頂点情報を取得----------------------------------------------------------------------------------------
	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

	// テクスチャ破棄
	for (auto iter = m_model.vsTexturePath.begin(); iter != m_model.vsTexturePath.end(); iter++)
	{
		pTexture->Unload(iter->c_str());
	}
	m_model.vsTexturePath.clear();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
	{
		string texturePath = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
		m_model.vsTexturePath.push_back(texturePath);
	}

	// 頂点バッファのポインタ
	BYTE* pVtxBuff = NULL;
	
	// 頂点フォーマットのサイズデータ
	WORD sizeFVF;

	// 頂点フォーマットのサイズを取得
	sizeFVF = (WORD)D3DXGetFVFVertexSize(m_model.pMesh->GetFVF());

	// メッシュの頂点数の取得
	m_nNumVtx = m_model.pMesh->GetNumVertices();

	// 頂点バッファのロック
	m_model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// モデルの頂点数分回す
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3* posVtx = (D3DXVECTOR3*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR3);
		D3DXVECTOR3* norVtx = (D3DXVECTOR3*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR3);
		D3DXVECTOR2* texVtx = (D3DXVECTOR2*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR2);
		
		// 頂点情報格納用
		VERTEXEDIT vtxInfo;
		ZeroMemory(&vtxInfo,sizeof(VERTEXEDIT));

		// 情報を移し替える
		vtxInfo.posOffSet = *posVtx;
		vtxInfo.norOffSet = *norVtx;

		// ボーンのインデックス分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			vtxInfo.aIdxBone[nCntIdxBone] = -1;
		}

		// 編集用頂点情報追加
		m_vVtxEdit.push_back(vtxInfo);
	}

	// 頂点バッファのアンロック
	m_model.pMesh->UnlockVertexBuffer();

	// 頂点グループを作成----------------------------------------------------------------------------------------

	auto iterVtxEditA = m_vVtxEdit.begin();
	
	// セットしたか否かの配列用意
	std::vector<bool> vBool;
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		vBool.push_back(false);
	}

	for (int nCntVtxA = 0; nCntVtxA < m_nNumVtx; nCntVtxA++, iterVtxEditA++)
	{
		if (vBool[nCntVtxA] == true)
		{
			continue;
		}

		// 頂点グループ変数用意
		VERTEXGROUP vtxGroup;

		// ボーンのインデックスの数分回す
		for (int nCntIdx = 0; nCntIdx < IDX_BONE_NUM; nCntIdx++)
		{
			// ボーンのインデックスと重さの初期値を設定
			vtxGroup.aIdxBone[nCntIdx] = -1;
			vtxGroup.aVtxWaght[nCntIdx] = 0.0f;
		}

		// 比較される頂点座標を取得
		D3DXVECTOR3 vtxPosA = (*iterVtxEditA).posOffSet;

		auto iterVtxEditB = m_vVtxEdit.begin();

		// Aの頂点番号を追加
		vtxGroup.vVtxIdx.push_back(nCntVtxA);

		// 頂点の数分回す
		for (int nCntVtxB = 0; nCntVtxB < m_nNumVtx; nCntVtxB++, iterVtxEditB++)
		{
			if (vBool[nCntVtxB] == true)
			{
				continue;
			}

			// 比較する頂点座標を取得
			D3DXVECTOR3 vtxPosB = (*iterVtxEditB).posOffSet;

			// 下三桁まであっていれば同じ頂点として扱う
			int nXA = (int)(vtxPosA.x * 1000.0f);
			int nYA = (int)(vtxPosA.y * 1000.0f);
			int nZA = (int)(vtxPosA.z * 1000.0f);

			int nXB = (int)(vtxPosB.x * 1000.0f);
			int nYB = (int)(vtxPosB.y * 1000.0f);
			int nZB = (int)(vtxPosB.z * 1000.0f);

			// どれか一つの成分があっていなければ飛ばす
			if (nXA != nXB || nYA != nYB || nZA != nZB)
			{
				continue;
			}

			// 頂点番号を追加
			vtxGroup.vVtxIdx.push_back(nCntVtxB);

			vBool[nCntVtxB] = true;
		}

		// 位置を設定
		vtxGroup.pos = vtxPosA;

		// IDを設定
		vtxGroup.nID = (int)m_vVtxGroup.size();

		// 頂点グループの要素追加
		m_vVtxGroup.push_back(vtxGroup);

		vBool[nCntVtxA] = true;
	}

	// 頂点のマーカーを頂点グループの数分作成
	for (int nCntMark = 0; nCntMark < (int)m_vVtxGroup.size(); nCntMark++)
	{
		// 生成
		CObjectBillboard* pMark = CObjectBillboard::Create(InitValu::VEC3_NULL, InitValu::VEC3_NULL,InitValu::RGBA_FULL, MARK_WIDTH, MARK_WIDTH);

		// 要素追加
		m_vpVtxMark.push_back(pMark);
	}

	AddFunction(new CFunctionLight);				// ライト機能追加
	AddFunction(new CFunctionBoneDispSize);			// ボーンのスケーリング機能追加
	AddFunction(new CFunctionBoneMovementScale);	// ボーンの移動量スケール機能追加

	// 編集機能設定
	ChangeFunctionEdit(new CFunctionEditNone);

	// 結果を返す
	return S_OK;
}

//**************************************************
// 編集終了
//**************************************************
void CEdit::Uninit(void)
{
	// エンティティ破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	m_model.vsTexturePath.clear();

	// マテリアルバッファの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	// 頂点マークの破棄
	for (auto& iter: m_vpVtxMark)
	{
		iter->Uninit();
	}
	m_vpVtxMark.clear();

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// ボーン破棄
	m_vBone.clear();

	// 頂点編集用破棄
	m_vVtxEdit.clear();	

	// 頂点グループ破棄
	for (auto& iter : m_vVtxGroup)
	{
		iter.vVtxIdx.clear();
	}
	m_vVtxGroup.clear();

	// 頂点グループ破棄
	for (auto& iter : m_vectorCollModel)
	{
		iter->Uninit();
	}
	m_vectorCollModel.clear();

	// 範囲用ポリゴン破棄
	if (m_pRange2D != nullptr)
	{
		m_pRange2D->Uninit();
		m_pRange2D = NULL;
	}

	// 機能クラス破棄
	for (auto& iter : m_vpFunctionBase)
	{
		iter->Uninit();
	}
	m_vpFunctionBase.clear();

	// 編集機能破棄
	if (m_pFunctionEdtBase != nullptr)
	{
		m_pFunctionEdtBase->Uninit();
		m_pFunctionEdtBase = nullptr;
	}

	// 自身の破棄
	Release();
}

//**************************************************
// 編集更新
//**************************************************
void CEdit::Update(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();
	
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// LSHIFT押したまま
	if (pInputKeyboard->GetPress(DIK_LSHIFT) == false)
	{
		// 編集タイプ切替
		if (pInputKeyboard->GetTrigger(DIK_F1) == true)
		{
			m_pFunctionEdtBase->F1();

			// 頂点自体の情報をリセット
			ResetVtx();

			// ボーンの位置と向きをリセット
			ResetBone();
		}
		// データを保存
		else if (pInputKeyboard->GetTrigger(DIK_F2) == true)
		{
			int nID = MessageBox(NULL, "現在の情報を書き込みますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				if (SkinInfoWrite() == true && SkinInfoWriteBinary() == true)
				{
					MessageBox(NULL, "書き込み完了！", "Error", MB_OK);
				}
			}
		}
		// データを読込
		else if (pInputKeyboard->GetTrigger(DIK_F3) == true)
		{
			int nID = MessageBox(NULL, "保存されている情報を読み込みますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				SkinInfoRead();
				//SkinInfoReadBinary();
			}
		}
		// モデルを読込
		else if (pInputKeyboard->GetTrigger(DIK_F4) == true)
		{
			int nID = MessageBox(NULL, "モデルファイルを読み込みますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				ReadModel();
			}
		}
		// モデル表示切替
		else if (pInputKeyboard->GetTrigger(DIK_F5) == true)
		{
			SwitchisDisp();
		}
		// ボーンの表示切替
		else if (pInputKeyboard->GetTrigger(DIK_F6) == true)
		{
			CBone::SwitchIsDisp();
		}
		// 頂点の表示切替
		else if (pInputKeyboard->GetTrigger(DIK_F7) == true)
		{
			CObjectBillboard::SwitchIsDisp();
		}
		// モデルのZテスト切替
		else if (pInputKeyboard->GetTrigger(DIK_F8) == true)
		{
			SwitchIsZTest();
		}
		// 当たり判定の表示非表示切替
		else if (pInputKeyboard->GetTrigger(DIK_F9) == true)
		{
			CCollisionModel::SwitchIsDisp();
		}
	}
	// LSHIFT押しっぱなしで
	else
	{
		// 武器の表示非表示切替
		if (pInputKeyboard->GetTrigger(DIK_F1) == true)
		{
			CWeapon::SwitchIsDisp();
		}
	}

	int nSelectBone = 0;

	// 頂点に触れる処理
	touchVertex();
	
	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// ボーンの数分回す
	for (auto& iterBone : m_vBone)
	{
		// ワールドマトリックス初期化
		D3DXMatrixIdentity(&iterBone.mtxWorld);

		// 向きを追加
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.mtxRot);

		// 位置を追加
		D3DXMatrixTranslation(&mtxTrans, iterBone.pos.x, iterBone.pos.y, iterBone.pos.z);
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxTrans);

		// 親がいれば親のマトリックスを掛ける
		if (iterBone.pBoneParent != NULL)
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.pBoneParent->mtxWorld);
		}
		// いなければこのオブジェクトのマトリックスを掛ける
		else
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &m_model.mtxWorld, &iterBone.mtxWorld);
		}

	}

	// 編集機能更新
	if (m_pFunctionEdtBase != nullptr)
	{
		m_pFunctionEdtBase->Update();
	}

	// エンティティ更新
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Update();
	}

	// マウスホイール
	if (pInputMouse->GetWheel() > 0)
	{
		m_pFunctionEdtBase->WheelPlas();
	}
	else if (pInputMouse->GetWheel() < 0)
	{
		m_pFunctionEdtBase->WheelMines();
	}

	// マウス左クリック
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		m_pFunctionEdtBase->CrickL();
	}
	// マウス右クリック
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		m_pFunctionEdtBase->CrickR();
	}
	// マウス中クリック
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		m_pFunctionEdtBase->CrickCenter();
	}

	// Wキー処理
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		m_pFunctionEdtBase->W();
	}
	// Aキー処理
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		m_pFunctionEdtBase->A();
	}
	// Sキー処理
	if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		m_pFunctionEdtBase->S();
	}
	// Dキー処理
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		m_pFunctionEdtBase->D();
	}
	// Qキー処理
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{
		m_pFunctionEdtBase->Q();
	}
	// Eキー処理
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{
		m_pFunctionEdtBase->E();
	}
	// Tキー処理
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		m_pFunctionEdtBase->T();
	}
	// Gキー処理
	if (pInputKeyboard->GetPress(DIK_G) == true)
	{
		m_pFunctionEdtBase->G();
	}
	// Fキー処理
	if (pInputKeyboard->GetPress(DIK_F) == true)
	{
		m_pFunctionEdtBase->F();
	}
	// Hキー処理
	if (pInputKeyboard->GetPress(DIK_H) == true)
	{
		m_pFunctionEdtBase->H();
	}
	// Rキー処理
	if (pInputKeyboard->GetPress(DIK_R) == true)
	{
		m_pFunctionEdtBase->R();
	}
	// Yキー処理
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{
		m_pFunctionEdtBase->Y();
	}
	// Xキー処理
	if (pInputKeyboard->GetPress(DIK_X) == true)
	{
		m_pFunctionEdtBase->X();
	}
	// Bキー処理
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{
		m_pFunctionEdtBase->B();
	}
	// UPキー処理
	if (pInputKeyboard->GetPress(DIK_UP) == true)
	{
		m_pFunctionEdtBase->Up();
	}
	// DOWNキー処理
	if (pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		m_pFunctionEdtBase->Down();
	}
	// LIGHTキー処理
	if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		m_pFunctionEdtBase->Right();
	}
	// LEFTキー処理
	if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		m_pFunctionEdtBase->Left();
	}

	// 機能クラス更新
	for (auto& iter : m_vpFunctionBase)
	{
		iter->Update();
	}

	// デバッグ表示
	m_pFunctionEdtBase->Debug();
}

//**************************************************
// 編集描画
//**************************************************
void CEdit::Draw(void)
{
	// エンティティ描画
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Draw();
	}

	// 映さなければ処理しない
	if (m_bDisp == false)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtx;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&mtx);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_model.rot.y, m_model.rot.x, m_model.rot.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_model.pos.x, m_model.pos.y, m_model.pos.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	if (m_bZTest == true)
	{
		// Zの比較方法
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		// Zバッファに書き込まない
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_model.vsTexturePath[nCntMat].c_str()));

		// モデル(パーツの描画)
		m_model.pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//**************************************************
// 編集生成
//**************************************************
CEdit* CEdit::Create(const char* pEditModelFilePass)
{
	CEdit* pEdit = new CEdit;

	if (pEdit != NULL)
	{
		// 初期化処理
		pEdit->Init(pEditModelFilePass);
	}
	else
	{
		MessageBox(NULL, " CEdit::Create() メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pEdit;
}

//**************************************************
// 編集機能なしの場合
//**************************************************
void CEdit::editNone(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// 右クリック中注視点操作
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		pCamera->OperateMousePosV();
	}
	// 中ボタンスライド操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		pCamera->OperateMouseSlide();
	}

	// マウスで距離を変える処理
	pCamera->OperateMouseDistance();
}

//**************************************************
// 編集リギング処理
//**************************************************
void CEdit::editBoneRigging(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// 右クリック中注視点操作
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		pCamera->OperateMousePosV();
	}
	// 中ボタンスライド操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		pCamera->OperateMouseSlide();
	}

	// マウスで距離を変える処理
	pCamera->OperateMouseDistance();

	// ボーン生成
	if (pInputKeyboard->GetTrigger(DIK_B) == true)
	{
		CreateBone();
	}
	// ボーン削除
	else if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		DeleteBone();
	}

	// ボーンが一つも設定されていなければ処理を止める
	if (m_vBone.size() == 0)
	{
		return;
	}

	// 操作するボーン取得
	auto iterOperateBone = m_vBone.begin() + m_nSelectBone;
	m_pTouchBone = &*iterOperateBone;

	// ボーン操作上移動
	if (pInputKeyboard->GetTriggerPress(DIK_Q) == true)
	{
		(*iterOperateBone).pos.y += m_fMovementBone;
	}
	// ボーン操作下移動
	else if (pInputKeyboard->GetTriggerPress(DIK_E) == true)
	{
		(*iterOperateBone).pos.y -= m_fMovementBone;
	}

	// ボーン操作奥移動
	if (pInputKeyboard->GetTriggerPress(DIK_W) == true)
	{
		(*iterOperateBone).pos.z += m_fMovementBone;
	}
	// ボーン操作手前移動
	else if (pInputKeyboard->GetTriggerPress(DIK_S) == true)
	{
		(*iterOperateBone).pos.z -= m_fMovementBone;
	}

	// ボーン操作左移動
	if (pInputKeyboard->GetTriggerPress(DIK_A) == true)
	{
		(*iterOperateBone).pos.x -= m_fMovementBone;
	}
	// ボーン操作右移動
	else if (pInputKeyboard->GetTriggerPress(DIK_D) == true)
	{
		(*iterOperateBone).pos.x += m_fMovementBone;
	}

	// ボーン親変更
	if (pInputKeyboard->GetTriggerPress(DIK_RIGHT) == true)
	{
		SelectBonePerrent(1);
	}
	// ボーン親変更
	else if (pInputKeyboard->GetTriggerPress(DIK_LEFT) == true)
	{
		SelectBonePerrent(-1);
	}

	// ボーンのオフセット位置リセット位置更新
	for (auto& iterBone : m_vBone)
	{
		iterBone.posOffset.x = iterBone.mtxWorld._41;
		iterBone.posOffset.y = iterBone.mtxWorld._42;
		iterBone.posOffset.z = iterBone.mtxWorld._43;

		iterBone.posReset = iterBone.pos;
	}
}

//**************************************************
// 編集スキニング処理
//**************************************************
void CEdit::editBoneSkinning(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// 右クリック中注視点操作
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		pCamera->OperateMousePosV();
	}
	// 中ボタンスライド操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		pCamera->OperateMouseSlide();
	}

	// 左クリック範囲選択
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		CreateRange();
	}
	// 左クリック解除時範囲選択終了
	else if (pInputMouse->GetRelease(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		DeleteRange();
	}

	// マウスで距離を変える処理
	pCamera->OperateMouseDistance();

	// ボーンが無ければ処理しない
	if (m_vBone.size() <= 0)
	{
		return;
	}

	// 操作するボーン取得
	auto iterOperateBone = m_vBone.begin() + m_nSelectBone;
	m_pTouchBone = &*iterOperateBone;

	// LSHIFTを押していれば
	if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
	{
		// ゼロ番目のインデックスを変更
		if (pInputKeyboard->GetTrigger(DIK_0) == true)
		{
			editVtx_BoneIdxReset(0);
		}
		// 一番目のインデックスを変更
		else if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{
			editVtx_BoneIdxReset(1);
		}
	}
	// LSHIFTを押していなければ
	else
	{
		// ゼロ番目のインデックスを変更
		if (pInputKeyboard->GetTrigger(DIK_0) == true)
		{
			editVtx_BoneIdx(0);
		}
		// 一番目のインデックスを変更
		else if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{
			editVtx_BoneIdx(1);
		}
	}

	// 重さを変える処理
	editVtx_Waght();

	// 重さで頂点を変える
	editVertexWaghtColor();
}

//**************************************************
// 編集ボーンのモデルの配置
//**************************************************
void CEdit::BoneModelSettingPosition(void)
{
	auto iterBone = m_vBone.begin();

	// ボーンのモデル分位置設定する
	for (auto iterBoneModel : m_vpBoneModel)
	{
		// 位置を取得
		D3DXVECTOR3 pos;
		pos.x = (*iterBone).mtxWorld._41;
		pos.y = (*iterBone).mtxWorld._42;
		pos.z = (*iterBone).mtxWorld._43;

		// 向き行列
		D3DXMATRIX mtxRot = (*iterBone).mtxWorld;
		mtxRot._41 = 0.0f;
		mtxRot._42 = 0.0f;
		mtxRot._43 = 0.0f;

		// 回転行列を設定
		iterBoneModel->SetMatrixRotate(mtxRot);

		// 位置設定
		iterBoneModel->SetPosition(pos);

		// 頂点位置を初期値に戻す
		iterBoneModel->ResetVertex();

		// 親のポインタを取得
		CBone::BONE* pBoneChild = (*iterBone).pBoneChild;

		// 親ボーンが存在する
		if (pBoneChild != nullptr)
		{
			// 親の位置取得
			D3DXVECTOR3 posChild;
			posChild.x = pBoneChild->mtxWorld._41;
			posChild.y = pBoneChild->mtxWorld._42;
			posChild.z = pBoneChild->mtxWorld._43;

			// 自身から親までのベクトルを取る
			D3DXVECTOR3 vecPos = posChild - pos;

			float fLength = D3DXVec3Length(&vecPos);

			// 長さを設定
			iterBoneModel->SetHeight(fLength);

			D3DXQUATERNION quat;
			D3DXVECTOR3 vecAxis;
			D3DXVECTOR3 vecY = { 0.0f,1.0f,0.0f };

			// 外積で回転軸を求める
			D3DXVec3Normalize(&vecPos, &vecPos);
			D3DXVec3Cross(&vecAxis, &vecY, &vecPos);

			if ((int)(D3DXVec3Length(&vecAxis) * 100.0f) == 0)
			{
				vecAxis = { 1.0f,0.0f,0.0f };
			}

			// ベクトルのなす角を求める
			float fAngle = CMyMath::VectorAngleToRadian(vecY, vecPos);

			// クオータニオンを求める
			D3DXQuaternionRotationAxis(&quat, &vecAxis, fAngle);

			// クオータニオンから回転行列を求める
			D3DXMatrixRotationQuaternion(&mtxRot, &quat);

			// 回転行列を設定
			iterBoneModel->SetMatrixRotate(mtxRot);
		}

		// 親のポインタを取得
		CBone::BONE* pBoneParent = (*iterBone).pBoneParent;

		// 親ボーンが存在する
		if (pBoneParent != nullptr)
		{
			// 親の位置取得
			D3DXVECTOR3 posParent;
			posParent.x = pBoneParent->mtxWorld._41;
			posParent.y = pBoneParent->mtxWorld._42;
			posParent.z = pBoneParent->mtxWorld._43;

			// 自身から親までのベクトルを取る
			D3DXVECTOR3 vecPos = posParent - pos;

			// ボーンの逆行列作成
			D3DXMATRIX mtxInvers;
			D3DXMATRIX mtxRotBone = iterBoneModel->GetMatrixRotate();
			D3DXMatrixInverse(&mtxInvers, nullptr, &mtxRotBone);

			// 逆行列を掛けた位置にする
			vecPos = CMyMath::MatrixMulRotation(vecPos, mtxInvers);

			// お尻を設定
			iterBoneModel->SetBack(vecPos);
		}

		// ボーンを一つ進める
		iterBone++;
	}
}

//**************************************************
// 編集すべてのボーンのモデルの色を変える
//**************************************************
void CEdit::BoneModelOllSetColor(D3DXCOLOR colBone, D3DXCOLOR colBoneParent, D3DXCOLOR colBoneSelect)
{
	// ボーンが無ければ処理しない
	if (m_vpBoneModel.size() == 0 || m_pTouchBone == NULL)
	{
		return;
	}

	// ボーンのモデルに色をつける
	for (auto iterBoneModel : m_vpBoneModel)
	{
		// 通常色に設定
		iterBoneModel->SetColor(colBone);
	}

	// 選んでいるボーンとその親のそれぞれのモデルを取得
	auto iterTouchBoneModel = m_vpBoneModel.begin() + m_pTouchBone->nIdx;

	// 今選んでいるボーンの色を設定する
	(*iterTouchBoneModel)->SetColor(colBoneSelect);

	// 親がいなければ処理はここまで
	if(m_pTouchBone->pBoneParent == NULL)
	{
		return;
	}

	// 親のボーンの色を変える
	auto iterTouchBoneModelParent = m_vpBoneModel.begin() + m_pTouchBone->pBoneParent->nIdx;
	(*iterTouchBoneModelParent)->SetColor(colBoneParent);
}

//**************************************************
// 編集ボーンの生成
//**************************************************
void CEdit::CreateBone(void)
{
	CBone::BONE bone;
	ZeroMemory(&bone, sizeof(CBone::BONE));

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&bone.mtxWorld);
	D3DXMatrixIdentity(&bone.mtxRot);

	// インデックスを設定
	bone.nIdx = m_nNumBone;
	bone.nIdxParent = -1;
	bone.nIdxChild = -1;

	// ボーンをリストに追加
	m_vBone.push_back(bone);

	// ボーンの総数カウントアップ
	m_nNumBone++;

	// ボーンモデルの生成
	CBone* pBoneModel = CBone::Create();

	// リストに追加
	m_vpBoneModel.push_back(pBoneModel);

	// 親と子の再設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// 選択しているボーンの設定
	if (m_nSelectBone > (int)m_vBone.size())
	{
		auto iterBoneSelect = m_vBone.begin() + m_nSelectBone;
		m_pTouchBone = &*iterBoneSelect;
	}
}

//**************************************************
// 編集ボーンの削除
//**************************************************
void CEdit::DeleteBone(void)
{
	// ボーンが一つも無ければ処理しない
	if (m_vBone.size() == 0)
	{
		return;
	}

	// ボーンの総数カウントダウン
	m_nNumBone--;

	// 選択しているボーンの情報を取得
	auto iterSelectBone = m_vBone.begin() + m_nSelectBone;
	int nDeleteBoneIdx = (*iterSelectBone).nIdx;

	// 親と子の情報を設定
	if ((*iterSelectBone).pBoneParent != nullptr)
	{
		(*iterSelectBone).pBoneParent->nIdxChild = -1;
		(*iterSelectBone).pBoneParent->pBoneChild = nullptr;
	}

	if ((*iterSelectBone).pBoneChild != nullptr)
	{
		(*iterSelectBone).pBoneChild->nIdxParent = -1;
		(*iterSelectBone).pBoneChild->pBoneParent = nullptr;
	}

	// ボーンの要素を削除
	m_vBone.erase(iterSelectBone);

	// 消したボーンが親か子であれば消す
	for (auto& iterBone : m_vBone)
	{
		if (iterBone.nIdxParent == nDeleteBoneIdx)
		{
			iterBone.pos.x = iterBone.pBoneParent->mtxWorld._41;
			iterBone.pos.y = iterBone.pBoneParent->mtxWorld._42;
			iterBone.pos.z = iterBone.pBoneParent->mtxWorld._43;
			iterBone.pBoneParent = nullptr;
			iterBone.nIdxParent = -1;
		}

		if (iterBone.nIdxChild == nDeleteBoneIdx)
		{
			iterBone.pBoneChild = nullptr;
			iterBone.nIdxChild = -1;
		}
	}

	// ボーンのカウント用変数
	int nCntBone = 0;

	// ボーンのインデックス番号を変更する
	for (auto iterBone = m_vBone.begin(); iterBone != m_vBone.end(); iterBone++)
	{
		(*iterBone).nIdx = nCntBone;
		nCntBone++;
	}

	// 親や子のインデックス整理
	for (auto& iterBone : m_vBone)
	{
		// 親がいる場合
		if (iterBone.pBoneParent != nullptr)
		{
			if (iterBone.nIdxParent >= nDeleteBoneIdx)
			{
				auto iterBoneParent = m_vBone.begin() + (iterBone.nIdxParent - 1);
				iterBone.pBoneParent = &*iterBoneParent;
				iterBone.nIdxParent = iterBone.nIdxParent - 1;
			}
		}

		// 子がいる場合
		if (iterBone.pBoneChild != nullptr)
		{
			if (iterBone.nIdxChild >= nDeleteBoneIdx)
			{
				auto iterBoneChild = m_vBone.begin() + (iterBone.nIdxChild - 1);
				iterBone.pBoneChild = &*iterBoneChild;
				iterBone.nIdxChild = iterBone.nIdxChild - 1;
			}
		}
	}

	// ボーンのモデルを一つ減らす
	auto iterBoneModel = m_vpBoneModel.end() - 1;
	(*iterBoneModel)->Uninit();
	m_vpBoneModel.erase(iterBoneModel);

	// 選択中のボーンを一つ下げる
	m_nSelectBone--;

	// 下限まで行ったらループする
	m_nSelectBone = Clamp(m_nSelectBone,0,m_nNumBone);
}

//**************************************************
// 編集選んでいるボーンの親を変更する処理
//**************************************************
void CEdit::SelectBonePerrent(int nAddSelect)
{
	// 変化が無ければ処理しない
	if (nAddSelect == 0)
	{
		return;
	}

	int nAddSelectNomal = (int)sqrtf((float)(nAddSelect * nAddSelect));
	nAddSelectNomal = nAddSelect / nAddSelectNomal;

	// 親ボーンの子ボーン情報削除
	if (m_pTouchBone->pBoneParent != nullptr)
	{
		m_pTouchBone->pBoneParent->nIdxChild = -1;
		m_pTouchBone->pBoneParent->pBoneChild = nullptr;
	}

	// 親ボーンインデックスカウント
	m_pTouchBone->nIdxParent += nAddSelect;

	// 自信を親にしないようにする
	if (m_pTouchBone->nIdxParent == m_pTouchBone->nIdx)
	{
		// インデックスを進める
		m_pTouchBone->nIdxParent += nAddSelectNomal;
	}

	// 自身の子を親にしないようにする
	if (m_pTouchBone->pBoneChild != NULL)
	{
		// 子とインデックスが同じ
		if (m_pTouchBone->pBoneChild->nIdx == m_pTouchBone->nIdxParent)
		{
			// インデックスを進める
			m_pTouchBone->nIdxParent += nAddSelectNomal;
		}
	}

	// 最大、最小まで行ったらループする
	m_pTouchBone->nIdxParent = Wrap(m_pTouchBone->nIdxParent, -1, m_nNumBone - 1);

	// ボーンの数分親子のポインタを設定する
	for (auto& iterBone : m_vBone)
	{
		// 親ボーンが存在していれば
		if (iterBone.nIdxParent != -1)
		{
			// 親ボーンのイテレーターを取得
			auto iterOperateBonePerrent = m_vBone.begin() + iterBone.nIdxParent;

			// 親ボーンのポインタ設定
			iterBone.pBoneParent = &*iterOperateBonePerrent;

			// 親ボーンのインデックスを設定
			iterBone.nIdxParent = (*iterOperateBonePerrent).nIdx;

			// 親ボーンの子ボーンのポインタ設定
			(*iterOperateBonePerrent).nIdxChild = iterBone.nIdx;

			// 親ボーンの子ボーンのインデックス設定
			(*iterOperateBonePerrent).pBoneChild = &iterBone;
		}
		// 存在しない場合
		else
		{
			// 親が設定されている場合
			if (iterBone.pBoneParent != NULL)
			{
				// 親ボーンの子ボーンのポインタ設定
				iterBone.pBoneParent->pBoneChild = NULL;

				// 親ボーンの子ボーンのインデックス設定
				iterBone.pBoneParent->nIdxChild = -1;

				// 親ボーンのインデックスを設定
				iterBone.nIdxParent = -1;

				// 親ボーンのポインタ設定
				iterBone.pBoneParent = NULL;
			}
		}
	}

	// 親がいれば
	if (m_pTouchBone->pBoneParent != nullptr)
	{
		// 高さ調整
		m_pTouchBone->pos.y = CBone::BONE_MODEL_HEIGHT;
	}
}

//**************************************************
// 編集選んでいる頂点の色設定
//**************************************************
void CEdit::editSelectVertexColor(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	
	// さわるものがあれば処理
	if (m_pTouchVtxGroup != NULL)
	{
		// 触れいている頂点マークの特定
		auto iterVtxMark = m_vpVtxMark.begin() + m_pTouchVtxGroup->nID;

		// 頂点マークを半透明にする
		D3DXCOLOR colNow = (*iterVtxMark)->GetColor();
		colNow.a = colNow.a * 0.5f;
		(*iterVtxMark)->SetColor(colNow);
	}

	// さわるものがあれば処理
	if (m_vpTouchVtxGroup.empty() == false)
	{
		// 触る頂点の数分操作する
		for (auto& iterTouchVtxGroup : m_vpTouchVtxGroup)
		{
			// 触れいている頂点マークの特定
			auto iterVtxMark = m_vpVtxMark.begin() + iterTouchVtxGroup->nID;

			// 頂点マークを半透明にする
			D3DXCOLOR colNow = (*iterVtxMark)->GetColor();
			colNow.a = colNow.a * 0.5f;
			(*iterVtxMark)->SetColor(colNow);
		}
	}
	// 触る物が無ければ
	else
	{

	}
}

//**************************************************
// 編集頂点の重さによる色の変更
//**************************************************
void CEdit::editVertexWaghtColor(void)
{
	// インデックス番号
	int nIdx;

	// 選んでいるボーンのイテレータ取得
	auto iterSelectBone = m_vBone.begin() + m_nSelectBone;

	// 選んでいるボーンのインデックスを設定
	nIdx = (*iterSelectBone).nIdx;

	// 頂点グループの数分回す
	for (auto& iterVtxGroup : m_vVtxGroup)
	{
		auto iterVtxMark = m_vpVtxMark.begin() + iterVtxGroup.nID;
		auto iterVtxIdx = iterVtxGroup.vVtxIdx.begin();
		auto iterVtxEdit = m_vVtxEdit.begin() + (*iterVtxIdx);

		// 最初は空色
		D3DXCOLOR col = InitValu::COLOR_DARKCYAN;

		// ボーンのインデックスの数分回す
		for (int nCntIdx = 0; nCntIdx < IDX_BONE_NUM; nCntIdx++)
		{
			// ボーンと同じインデックスだったら
			if ((*iterVtxEdit).aIdxBone[nCntIdx] == nIdx)
			{
				// 重さを取得
				float fWaght = (*iterVtxEdit).aVtxWaght[nCntIdx];
				
				// 頂点マークを重さに準じた色にする
				D3DXCOLOR colGreen = InitValu::COLOR_GREEN * (1.0f - fWaght);
				D3DXCOLOR colred = InitValu::COLOR_RED * fWaght;
				col = colGreen + colred;
				break;
			}
		}

		// 色を設定
		(*iterVtxMark)->SetColor(col);
	}

}

//**************************************************
// 編集頂点に触れる処理
//**************************************************
CEdit::VERTEXGROUP* CEdit::touchVertex()
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// 頂点グループのイテレーター取得
	auto iterVtxGroup = m_vVtxGroup.begin();

	// 触れた頂点グループ
	VERTEXGROUP* pTouchVtxGroup = NULL;

	// 触れた頂点の2D座標
	D3DXVECTOR3 tatchPos2D = { 0.0f,0.0f,1.0f };

	// カメラの位置取得
	D3DXVECTOR3 posCameraV = pCamera->GetPosV();

	// 触れている頂点グループのインデックス
	int nIdxTouchVtxGroup = -1;

	// マーカーの数分位置を設定する
	for (auto& iterVtxMark : m_vpVtxMark)
	{
		// 位置を取得し
		D3DXVECTOR3 pos3D = (*iterVtxGroup).pos;

		// 3D座標から2D座標に変換
		D3DXVECTOR3 pos2D;
		CMyMath::Posision3Dto2D(&pos2D, pos3D);

		// 比較用変数設定
		D3DXVECTOR2 posA = { pos2D.x,pos2D.y };
		D3DXVECTOR2 posB = { posMouse.x ,posMouse.y };

		// カメラとの距離を図って
		D3DXVECTOR3 vecA = pos3D - posCameraV;
		float fLength = D3DXVec3Length(&vecA);

		// 距離倍率を求める
		float fMugLength = fLength / CCamera::DRAW_DISTANCE;

		// 横幅を求める
		float fWidth = MARK_WIDTH * fMugLength;

		// 求めた2D座標を設定
		iterVtxMark->SetPosition(pos3D);

		// 幅を設定
		iterVtxMark->SetWidth(fWidth);
		
		// 色をマゼンタに設定
		iterVtxMark->SetColor(InitValu::COLOR_MAGENTA);

		// マウスと当たっていれば
		if (CMyMath::IsColiisionCircleAndCircle2D(posA, 10.0f, posB, 0.0f) == true)
		{
			// z値が小さければ設定する
			if (pos2D.z <= tatchPos2D.z)
			{
				// 触れた頂点グループリストに追加
				pTouchVtxGroup = &*iterVtxGroup;
			}
		}

		// 頂点グループの情報を一つずらす
		iterVtxGroup++;
	}

	// 触れている頂点があれば
	if (pTouchVtxGroup != NULL)
	{
		// LSHIFTを押したまま
		if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			// 左クリックでリストに追加
			if (pInputMouse->GetTrigger(CInputMouse::MOUSEBUTTON_LEFT) == true)
			{
				m_vpTouchVtxGroup.push_back(pTouchVtxGroup);
			}
			// 右クリックでリストから削除
			else if (pInputMouse->GetTrigger(CInputMouse::MOUSEBUTTON_LEFT) == true)
			{
				// 探し出して消す
				for (auto iterVtxGro = m_vpTouchVtxGroup.begin(); iterVtxGro != m_vpTouchVtxGroup.end(); iterVtxGro++)
				{
					// IDが一緒なら消す
					if ((*iterVtxGro)->nID == pTouchVtxGroup->nID)
					{
						m_vpTouchVtxGroup.erase(iterVtxGro);
					}
				}
			}
		}
		// 押していない場合
		else
		{
			// 左クリックでリストを空にしてから要素追加
			if (pInputMouse->GetTrigger(CInputMouse::MOUSEBUTTON_LEFT) == true)
			{
				m_vpTouchVtxGroup.clear();
				m_vpTouchVtxGroup.push_back(pTouchVtxGroup);
			}
		}
	}
	// 触れている頂点が無ければ
	else
	{
		// 左クリックでリストを空にする
		if (pInputMouse->GetTrigger(CInputMouse::MOUSEBUTTON_LEFT) == true)
		{
			m_vpTouchVtxGroup.clear();
		}
	}

	m_pTouchVtxGroup = pTouchVtxGroup;
	return pTouchVtxGroup;
}

//**************************************************
// 編集ボーンを選択する処理
//**************************************************
void CEdit::SelectBone(int nAdd)
{
	// 一つ進める
	m_nSelectBone += nAdd;
	
	// 最大、最小まで行ったらループする
	m_nSelectBone = Wrap(m_nSelectBone, 0, m_nNumBone - 1);
}

//**************************************************
// 編集ボーンに触れる処理
//**************************************************
CBone::BONE* CEdit::touchBone(D3DXVECTOR3 posScreen)
{
	// 触れた頂点グループ
	CBone::BONE* pTouchBone = NULL;

	// 触れた頂点の2D座標
	D3DXVECTOR3 tatchPos2D = { 0.0f,0.0f,1.0f };

	auto iterBoneModel = m_vpBoneModel.begin();

	// マーカーの数分位置を設定する
	for (auto& iterBone : m_vBone)
	{
		// 位置を取得し
		D3DXVECTOR3 pos3D = iterBone.pos;

		// 3D座標から2D座標に変換
		D3DXVECTOR3 pos2D;
		CMyMath::Posision3Dto2D(&pos2D, pos3D);

		// 比較用変数設定
		D3DXVECTOR2 posA = { pos2D.x,pos2D.y };
		D3DXVECTOR2 posB = { posScreen.x ,posScreen.y };

		// マウスと当たっていれば
		if (CMyMath::IsColiisionCircleAndCircle2D(posA, MARK_WIDTH, posB, 0.0f) == true)
		{
			// z値が小さければ設定する
			if (pos2D.z <= tatchPos2D.z)
			{
				// 触れた頂点グループリストに追加
				pTouchBone = &iterBone;

				// ID取得
				m_nSelectBone = (*iterBoneModel)->GetID();
			}
		}

		iterBoneModel++;
	}

	return pTouchBone;
}

//**************************************************
// 編集頂点のボーンのインデック変える処理
//**************************************************
void CEdit::editVtx_BoneIdx(int nIdxBone)
{
	// インデックスが範囲外なら処理しない
	if (nIdxBone >= IDX_BONE_NUM ||
		nIdxBone < 0)
	{
		return;
	}

	// 触る頂点の数分操作する
	for (auto& iterTouchVtxGroup : m_vpTouchVtxGroup)
	{
		// グループの中のすべての頂点を処理する
		for (auto& iterVtxIdx : iterTouchVtxGroup->vVtxIdx)
		{
			// 選択しているボーンのインデックスを設定する
			auto iterVtxEdit = m_vVtxEdit.begin() + iterVtxIdx;
			auto iterSelectBone = m_vBone.begin() + m_nSelectBone;

			(*iterVtxEdit).aIdxBone[nIdxBone] = (*iterSelectBone).nIdx;
			iterTouchVtxGroup->aIdxBone[nIdxBone] = (*iterSelectBone).nIdx;
		}
	}
}

//**************************************************
// 編集頂点のボーンのインデックをリセットする処理
//**************************************************
void CEdit::editVtx_BoneIdxReset(int nIdxBone)
{
	// インデックスが範囲外なら処理しない
	if (nIdxBone >= IDX_BONE_NUM ||
		nIdxBone < 0)
	{
		return;
	}

	// 触る頂点の数分操作する
	for (auto& iterTouchVtxGroup : m_vpTouchVtxGroup)
	{
		// グループの中のすべての頂点を処理する
		for (auto& iterVtxIdx : iterTouchVtxGroup->vVtxIdx)
		{
			// 選択しているボーンのインデックスを設定する
			auto iterVtxEdit = m_vVtxEdit.begin() + iterVtxIdx;
			auto iterSelectBone = m_vBone.begin() + m_nSelectBone;

			(*iterVtxEdit).aIdxBone[nIdxBone] = -1;
			(*iterVtxEdit).aVtxWaght[nIdxBone] = 0.0f;
			iterTouchVtxGroup->aIdxBone[nIdxBone] = -1;
			iterTouchVtxGroup->aVtxWaght[nIdxBone] = 0.0f;
		}
	}
}

//**************************************************
// 編集重さを変える処理
//**************************************************
void CEdit::editVtx_Waght(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// さわるものが無ければ
	if (m_vpTouchVtxGroup.empty() == true)
	{
		// ホイールで設定する重さ調整
		if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			if ((int)posMouse.z != 0)
			{
				int nNomalizeZ = (int)sqrtf(posMouse.z * posMouse.z);
				nNomalizeZ = (int)posMouse.z / nNomalizeZ;

				// 与える重さを計算
				float fAddWaght = WAGTH_INCDEC_AMOUNT * nNomalizeZ;

				// 設定する重さに追加
				m_fSetWaght += fAddWaght;

				// 最大、最小値を超えないようにする
				m_fSetWaght = Clamp(m_fSetWaght, WAGTH_MIN, WAGTH_MAX);
			}
		}

		// 処理を止める
		return;
	}

	// 重さの上げ下げの有無
	bool bWagthUp = false;
	bool bWagthDown = false;

	// Sキーで下げる
	if (pInputKeyboard->GetTriggerPress(DIK_S) == true)
	{
		bWagthDown = true;
	}
	// Wキーで上げる
	else if (pInputKeyboard->GetTriggerPress(DIK_W) == true)
	{
		bWagthUp = true;
	}

	// 触る頂点の数分操作する
	for (auto& iterTouchVtxGroup : m_vpTouchVtxGroup)
	{
		// インデックスと現在の頂点一つにある全体の重さ
		int nIdx = -1;
		float fAllWaght = 0.0f;

		// 全体の重さと現在選んでいるボーンが設定されているか確かめる
		for (int nCntIdx = 0; nCntIdx < IDX_BONE_NUM; nCntIdx++)
		{
			// ボーン情報取得
			auto iterSelectBone = m_vBone.begin() + m_nSelectBone;

			// 選んでいるボーンのインデックス取得
			int nSelectIdx = -1;
			nSelectIdx = (*iterSelectBone).nIdx;

			// 合計の重さに足す
			fAllWaght += iterTouchVtxGroup->aVtxWaght[nCntIdx];

			// 頂点にこのボーンが設定されていたら
			if (iterTouchVtxGroup->aIdxBone[nCntIdx] == nSelectIdx)
			{
				// 設定するインデクス番号を設定
				nIdx = nCntIdx;
			}
		}

		// インデックスがなければ処理しない
		if (nIdx == -1)
		{
			return;
		}

		// 今の重さを取得
		float fWaghtNow = iterTouchVtxGroup->aVtxWaght[nIdx];

		// 追加する値
		float fAddWaght = 0.0f;

		// Sキーで下げる
		if (bWagthDown == true)
		{
			// 与える重さを計算
			fAddWaght = -WAGTH_INCDEC_AMOUNT;
		}
		// Wキーで上げる
		else if (bWagthUp == true)
		{
			// 与える重さを計算
			fAddWaght = WAGTH_INCDEC_AMOUNT;
		}
		// 重さの合計値が1.0を超えないようにする
		if (fAllWaght + fAddWaght > 1.0f)
		{
			fAddWaght = 1.0f - fAllWaght;
		}

		// 重さを足す
		fWaghtNow += fAddWaght;

		// 編集用頂点の重さを設定
		for (auto& iterVtxIdx : iterTouchVtxGroup->vVtxIdx)
		{
			auto iterVtxEdit = m_vVtxEdit.begin() + iterVtxIdx;

			// 重さ設定
			(*iterVtxEdit).aVtxWaght[nIdx] = fWaghtNow;
		}

		// 上限と下限を設定
		fWaghtNow = Clamp(fWaghtNow, 0.0f, 1.0f);

		// 頂点グループにも設定
		iterTouchVtxGroup->aVtxWaght[nIdx] = fWaghtNow;
	}
}

//**************************************************
// 編集プレビュー
//**************************************************
void CEdit::editBonePreview(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// ボーンが無ければ処理しない
	if (m_vBone.size() == 0)
	{
		return;
	}

	// 操作するボーン取得
	auto iterOperateBone = m_vBone.begin() + m_nSelectBone;
	m_pTouchBone = &*iterOperateBone;

	D3DXMATRIX mtxAddRot;
	D3DXQUATERNION quat;
	D3DXVECTOR3 vecAxis = InitValu::VEC3_NULL;
	float fAngle = D3DX_PI * 0.05f;

	// ボーンのX軸回転(手前)
	if (pInputKeyboard->GetTriggerPress(DIK_W) == true)
	{
		// 回転軸設定
		vecAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	// ボーンのX軸回転(奥)
	else if (pInputKeyboard->GetTriggerPress(DIK_S) == true)
	{
		// 回転軸設定
		vecAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}

	// ボーンのY軸回転
	if (pInputKeyboard->GetTriggerPress(DIK_A) == true)
	{
		// 回転軸設定
		vecAxis = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}
	// ボーンのY軸回転
	else if (pInputKeyboard->GetTriggerPress(DIK_D) == true)
	{
		// 回転軸設定
		vecAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// ボーンのZ軸回転
	if (pInputKeyboard->GetTriggerPress(DIK_Q) == true)
	{
		// 回転軸設定
		vecAxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	// ボーンのZ軸回転
	else if (pInputKeyboard->GetTriggerPress(DIK_E) == true)
	{
		// 回転軸設定
		vecAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	// ボーンの奥移動
	if (pInputKeyboard->GetTriggerPress(DIK_T) == true)
	{
		(*iterOperateBone).pos.z += m_fMovementBone;
	}
	// ボーンの手前移動
	else if (pInputKeyboard->GetTriggerPress(DIK_G) == true)
	{
		(*iterOperateBone).pos.z -= m_fMovementBone;
	}

	// ボーンの左移動
	if (pInputKeyboard->GetTriggerPress(DIK_F) == true)
	{
		(*iterOperateBone).pos.x -= m_fMovementBone;
	}
	// ボーンの右移動
	else if (pInputKeyboard->GetTriggerPress(DIK_H) == true)
	{
		(*iterOperateBone).pos.x += m_fMovementBone;
	}

	// ボーンの下移動
	if (pInputKeyboard->GetTriggerPress(DIK_R) == true)
	{
		(*iterOperateBone).pos.y -= m_fMovementBone;
	}
	// ボーンの上移動
	else if (pInputKeyboard->GetTriggerPress(DIK_Y) == true)
	{
		(*iterOperateBone).pos.y += m_fMovementBone;
	}

	// クオータニオンを求める
	D3DXQuaternionRotationAxis(&quat, &vecAxis, fAngle);

	// クオータニオンから回転行列を求める
	D3DXMatrixRotationQuaternion(&mtxAddRot, &quat);

	// 回転行列を設定
	auto iterBone = m_vBone.begin() + m_nSelectBone;

	// 回転を掛ける
	D3DXMatrixMultiply(&(*iterBone).mtxRot, &(*iterBone).mtxRot, &mtxAddRot);

	// ボーンのマトリックスリセット
	if (pInputKeyboard->GetPress(DIK_RETURN) == true)
	{
		// マトリックス初期化
		D3DXMatrixIdentity(&(*iterBone).mtxRot);
	}

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ボーンの数分回す
	for (auto& iterBone : m_vBone)
	{
		// ワールドマトリックス初期化
		D3DXMatrixIdentity(&iterBone.mtxWorld);

		// 向きを追加
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.mtxRot);

		// 位置を追加
		D3DXMatrixTranslation(&mtxTrans, iterBone.pos.x, iterBone.pos.y, iterBone.pos.z);
		D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &mtxTrans);

		// 親がいれば親のマトリックスを掛ける
		if (iterBone.pBoneParent != NULL)
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &iterBone.mtxWorld, &iterBone.pBoneParent->mtxWorld);
		}
		// いなければこのオブジェクトのマトリックスを掛ける
		else
		{
			D3DXMatrixMultiply(&iterBone.mtxWorld, &m_model.mtxWorld, &iterBone.mtxWorld);
		}
	}

	// 頂点バッファのポインタ
	BYTE* pVtxBuff = NULL;

	// 頂点フォーマットのサイズデータ
	WORD sizeFVF;

	// 頂点フォーマットのサイズを取得
	sizeFVF = (WORD)D3DXGetFVFVertexSize(m_model.pMesh->GetFVF());

	// メッシュの頂点数の取得
	m_nNumVtx = m_model.pMesh->GetNumVertices();

	// 頂点バッファのロック
	m_model.pMesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVtxBuff);

	// 頂点情報のイテレーター
	auto iterVtxEdit = m_vVtxEdit.begin();

	// モデルの頂点数分回す
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, iterVtxEdit++)
	{
		// 計算用変数
		D3DXVECTOR3 posHope, offSet, offSetEX, posLocal;
		D3DXVECTOR3 aNorOffset[IDX_BONE_NUM];
		D3DXMATRIX mtx, mtxOffset;

		// 一回でも設定下したかどうか
		bool bSet = false;

		// 位置をゼロにする
		posLocal = InitValu::VEC3_NULL;

		// 位置のポインタ設定
		D3DXVECTOR3* pPos = (D3DXVECTOR3*)pVtxBuff;

		offSet.x = (*iterVtxEdit).posOffSet.x;
		offSet.y = (*iterVtxEdit).posOffSet.y;
		offSet.z = (*iterVtxEdit).posOffSet.z;

		// ボーンインデックスの数分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			aNorOffset[nCntIdxBone].x = (*iterVtxEdit).norOffSet.x;
			aNorOffset[nCntIdxBone].y = (*iterVtxEdit).norOffSet.y;
			aNorOffset[nCntIdxBone].z = (*iterVtxEdit).norOffSet.z;

			int nIdxBone = (*iterVtxEdit).aIdxBone[nCntIdxBone];
			float fWeight = (*iterVtxEdit).aVtxWaght[nCntIdxBone];

			// 親ボーンが存在しないのであれば処理しない
			if (nIdxBone == -1)
			{
				aNorOffset[nCntIdxBone] = InitValu::VEC3_NULL;
				continue;
			}

			// セットした状態にする
			bSet = true;

			// ボーン情報取得
			auto iterBone = m_vBone.begin() + nIdxBone;

			// ボーンのマトリックス
			mtx = iterBone->mtxWorld;

			// 原点に移動
			offSetEX.x = offSet.x - (*iterBone).posOffset.x;
			offSetEX.y = offSet.y - (*iterBone).posOffset.y;
			offSetEX.z = offSet.z - (*iterBone).posOffset.z;

			// ボーンの回転を掛ける
			posHope.x = (offSetEX.x * mtx._11) + (offSetEX.y * mtx._21) + (offSetEX.z * mtx._31) + mtx._41;
			posHope.y = (offSetEX.x * mtx._12) + (offSetEX.y * mtx._22) + (offSetEX.z * mtx._32) + mtx._42;
			posHope.z = (offSetEX.x * mtx._13) + (offSetEX.y * mtx._23) + (offSetEX.z * mtx._33) + mtx._43;

			aNorOffset[nCntIdxBone].x = (aNorOffset[nCntIdxBone].x * mtx._11) + (aNorOffset[nCntIdxBone].y * mtx._21) + (aNorOffset[nCntIdxBone].z * mtx._31);
			aNorOffset[nCntIdxBone].y = (aNorOffset[nCntIdxBone].x * mtx._12) + (aNorOffset[nCntIdxBone].y * mtx._22) + (aNorOffset[nCntIdxBone].z * mtx._32);
			aNorOffset[nCntIdxBone].z = (aNorOffset[nCntIdxBone].x * mtx._13) + (aNorOffset[nCntIdxBone].y * mtx._23) + (aNorOffset[nCntIdxBone].z * mtx._33);

			// 重さを掛ける
			posHope *= fWeight;

			// 頂点座標に足す
			posLocal += posHope;
		}
		
		if (bSet == true)
		{
			// 計算した値を代入
			*pPos = posLocal;
		}
		else
		{
			// オフセット位置を設定
			*pPos = offSet;
		}

		// 位置のサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);

		// 法線のポインタを取得
		D3DXVECTOR3* pNor = (D3DXVECTOR3*)pVtxBuff;

		// ゼロに設定して
		*pNor = InitValu::VEC3_NULL;

		// ボーンインデックスの数分回す
		for (int nCntIdxBone = 0; nCntIdxBone < IDX_BONE_NUM; nCntIdxBone++)
		{
			// 計算した値を足していく
			*pNor += aNorOffset[nCntIdxBone];
		}

		// 正規化する
		D3DXVec3Normalize(pNor, pNor);

		// 残りのサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);
		pVtxBuff += sizeof(D3DXVECTOR2);
	}

	// 頂点バッファをアンロック
	m_model.pMesh->UnlockVertexBuffer();
}

//**************************************************
// 編集範囲生成処理
//**************************************************
void CEdit::CreateRange(void)
{
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// マウス位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// まだポリゴンが生成されていなかったらポリゴンを生成
	if (m_pRange2D == nullptr)
	{
		m_pRange2D = CRange2D::Create(posMouse,InitValu::VEC3_NULL,D3DXCOLOR(0.6f,0.6f,1.0f,0.5f),0.0f,0.0f);

		// 範囲の開始地点を設定
		m_posRangeA = posMouse;
	}
	// 生成されていた場合大きさを変える
	else
	{
		// 範囲の終端地点を設定
		m_posRangeB = posMouse;
		D3DXVECTOR3 vecPosRange = m_posRangeB - m_posRangeA;

		// 幅と高さを求める
		float fWidth = vecPosRange.x;
		float fHeight = vecPosRange.y;
		
		// ポリゴンの位置設定
		D3DXVECTOR3 posPolygon;
		posPolygon.x = m_posRangeA.x + fWidth;
		posPolygon.y = m_posRangeA.y + fHeight;
		posPolygon.z = m_posRangeA.z;

		// 幅と高さと位置設定
		m_pRange2D->SetHeight(fHeight);
		m_pRange2D->SetWidth(fWidth);
	}
}

//**************************************************
// 編集範囲削除処理
//**************************************************
void CEdit::DeleteRange(void)
{
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// マウス位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// まだポリゴンが生成されていたらポリゴンを削除
	if (m_pRange2D != nullptr)
	{
		// 頂点グループの数分回す
		for (auto& iterVtxGroup : m_vVtxGroup)
		{
			// 幅と高さを取得
			float fWidth = m_pRange2D->GetWidth();
			float fHeight = m_pRange2D->GetHeight();

			// 当たり判定用の幅と高さを設定
			float fWidthColi = sqrtf(fWidth * fWidth) * 0.5f;
			float fHeightColi = sqrtf(fHeight * fHeight) * 0.5f;

			// 範囲の中心までのベクトルを求め
			D3DXVECTOR3 vecRangeHalf = m_posRangeB - m_posRangeA;
			vecRangeHalf *= 0.5f;

			// 範囲の中心を求める
			D3DXVECTOR3 posRangeCenter = m_posRangeA + vecRangeHalf;

			// 頂点の座標を2D座標に変換する
			D3DXVECTOR3 posVtx2D;
			CMyMath::Posision3Dto2D(&posVtx2D, iterVtxGroup.pos);

			// 四角の範囲内に入っていればその頂点を触れるリストに入れる
			if (CMyMath::IsColiisionLectAndPoint(D3DXVECTOR2(posRangeCenter.x, posRangeCenter.y), fWidthColi, fHeightColi, D3DXVECTOR2(posVtx2D.x, posVtx2D.y)) == true)
			{
				// 要素追加
				m_vpTouchVtxGroup.push_back(&iterVtxGroup);
			}
		}

		m_pRange2D->Uninit();
		m_pRange2D = NULL;
	}
}

//**************************************************
// 編集頂点初期位置設定処理
//**************************************************
void CEdit::ResetVtx(void)
{
	// 頂点バッファのポインタ
	BYTE* pVtxBuff = NULL;

	// 頂点フォーマットのサイズデータ
	WORD sizeFVF;

	// 頂点フォーマットのサイズを取得
	sizeFVF = (WORD)D3DXGetFVFVertexSize(m_model.pMesh->GetFVF());

	// メッシュの頂点数の取得
	m_nNumVtx = m_model.pMesh->GetNumVertices();

	// 頂点バッファのロック
	m_model.pMesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVtxBuff);

	// 頂点情報のイテレーター
	auto iterVtxEdit = m_vVtxEdit.begin();

	// モデルの頂点数分回す
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, iterVtxEdit++)
	{

		// 位置のポインタ取得
		D3DXVECTOR3* pPos = (D3DXVECTOR3*)pVtxBuff;

		// オフセット位置に設定
		*pPos = (*iterVtxEdit).posOffSet;

		// 位置のサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);

		// 法線のポインタを取得
		D3DXVECTOR3* pNor = (D3DXVECTOR3*)pVtxBuff;

		// 法線のオフセット位置に設定
		*pNor = (*iterVtxEdit).norOffSet;

		// 残りのサイズ分ずらす
		pVtxBuff += sizeof(D3DXVECTOR3);
		pVtxBuff += sizeof(D3DXVECTOR2);
	}

	// 頂点バッファをアンロック
	m_model.pMesh->UnlockVertexBuffer();
}

//**************************************************
// 編集書き出し処理
//**************************************************
void CEdit::ResetBone(void)
{
	// ボーンの数分回す
	for (auto& iterBone : m_vBone)
	{
		// 回転をリセット
		D3DXMatrixIdentity(&iterBone.mtxRot);

		// 位置リセット
		iterBone.pos = iterBone.posReset;
	}
}

//**************************************************
// 編集書き出し処理
//**************************************************
bool CEdit::SkinInfoWrite(void)
{
	// ファイルを開く
	ofstream ofs(CManager::GetFilePathWriteSkinMesh());

	// ファイルが開けなかったら処理しない
	if (ofs.is_open() == NULL)
	{
		MessageBox(NULL, "CEdit::Writing() > ファイルが開けませんでした", "Error", MB_OK);
		return false;
	}

	ofs << "#*******************************************************************************\n";
	ofs << "#\n";
	ofs << "# スキンメッシュ情報(書き込み)\n";
	ofs << "# Author Tetsuji Yamamoto\n";
	ofs << "#\n";
	ofs << "# < コンポーネント >\n";
	ofs << "# COLLISION_SET	# 当たり判定用モデル情報\n";
	ofs << "# COLLISION_NAME = collisions_body\n";
	ofs << "# COLLISION_XFILEPATH = data/test.x\n";
	ofs << "# COLLISION_BONEIDX = -1\n";
	ofs << "# COLLISION_OFFSET = 0 0 0\n";
	ofs << "# COLLISION_QUAT = 0 0 0 1\n";
	ofs << "# COLLISION_END\n";
	ofs << "# \n";
	ofs << "# WEAPON_SET	# 当たり判定用モデル情報\n";
	ofs << "# WEAPON_NAME = collisions_body\n";
	ofs << "# WEAPON_XFILEPATH = data/test.x\n";
	ofs << "# WEAPON_BONEIDX = -1\n";
	ofs << "# WEAPON_OFFSET = 0 0 0\n";
	ofs << "# WEAPON_QUAT = 0 0 0 1\n";
	ofs << "# WEAPON_END\n";
	ofs << "#*******************************************************************************\n";
	ofs << "XFILEPASS = " << m_sXFilePass.c_str() << "\t# Xファイルの相対パス\n";
	ofs << "NUMBONE = " << m_nNumBone <<"\t# ボーンの数\n";
	ofs << "NUMVTX = "<< m_nNumVtx << "\t# 頂点数\n";
	ofs << "\n";

	ofs << "#*******************************************************************************\n";
	ofs << "# コンポーネント\n";
	ofs << "#*******************************************************************************\n";

	// コンポーネントを書き込む
	for (auto& iterComp : m_pPlayer->GetComponentVector())
	{
		// 武器であれば
		if (auto* compWeapon = dynamic_cast<CWeapon*>(iterComp))
		{
			ofs << "WEAPON_SET\t" << "# 武器情報\n";
			ofs << "\tWEAPON_NAME = " << compWeapon->GetName() << "\n";
			ofs << "\tWEAPON_XFILEPATH = " << compWeapon->GetXFilePath() << "\n";
			ofs << "\tWEAPON_BONEIDX = " << compWeapon->GetIdxBone() << "\n";
			D3DXVECTOR3 pos = compWeapon->GetPosition();
			ofs << "\tWEAPON_OFFSET = ";
			ofs << pos.x << " ";
			ofs << pos.y << " ";
			ofs << pos.z << "\n";
			D3DXQUATERNION quat = compWeapon->GetQuat();
			ofs << "\tWEAPON_QUAT = ";
			ofs << quat.x << " ";
			ofs << quat.y << " ";
			ofs << quat.z << " ";
			ofs << quat.w << "\n";
			ofs << "WEAPON_END\n";
			ofs << "\n";
		}
		// 当たり判定用モデルであれば
		else if (auto* compCollModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			ofs << "COLLISION_SET\t" << "# 当たり判定用モデル情報\n";
			ofs << "\tCOLLISION_NAME = " << compCollModel->GetName() << "\n";
			ofs << "\tCOLLISION_XFILEPATH = " << compCollModel->GetXFilePath() << "\n";
			ofs << "\tCOLLISION_BONEIDX = " << compCollModel->GetIdxBone() << "\n";
			D3DXVECTOR3 pos = compCollModel->GetPosition();
			ofs << "\tCOLLISION_OFFSET = ";
			ofs << pos.x << " ";
			ofs << pos.y << " ";
			ofs << pos.z << "\n";
			D3DXQUATERNION quat = compCollModel->GetQuat();
			ofs << "\tCOLLISION_QUAT = ";
			ofs << quat.x << " ";
			ofs << quat.y << " ";
			ofs << quat.z << " ";
			ofs << quat.w << "\n";
			ofs << "COLLISION_END\n";
			ofs << "\n";
		}
	}

	ofs << "#*******************************************************************************\n";
	ofs << "# ボーン\n";
	ofs << "#*******************************************************************************\n";

	// ボーン情報
	for (auto& iterBone : m_vBone)
	{
		ofs << "BONE_SET\n";
		ofs << "\tBONE_IDX = " << iterBone.nIdx <<"\n";
		ofs << "\tBONE_CHILD_IDX = "<< iterBone.nIdxChild << " \n";
		ofs << "\tBONE_PARENT_IDX = " << iterBone.nIdxParent << " \n";
		ofs << "\tBONE_POS = ";
		ofs << iterBone.pos.x << " ";
		ofs << iterBone.pos.y << " ";
		ofs << iterBone.pos.z << "\n";
		ofs << "\tBONE_OFFSETPOS = ";
		ofs << iterBone.posOffset.x << " ";
		ofs << iterBone.posOffset.y << " ";
		ofs << iterBone.posOffset.z << "\n";
		ofs << "BONE_END\n";
		ofs << "\n";
	}

	ofs << "#*******************************************************************************\n";
	ofs << "# 頂点\n";
	ofs << "#*******************************************************************************\n";
	
	// 頂点情報
	for (auto& iterVtx : m_vVtxEdit)
	{
		ofs << "VTX_SET\n";
		ofs << "\tVTX_IDXBONE_0 = " << iterVtx.aIdxBone[0] << "\n";
		ofs << "\tVTX_IDXBONE_1 = " << iterVtx.aIdxBone[1] << "\n";
		ofs << "\tVTX_WAGHT_0 = " << iterVtx.aVtxWaght[0] << "\n";
		ofs << "\tVTX_WAGHT_1 = " << iterVtx.aVtxWaght[1] << "\n";
		ofs << "\tVTX_NOROFFSET = ";
		ofs << iterVtx.norOffSet.x << " ";
		ofs << iterVtx.norOffSet.y << " ";
		ofs << iterVtx.norOffSet.z << "\n";
		ofs << "\tVTX_POSOFFSET = ";
		ofs << iterVtx.posOffSet.x << " ";
		ofs << iterVtx.posOffSet.y << " ";
		ofs << iterVtx.posOffSet.z << "\n";
		ofs << "VTX_END\n";
		ofs << "\n";
	}

	// ファイルを閉じる
	ofs.close();

	return true;
}

//**************************************************
// 編集書き出し処理(バイナリ)
//**************************************************
bool CEdit::SkinInfoWriteBinary(void)
{
	// ファイルを開く
	ofstream ofs(CManager::GetFilePathWriteSkinMeshBinary(),ios::binary);

	// ファイルが開けなかったら処理しない
	if (ofs.is_open() == NULL)
	{
		MessageBox(NULL, "CEdit::Writing() > ファイルが開けませんでした", "Error", MB_OK);
		return false;
	}

	// コンポーネント配列取得
	std::vector<CComponent*>& vectorComp = m_pPlayer->GetComponentVector();

	// 保存用配列
	std::vector<CWeapon*> vectorWeapom;
	std::vector<CCollisionModel*> vectorCollModel;

	// コンポーネント情報
	for (auto& iterComp : vectorComp)
	{
		// 武器であれば
		if (auto* compWeapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 保存用の配列に並べる
			vectorWeapom.push_back(compWeapon);
		}
		// 当たり判定用モデルであれば
		else if (auto* compCollModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 保存用の配列に並べる
			vectorCollModel.push_back(compCollModel);
		}
	}

	// Xファイルの相対パスの文字数
	int nSize = m_sXFilePass.size();
	ofs.write((const char*)&nSize,sizeof(int));

	// 武器コンポーネント数書き込み
	int nNumCompWeapon = (int)vectorWeapom.size();
	ofs.write((const char*)&nNumCompWeapon, sizeof(int));

	// 当たり判定用モデルコンポーネント数書き込み
	int nNumCompCollModel = (int)vectorCollModel.size();
	ofs.write((const char*)&nNumCompCollModel, sizeof(int));

	// ボーンの数
	ofs.write((const char*)&m_nNumBone, sizeof(int));
	
	// 頂点数
	ofs.write((const char*)&m_nNumVtx, sizeof(int));

	// Xファイルの相対パス
	for (int nCnt = 0; nCnt < (int)m_sXFilePass.size(); nCnt++)
	{
		char cData = m_sXFilePass[nCnt];
		ofs.write((const char*)&cData, sizeof(char));
	}

	// 武器情報保存
	for (auto& iterWeapon : vectorWeapom)
	{
		// 武器の書き込む情報取得
		string& name = iterWeapon->GetName();
		string& XFilePath = iterWeapon->GetXFilePath();
		int nIdxBone = iterWeapon->GetIdxBone();
		D3DXVECTOR3 pos = iterWeapon->GetPosition();
		D3DXQUATERNION quat = iterWeapon->GetQuat();

		// 名前文字数書き込み
		int nNumName = name.size();
		ofs.write((const char*)&nNumName, sizeof(int));
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			// 名前の書き込み
			char cData = name[nCntName];
			ofs.write((const char*)&cData, sizeof(char));
		}

		// ファイルパス文字数書き込み
		int nNumXFilePath = XFilePath.size();
		ofs.write((const char*)&nNumXFilePath, sizeof(int));
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			// ファイルパスの書き込み
			char cData = XFilePath[nCntXFilePath];
			ofs.write((const char*)&cData, sizeof(char));
		}

		// ボーンインデックス書き込み
		ofs.write((const char*)&nIdxBone, sizeof(int));

		// 位置の書き込み
		ofs.write((const char*)&pos, sizeof(D3DXVECTOR3));

		// クォータニオンの書き込み
		ofs.write((const char*)&quat, sizeof(D3DXQUATERNION));
	}

	// 当たり判定用モデル情報書き込み
	for (auto& iterCollModel : vectorCollModel)
	{
		// 当たり判定用モデルの書き込む情報取得
		string& name = iterCollModel->GetName();
		string& XFilePath = iterCollModel->GetXFilePath();
		int nIdxBone = iterCollModel->GetIdxBone();
		D3DXVECTOR3 pos = iterCollModel->GetPosition();
		D3DXQUATERNION quat = iterCollModel->GetQuat();

		// 名前文字数書き込み
		int nNumName = name.size();
		ofs.write((const char*)&nNumName, sizeof(int));
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			// 名前の書き込み
			char cData = name[nCntName];
			ofs.write((const char*)&cData, sizeof(char));
		}

		// ファイルパス文字数書き込み
		int nNumXFilePath = XFilePath.size();
		ofs.write((const char*)&nNumXFilePath, sizeof(int));
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			// ファイルパスの書き込み
			char cData = XFilePath[nCntXFilePath];
			ofs.write((const char*)&cData, sizeof(char));
		}

		// ボーンインデックス書き込み
		ofs.write((const char*)&nIdxBone, sizeof(int));

		// 位置の書き込み
		ofs.write((const char*)&pos, sizeof(D3DXVECTOR3));

		// クォータニオンの書き込み
		ofs.write((const char*)&quat, sizeof(D3DXQUATERNION));
	}

	// ボーン情報
	for (auto& iterBone : m_vBone)
	{
		// ボーン情報
		CBone::BONE bone = iterBone;
		ofs.write((const char*)&bone, sizeof(CBone::BONE));
	}

	// 頂点情報
	for (auto& iterVtx : m_vVtxEdit)
	{
		VERTEXEDIT vtxEdit = iterVtx;
		ofs.write((const char*)&vtxEdit, sizeof(VERTEXEDIT));
	}

	// ファイルを閉じる
	ofs.close();

	return true;
}

//**************************************************
// 編集読込処理(バイナリ)
//**************************************************
bool CEdit::SkinInfoReadBinary(void)
{
	// 読み取り用としてファイルを開く
	ifstream ifs(CManager::GetFilePathReadSkinMeshBinary().c_str(), ios::binary);

	// ファイルが開けなかった
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CEdit::ReadBinary() > ファイルが開けませんでした", "Error", MB_OK);
		return false;
	}

	m_nSelectWeapon = -1;			// 今選択している武器
	m_pWeaponOperate = nullptr;		// 操る武器
	m_nSelectCollModel = -1;		// 今選択している当たり判定用モデル
	m_pCollModelOperate = nullptr;	// 操る当たり判定用モデル

	// エンティティ破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// コンポーネントリスト空
	m_vectorCollModel.clear();
	m_vectorWeapon.clear();

	// エンティティ生成
	m_pPlayer = new CEntity;

	// マテリアルバッファの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	// 頂点マークの破棄
	for (auto& iter : m_vpVtxMark)
	{
		iter->Uninit();
	}
	m_vpVtxMark.clear();

	// ボーン破棄
	m_vBone.clear();

	// 頂点編集用破棄
	m_vVtxEdit.clear();

	// 選択中の頂点グループ破棄
	m_vpTouchVtxGroup.clear();

	// 触れいているボーンをリセット
	m_pTouchBone = NULL;

	// 触れている頂点グループをリセット
	m_pTouchVtxGroup = NULL;

	// 頂点グループ破棄
	for (auto& iter : m_vVtxGroup)
	{
		iter.vVtxIdx.clear();
	}
	m_vVtxGroup.clear();

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// ボーン数リセット
	m_nNumBone = 0;

	// ファイルパスリセット
	m_sXFilePass.clear();

	// Xファイルの相対パスの文字数読込
	int nXfilePathSize;
	ifs.read((char*)&nXfilePathSize, sizeof(int));

	// 武器コンポーネント数読込
	int nNumCompWeapon;
	ifs.read((char*)&nNumCompWeapon, sizeof(int));

	//	当たり判定用モデルコンポーネント数読込
	int nNumCompCollModel;
	ifs.read((char*)&nNumCompCollModel, sizeof(int));

	// ボーンの数読込
	ifs.read((char*)&m_nNumBone, sizeof(int));

	// 頂点数読込
	ifs.read((char*)&m_nNumVtx, sizeof(int));

	// Xファイルの相対パス読込
	for (int nCntFilePath = 0; nCntFilePath < nXfilePathSize; nCntFilePath++)
	{
		char cData;
		ifs.read((char*)&cData, sizeof(char));
		m_sXFilePass += cData;
	}

	// 初期化処理
	Init(m_sXFilePass.c_str());

	// 武器コンポーネント読込
	for (int nCntCompWeapon = 0; nCntCompWeapon < nNumCompWeapon; nCntCompWeapon++)
	{
		CWeapon *weaponRead = new CWeapon;
		string name;
		string XFilePath;
		int nIdxBone;
		D3DXVECTOR3 pos;
		D3DXQUATERNION quat;

		// 名前文字数読込
		int nNumName = 0;
		ifs.read((char*)&nNumName,sizeof(int));
		
		// 文字読込
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			name += cData;
		}

		// Xファイルパス文字数読込
		int nNumXFilePath = 0;
		ifs.read((char*)&nNumXFilePath, sizeof(int));

		// 文字読込
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			XFilePath += cData;
		}

		// ボーンインデックス読み取り
		ifs.read((char*)&nIdxBone, sizeof(int));

		// 位置読み取り
		ifs.read((char*)&pos, sizeof(D3DXVECTOR3));
		
		// 向き読み取り
		ifs.read((char*)&quat, sizeof(D3DXQUATERNION));

		// 情報設定
		weaponRead->SetName(name);
		weaponRead->SetXFilePath(XFilePath);
		weaponRead->SetIdxBone(nIdxBone);
		weaponRead->SetPosition(pos);
		weaponRead->SetQuat(quat);

		// 初期化
		weaponRead->Init();

		// コンポーネントに要素追加
		m_pPlayer->AddComponentPushBack(weaponRead);

		// リストに追加
		m_vectorWeapon.push_back(weaponRead);
	}

	// 当たり判定用モデルコンポーネント読込
	for (int nCntCompCollModel = 0; nCntCompCollModel < nNumCompCollModel; nCntCompCollModel++)
	{
		CCollisionModel* collModelRead = new CCollisionModel;
		string name;
		string XFilePath;
		int nIdxBone;
		D3DXVECTOR3 pos;
		D3DXQUATERNION quat;

		// 名前文字数読込
		int nNumName = 0;
		ifs.read((char*)&nNumName, sizeof(int));

		// 文字読込
		for (int nCntName = 0; nCntName < nNumName; nCntName++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			name += cData;
		}

		// Xファイルパス文字数読込
		int nNumXFilePath = 0;
		ifs.read((char*)&nNumXFilePath, sizeof(int));

		// 文字読込
		for (int nCntXFilePath = 0; nCntXFilePath < nNumXFilePath; nCntXFilePath++)
		{
			char cData;
			ifs.read((char*)&cData, sizeof(char));
			XFilePath += cData;
		}

		// ボーンインデックス読み取り
		ifs.read((char*)&nIdxBone, sizeof(int));

		// 位置読み取り
		ifs.read((char*)&pos, sizeof(D3DXVECTOR3));

		// 向き読み取り
		ifs.read((char*)&quat, sizeof(D3DXQUATERNION));

		// 情報設定
		collModelRead->SetName(name);
		collModelRead->SetXFilePath(XFilePath);
		collModelRead->SetIdxBone(nIdxBone);
		collModelRead->SetPosition(pos);
		collModelRead->SetQuat(quat);

		// 初期化
		collModelRead->Init();

		// コンポーネントに要素追加
		m_pPlayer->AddComponentPushBack(collModelRead);

		// リストに追加
		m_vectorCollModel.push_back(collModelRead);
	}

	// ボーン情報読込
	for (int nCntBone = 0; nCntBone < m_nNumBone; nCntBone++)
	{
		CBone::BONE bone;
		ifs.read((char*)&bone, sizeof(CBone::BONE));
		m_vBone.push_back(bone);

		// ボーンモデルの生成
		CBone* pBoneModel = CBone::Create();

		// リストに追加
		m_vpBoneModel.push_back(pBoneModel);
	}

	// 頂点情報読込
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		VERTEXEDIT vtxEdit;
		ifs.read((char*)&vtxEdit, sizeof(VERTEXEDIT));
		auto iterVtx = m_vVtxEdit.begin() + nCntVtx;
		*iterVtx = vtxEdit;
	}

	// ファイルを閉じる
	ifs.close();

	// 親と子の再設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// 頂点グループの設定
	for (auto& iterVtxGroup : m_vVtxGroup)
	{
		// 頂点番号取得
		auto iterVtxIdx = iterVtxGroup.vVtxIdx.begin();

		// 頂点情報取得
		auto iterVtxEdit = m_vVtxEdit.begin() + (*iterVtxIdx);

		for (int nCnt = 0; nCnt < IDX_BONE_NUM; nCnt++)
		{
			iterVtxGroup.aVtxWaght[nCnt] = (*iterVtxEdit).aVtxWaght[nCnt];
			iterVtxGroup.aIdxBone[nCnt] = (*iterVtxEdit).aIdxBone[nCnt];
		}
	}

	// コンポーネントの各種設定
	for (auto& iterComp : m_pPlayer->GetComponentVector())
	{
		// 武器の設定
		if (auto* weapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = weapon->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					weapon->SetParentBone(&iterBone);
					break;
				}
			}
		}
		// 当たり判定用モデルの設定
		else if (auto* collModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = collModel->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					collModel->SetParentBone(&iterBone);
					break;
				}
			}
		}
	}
		
	return true;
}

//**************************************************
// 編集システム読込処理
//**************************************************
bool CEdit::ReadModel(void)
{
	// マテリアルバッファの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	// 頂点マークの破棄
	for (auto& iter : m_vpVtxMark)
	{
		iter->Uninit();
	}
	m_vpVtxMark.clear();

	// ボーン破棄
	m_vBone.clear();

	// 頂点編集用破棄
	m_vVtxEdit.clear();

	// 選択中の頂点グループ破棄
	m_vpTouchVtxGroup.clear();

	// 触れいているボーンをリセット
	m_pTouchBone = NULL;

	// 触れている頂点グループをリセット
	m_pTouchVtxGroup = NULL;

	// 頂点グループ破棄
	for (auto& iter : m_vVtxGroup)
	{
		iter.vVtxIdx.clear();
	}
	m_vVtxGroup.clear();

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// ボーン数リセット
	m_nNumBone = 0;

	// 初期化
	HRESULT hr = Init(CManager::GetFilePathModel().c_str());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

//**************************************************
// 編集読込処理
//**************************************************
bool CEdit::SkinInfoRead(void)
{
	// 読み取り用としてファイルを開く
	ifstream ifs(CManager::GetFilePathReadSkinMesh().c_str(), ios::binary);

	// ファイルが開けなかった
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CEdit::Read() > ファイルが開けませんでした", "Error", MB_OK);
		return false;
	}

	m_nSelectWeapon = -1;				// 今選択している武器
	m_pWeaponOperate = nullptr;			// 操る武器
	m_nSelectCollModel = -1;			// 今選択している当たり判定用モデル
	m_pCollModelOperate = nullptr;		// 操る当たり判定用モデル

	// エンティティ破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// コンポーネントリスト空
	m_vectorCollModel.clear();
	m_vectorWeapon.clear();

	// エンティティ生成
	m_pPlayer = new CEntity;

	// マテリアルバッファの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	// 頂点マークの破棄
	for (auto& iter : m_vpVtxMark)
	{
		iter->Uninit();
	}
	m_vpVtxMark.clear();

	// ボーン破棄
	m_vBone.clear();

	// 頂点編集用破棄
	m_vVtxEdit.clear();

	// 選択中の頂点グループ破棄
	m_vpTouchVtxGroup.clear();

	// 触れいているボーンをリセット
	m_pTouchBone = NULL;

	// 触れている頂点グループをリセット
	m_pTouchVtxGroup = NULL;

	// 頂点グループ破棄
	for (auto& iter : m_vVtxGroup)
	{
		iter.vVtxIdx.clear();
	}
	m_vVtxGroup.clear();

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// ボーン数リセット
	m_nNumBone = 0;

	string str;					// 文字列
	char cData;					// 一文字
	CBone::BONE boneRead;		// 読み取りボーン
	VERTEXEDIT vtxEditRead;		// 読み取り頂点
	int nCntVtxEdit = 0;		// 頂点数を数える変数
	D3DXVECTOR3 posRead;		// 読込位置
	D3DXQUATERNION quatRead;	// 読込向き
	string nameRead;			// 名前読込文字列
	string filePathRead;		// ファイルパス読込文字列
	int nDataRead;				// int読込用

	// 一文字ずつ読み取る
	while (ifs >> cData)
	{
		// コメント文字
		if (cData == '#')
		{
			// 一行読む
			getline(ifs, str);

			// 文字列削除
			str.clear();

			continue;
		}
		// 改行文字
		else if (cData == '\n')
		{
			// 一行読む
			getline(ifs, str);

			// 文字列削除
			str.clear();

			continue;
		}
		// タブ文字
		else if (cData == '\t')
		{
			continue;
		}

		// 文字列に一文字追加
		str += cData;

		// ファイルパス
		if (str == "XFILEPASS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// 初期化処理
			Init(str.c_str());

			// 文字列削除
			str.clear();
		}
		// 武器セット
		else if (str == "WEAPON_SET")
		{
			// 読込情報リセット
			posRead = InitValu::VEC3_NULL;	// 読込位置
			quatRead = InitValu::QUAT_NULL;	// 読込向き
			nameRead.clear();				// 名前読込文字列
			filePathRead.clear();			// ファイルパス読込文字列

			// 文字列削除
			str.clear();
		}
		// 武器の名前
		else if (str == "WEAPON_NAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 名前読み取り
			CMyMath::ReadToSpace(ifs, nameRead);

			// 文字列削除
			str.clear();
		}
		// 武器のファイルパス
		else if (str == "WEAPON_XFILEPATH")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, filePathRead);

			// 文字列削除
			str.clear();
		}
		// 武器のボーンインデックス
		else if (str == "WEAPON_BONEIDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置取得
			CMyMath::ReadToSpace(ifs, str);
			nDataRead = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 武器の位置
		else if (str == "WEAPON_OFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			posRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 武器の向き
		else if (str == "WEAPON_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			quatRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// 武器の設定終わり
		else if (str == "WEAPON_END")
		{
			// コリジョン用モデルインスタンス生成
			CWeapon* pWeapon = new CWeapon;

			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 情報設定
			pWeapon->SetName(nameRead);
			pWeapon->SetXFilePath(filePathRead);
			pWeapon->SetIdxBone(nDataRead);
			pWeapon->SetPosition(posRead);
			pWeapon->SetQuat(quatRead);

			// 初期化
			pWeapon->Init();

			// コンポーネントリストに追加
			m_pPlayer->AddComponentPushBack(pWeapon);

			// リストに追加
			m_vectorWeapon.push_back(pWeapon);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルセット
		else if (str == "COLLISION_SET")
		{
			// 読込情報リセット
			posRead = InitValu::VEC3_NULL;	// 読込位置
			quatRead = InitValu::QUAT_NULL;	// 読込向き
			nameRead.clear();				// 名前読込文字列
			filePathRead.clear();			// ファイルパス読込文字列

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの名前
		else if (str == "COLLISION_NAME")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 名前読み取り
			CMyMath::ReadToSpace(ifs, nameRead);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルのファイルパス
		else if (str == "COLLISION_XFILEPATH")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ファイルパス読み取り
			CMyMath::ReadToSpace(ifs, filePathRead);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルのボーンインデックス
		else if (str == "COLLISION_BONEIDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置取得
			CMyMath::ReadToSpace(ifs, str);
			nDataRead = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの位置
		else if (str == "COLLISION_OFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			posRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			posRead.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの向き
		else if (str == "COLLISION_QUAT")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 値の読込
			CMyMath::ReadToSpace(ifs, str);
			quatRead.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.z = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			quatRead.w = stof(str);

			// 文字列削除
			str.clear();
		}
		// 当たり判定用モデルの設定終わり
		else if (str == "COLLISION_END")
		{
			// コリジョン用モデルインスタンス生成
			CCollisionModel* pCollModel = new CCollisionModel;

			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 情報設定
			pCollModel->SetName(nameRead);
			pCollModel->SetXFilePath(filePathRead);
			pCollModel->SetIdxBone(nDataRead);
			pCollModel->SetPosition(posRead);
			pCollModel->SetQuat(quatRead);

			// 初期化
			pCollModel->Init();

			// コンポーネントリストに追加
			m_pPlayer->AddComponentPushBack(pCollModel);

			// リストに追加
			m_vectorCollModel.push_back(pCollModel);

			// 文字列削除
			str.clear();
		}
		// ボーン開始読込
		else if (str == "BONE_SET")
		{
			// 文字列削除
			str.clear();
		}
		// ボーンインデックス
		else if (str == "BONE_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			boneRead.nIdx = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンインデックス(子)
		else if (str == "BONE_CHILD_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス(子)読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			boneRead.nIdxChild = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンインデックス(親)
		else if (str == "BONE_PARENT_IDX")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// ボーンインデックス(子)読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			boneRead.nIdxParent = stoi(str);

			// 文字列削除
			str.clear();
		}
		// ボーンの位置
		else if (str == "BONE_POS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置読み取り
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.pos.z = stof(str);

			// ボーンのリセット位置設定
			boneRead.posReset = boneRead.pos;

			// 文字列削除
			str.clear();
		}
		// ボーンの位置
		else if (str == "BONE_OFFSETPOS")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 位置読み取り
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			boneRead.posOffset.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// ボーン終了
		else if (str == "BONE_END")
		{
			// ボーンの総数カウントアップ
			m_nNumBone++;

			// 要素を追加する
			// ワールドマトリックス初期化
			D3DXMatrixIdentity(&boneRead.mtxWorld);
			D3DXMatrixIdentity(&boneRead.mtxRot);
			boneRead.pBoneChild = NULL;
			boneRead.pBoneParent = NULL;
			boneRead.rot = InitValu::VEC3_NULL;
			m_vBone.push_back(boneRead);

			// ボーンモデルの生成
			CBone* pBoneModel = CBone::Create();

			// リストに追加
			m_vpBoneModel.push_back(pBoneModel);

			// 読込情報リセット
			ZeroMemory(&boneRead, sizeof(boneRead));

			// 文字列削除
			str.clear();
		}
		// 頂点開始
		else if (str == "VTX_SET")
		{
			// 文字列削除
			str.clear();
		}
		// 頂点ボーンインデックス0
		else if (str == "VTX_IDXBONE_0")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点ボーンインデックス0読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxEditRead.aIdxBone[0] = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点ボーンインデックス1
		else if (str == "VTX_IDXBONE_1")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点ボーンインデックス1読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxEditRead.aIdxBone[1] = stoi(str);

			// 文字列削除
			str.clear();
		}
		// 頂点重さ0
		else if (str == "VTX_WAGHT_0")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点重さ0読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxEditRead.aVtxWaght[0] = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点重さ1
		else if (str == "VTX_WAGHT_1")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点重さ1読み取り
			CMyMath::ReadToSpace(ifs, str);

			// int型に変換
			vtxEditRead.aVtxWaght[1] = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点法線オフセット
		else if (str == "VTX_NOROFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			vtxEditRead.norOffSet.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxEditRead.norOffSet.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxEditRead.norOffSet.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点位置オフセット
		else if (str == "VTX_POSOFFSET")
		{
			// = まで読み取る
			CMyMath::ReadToSpace(ifs, str);

			// 頂点法線オフセット
			CMyMath::ReadToSpace(ifs, str);
			vtxEditRead.posOffSet.x = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxEditRead.posOffSet.y = stof(str);
			CMyMath::ReadToSpace(ifs, str);
			vtxEditRead.posOffSet.z = stof(str);

			// 文字列削除
			str.clear();
		}
		// 頂点終了
		else if (str == "VTX_END")
		{
			// 編集用頂点のイテレータ取得
			auto iterVtxEdit = m_vVtxEdit.begin() + nCntVtxEdit;

			// 読み取った情報設定
			(*iterVtxEdit) = vtxEditRead;

			// カウンター加算
			nCntVtxEdit++;

			// 文字列削除
			str.clear();
		}
	}

	// 親と子の再設定
	for (auto& iterBone : m_vBone)
	{
		// 子のポインタを設定
		if (iterBone.nIdxChild != -1)
		{
			auto iterBoneChild = m_vBone.begin() + iterBone.nIdxChild;
			iterBone.pBoneChild = &*iterBoneChild;
		}

		// 親のポインタを設定
		if (iterBone.nIdxParent != -1)
		{
			auto iterBoneParent = m_vBone.begin() + iterBone.nIdxParent;
			iterBone.pBoneParent = &*iterBoneParent;
		}
	}

	// 頂点グループの設定
	for (auto& iterVtxGroup : m_vVtxGroup)
	{
		// 頂点番号取得
		auto iterVtxIdx = iterVtxGroup.vVtxIdx.begin();

		// 頂点情報取得
		auto iterVtxEdit = m_vVtxEdit.begin() + (*iterVtxIdx);

		for (int nCnt = 0; nCnt < IDX_BONE_NUM; nCnt++)
		{
			iterVtxGroup.aVtxWaght[nCnt] = (*iterVtxEdit).aVtxWaght[nCnt];
			iterVtxGroup.aIdxBone[nCnt] = (*iterVtxEdit).aIdxBone[nCnt];
		}
	}

	// コンポーネントの各種設定
	for (auto& iterComp : m_pPlayer->GetComponentVector())
	{
		// 武器の設定
		if (auto* weapon = dynamic_cast<CWeapon*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = weapon->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					weapon->SetParentBone(&iterBone);
					break;
				}
			}
		}
		// 当たり判定用モデルの設定
		else if (auto* collModel = dynamic_cast<CCollisionModel*>(iterComp))
		{
			// 親ボーンインデックス取得
			int nIdxBone = collModel->GetIdxBone();

			// 親と子の再設定
			for (auto& iterBone : m_vBone)
			{
				// インデックスが一致したら
				if (iterBone.nIdx == nIdxBone)
				{
					// 親設定
					collModel->SetParentBone(&iterBone);
					break;
				}
			}
		}
	}

	// ファイルを閉じる
	ifs.close();

	return true;
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::Debug(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	CBone::BONE* pBone = NULL;

	CDebugprocc::PrintRight("<  EDIT  >\n");
	CDebugprocc::PrintRight("\n");

	// LSHIFT押したまま
	if (pInputKeyboard->GetPress(DIK_LSHIFT) == false)
	{
		CDebugprocc::PrintRight("編集タイプを変える         : [ F1 ]\n");
		CDebugprocc::PrintRight("現在の情報を保存           : [ F2 ]\n");
		CDebugprocc::PrintRight("情報の読込                 : [ F3 ]\n");
		CDebugprocc::PrintRight("モデルの読込               : [ F4 ]\n");
		CDebugprocc::PrintRight("モデルの表示非表示         : [ F5 ]\n");
		CDebugprocc::PrintRight("ボーンの表示非表示         : [ F6 ]\n");
		CDebugprocc::PrintRight("頂点の表示非表示           : [ F7 ]\n");
		CDebugprocc::PrintRight("頂点のZテスト切替          : [ F8 ]\n");
		CDebugprocc::PrintRight("当たり判定モデルの表示切替 : [ F9 ]\n");
	}
	else
	{
		CDebugprocc::PrintRight("武器の表示非表示           : [ F1 ]\n");
	}
	CDebugprocc::PrintRight("ボーンの選択               : [ UP ]\n");
	CDebugprocc::PrintRight("ボーンの選択               : [ DOWN ]\n");
	CDebugprocc::PrintRight("\n");
	CDebugprocc::PrintRight("boneNumOll : %d\n", (int)m_vBone.size());
	CDebugprocc::PrintRight("\n");

	if (m_vBone.size() > 0)
	{
		// 操作しているボーン取得
		auto iterOperateBone = m_vBone.begin() + m_nSelectBone;
		pBone = &*iterOperateBone;

		CDebugprocc::PrintRight("SelectBone...\n");
		CDebugprocc::PrintRight("nIdx       : %d\n", pBone->nIdx);
		CDebugprocc::PrintRight("nIdxParent : %d\n", pBone->nIdxParent);
		CDebugprocc::PrintRight("nIdxChild  : %d\n", pBone->nIdxChild);
		CDebugprocc::PrintRight("pos        : x %0.3f,y %0.3f,z %0.3f\n", pBone->pos.x, pBone->pos.y, pBone->pos.z);
		CDebugprocc::PrintRight("rot        : x %0.3f,y %0.3f,z %0.3f\n", pBone->rot.x, pBone->rot.y, pBone->rot.z);
		CDebugprocc::PrintRight("\n");
	}

	CDebugprocc::PrintRight("\n");
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::DebugNone(void)
{
	CDebugprocc::Print("** EDITTYPE_NONE **\n");
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::DebugRigging(void)
{
	CBone::BONE* pBone = NULL;

	CDebugprocc::Print("** EDITTYPE_BONE_RIGGING **\n");
	CDebugprocc::Print("ボーンの生成       : [ B ]\n");
	CDebugprocc::Print("ボーンの親切替     : [ LEFT ]\n");
	CDebugprocc::Print("ボーンの親切り替え : [ RIGHT ]\n");
	CDebugprocc::Print("ボーンの上移動     : [ Q ]\n");
	CDebugprocc::Print("ボーンの下移動     : [ E ]\n");
	CDebugprocc::Print("ボーンの奥移動     : [ W ]\n");
	CDebugprocc::Print("ボーンの手前移動   : [ S ]\n");
	CDebugprocc::Print("ボーンの左移動     : [ A ]\n");
	CDebugprocc::Print("ボーンの右移動     : [ D ]\n");
	CDebugprocc::Print("m_nNumBone         : %d\n", m_nNumBone);
	CDebugprocc::Print("m_nSelectBon       : %d\n", m_nSelectBone);
	CDebugprocc::Print("\n");

	if (pBone != NULL)
	{
		if (pBone->pBoneParent == NULL)
		{
			CDebugprocc::Print("pBoneParent : NULL\n");
		}
		else
		{
			CDebugprocc::Print("pBoneParent...\n");
			CDebugprocc::Print("nIdx       : %d\n", pBone->pBoneParent->nIdx);
			CDebugprocc::Print("nIdxParent : %d\n", pBone->pBoneParent->nIdxParent);
			CDebugprocc::Print("nIdxChild  : %d\n", pBone->pBoneParent->nIdxChild);
			CDebugprocc::Print("pos        : x %0.3f,y %0.3f,z %0.3f\n", pBone->pBoneParent->pos.x, pBone->pBoneParent->pos.y, pBone->pBoneParent->pos.z);
			CDebugprocc::Print("rot        : x %0.3f,y %0.3f,z %0.3f\n", pBone->pBoneParent->rot.x, pBone->pBoneParent->rot.y, pBone->pBoneParent->rot.z);
			CDebugprocc::Print("\n");
		}
	}
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::DebugSknning(void)
{
	CDebugprocc::Print("** EDITTYPE_BONE_SKINNING **\n");
	CDebugprocc::Print("m_fSetWaght : %0.3f\n", m_fSetWaght);
	CDebugprocc::Print("\n");
	CDebugprocc::Print("マウスカーソルが頂点と重なっているとき\n");
	CDebugprocc::Print("0番目に選んでいるボーンインデックスを設定する : [ 0 ]\n");
	CDebugprocc::Print("1番目に選んでいるボーンインデックスを設定する : [ 1 ]\n");
	CDebugprocc::Print("重さを上げる                                  : [ W ]\n");
	CDebugprocc::Print("重さを下げる                                  : [ S ]\n");
	CDebugprocc::Print("重さにm_fSetWaghtの値を設定                   : [ CLICK_LEFT ]\n");
	CDebugprocc::Print("m_fSetWaghtに頂点の重さの値を設定             : [ CLICK_RIGHT ]\n");
	CDebugprocc::Print("重さの値を変化させる                          : [ WHEEL ]\n");
	CDebugprocc::Print("m_fSetWaghtの値を変化させる                   : [ SHIFT_L ] + [ WHEEL ]\n");
	CDebugprocc::Print("\n");

	// 触る頂点の数分操作する
	if (m_vpTouchVtxGroup.empty() == false)
	{
		// 始めの頂点グループを取得
		auto iterTouchVtxGroup = m_vpTouchVtxGroup.begin();

		CDebugprocc::Print("SelectVtx...\n");
		for (int nCntIdx = 0; nCntIdx < IDX_BONE_NUM; nCntIdx++)
		{
			CDebugprocc::Print("iterTouchVtxGroup->aIdxBone[%d]	: %d\n", nCntIdx, (*iterTouchVtxGroup)->aIdxBone[nCntIdx]);
		}

		for (int nCntIdx = 0; nCntIdx < IDX_BONE_NUM; nCntIdx++)
		{
			CDebugprocc::Print("iterTouchVtxGroup->aVtxWaght[%d] : %0.3f\n", nCntIdx, (*iterTouchVtxGroup)->aVtxWaght[nCntIdx]);
		}

		CDebugprocc::Print("\n");
	}
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::DebugWeapon(void)
{
	CDebugprocc::Print("** EDITTYPE_WEAPON **\n");
	CDebugprocc::Print("m_nSelectWeapon : %d\n", m_nSelectWeapon);

	// 操る武器があれば
	if (m_pWeaponOperate != nullptr)
	{
		D3DXVECTOR3 pos = m_pWeaponOperate->GetPosition();
		D3DXQUATERNION quat = m_pWeaponOperate->GetQuat();
		CDebugprocc::Print("name      : %s\n", m_pWeaponOperate->GetName().c_str());
		CDebugprocc::Print("XfilePath : %s\n", m_pWeaponOperate->GetXFilePath().c_str());
		CDebugprocc::Print("IdxBone   : %d\n", m_pWeaponOperate->GetIdxBone());
		CDebugprocc::Print("Position  : x%0.3f y%0.3f z%0.3f\n", pos.x, pos.y, pos.z);
		CDebugprocc::Print("Quat      : x%0.3f y%0.3f z%0.3f w%0.3f\n", quat.x, quat.y, quat.z, quat.w);
	}
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::DebugCollModel(void)
{
	CDebugprocc::Print("** EDITTYPE_COLLMODEL **\n");
	CDebugprocc::Print("m_nSelectCollModel : %d\n", m_nSelectCollModel);
	
	// 操る武器があれば
	if (m_pCollModelOperate != nullptr)
	{
		D3DXVECTOR3 pos = m_pCollModelOperate->GetPosition();
		D3DXQUATERNION quat = m_pCollModelOperate->GetQuat();
		CDebugprocc::Print("name      : %s\n", m_pCollModelOperate->GetName().c_str());
		CDebugprocc::Print("XfilePath : %s\n", m_pCollModelOperate->GetXFilePath().c_str());
		CDebugprocc::Print("IdxBone   : %d\n", m_pCollModelOperate->GetIdxBone());
		CDebugprocc::Print("Position  : x%0.3f y%0.3f z%0.3f\n", pos.x, pos.y, pos.z);
		CDebugprocc::Print("Quat      : x%0.3f y%0.3f z%0.3f w%0.3f\n", quat.x, quat.y, quat.z, quat.w);
	}
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::DebugPreview(void)
{
	CDebugprocc::Print("** EDITTYPE_BONE_PREVIEW **\n");
}

//**************************************************
// 編集機能追加クラス
//**************************************************
void CEdit::AddFunction(CFunctionBase* function)
{
	// 機能追加
	m_vpFunctionBase.push_back(function);
}

//**************************************************
// 編集ボーンのサイズ変更処理
//**************************************************
void CEdit::BoneScaling(float fPlam)
{
	// すべてのボーンに適用
	for (auto& iterBoneModel : m_vpBoneModel)
	{
		float fRadius = CBone::BONE_MODEL_RADIUS * fPlam;
		iterBoneModel->SetRadius(fRadius);
	}
}

//**************************************************
// 編集ボーンの移動量変更処理
//**************************************************
void CEdit::BoneMovementScaling(float fPlam)
{
	m_fMovementBone = ADDMOVEMENT * fPlam;
}

//**************************************************
// 編集機能変更
//**************************************************
void CEdit::ChangeFunctionEdit(CFunctionEditBase* function)
{
	// 編集機能破棄
	if (m_pFunctionEdtBase != nullptr)
	{
		m_pFunctionEdtBase->Uninit();
		m_pFunctionEdtBase = nullptr;
	}

	// 機能設定
	m_pFunctionEdtBase = function;
	m_pFunctionEdtBase->Init();
}

//**************************************************
// 編集操作武器選択処理
//**************************************************
void CEdit::WeaponSelectOperate(int nAddIdx)
{
	// 空であれば処理しない
	if (m_vectorWeapon.empty() == true)return;

	// 値追加
	m_nSelectWeapon += nAddIdx;

	// 値を範囲内に設定する
	m_nSelectWeapon = Wrap(m_nSelectWeapon,0, (int)m_vectorWeapon.size() - 1);

	// 見つける
	m_pWeaponOperate = m_vectorWeapon[m_nSelectWeapon];
}

//**************************************************
// 編集親ボーン切替処理
//**************************************************
void CEdit::WeaponSelectFollowBone(int nAddIdx)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	int nIdxBone = m_pWeaponOperate->GetIdxBone();
	nIdxBone += nAddIdx;

	// 範囲外なら繰り返す
	nIdxBone = Wrap(nIdxBone,-1,(int)m_vBone.size() - 1);

	// ないのであれば
	if (nIdxBone < 0)
	{
		// 設定
		m_pWeaponOperate->SetIdxBone(-1);
		m_pWeaponOperate->SetParentBone(nullptr);
		return;
	}

	// ボーン取得
	CBone::BONE* pBone = &*(m_vBone.begin() + nIdxBone);

	// 設定
	m_pWeaponOperate->SetIdxBone(nIdxBone);
	m_pWeaponOperate->SetParentBone(pBone);
}

//**************************************************
// 編集武器位置追加処理
//**************************************************
void CEdit::WeaponAddPosition(D3DXVECTOR3 add)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	// 位置追加
	m_pWeaponOperate->AddPosition(add);
}

//**************************************************
// 編集武器位置追加処理
//**************************************************
void CEdit::WeaponResetPosition(void)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	// 位置追加
	m_pWeaponOperate->SetPosition(InitValu::VEC3_NULL);
}

//**************************************************
// 編集武器向き追加処理
//**************************************************
void CEdit::WeaponAddRotate(D3DXVECTOR3 axis)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	// 向き追加
	m_pWeaponOperate->AddRotate(axis, ADDROTATE);
}

//**************************************************
// 編集武器向き追加処理
//**************************************************
void CEdit::WeaponResetRotate(void)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	// 向き設定
	m_pWeaponOperate->SetQuat(InitValu::QUAT_NULL);
}

//**************************************************
// 編集操作武器強調表示処理
//**************************************************
void CEdit::WeaponHighlightsOperate(void)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	// ボーンの数分回す
	for (auto& iterWeapon : m_vectorWeapon)
	{
		// 同じインデックスであれば
		if (iterWeapon->GetIdxBone() == m_pWeaponOperate->GetIdxBone())
		{
			// 強調表示に設定
			iterWeapon->SetIsHilights(true);
			continue;
		}

		// 非強調表示に設定
		iterWeapon->SetIsHilights(false);
	}
}

//**************************************************
// 編集武器の親ボーン強調表示処理
//**************************************************
void CEdit::WeaponHighlightsParentBone(void)
{
	// 操る武器が無ければ処理しない
	if (m_pWeaponOperate == nullptr)return;

	// ボーンが無ければ処理しない
	if (m_vpBoneModel.size() == 0)return;

	// 親ボーンが無ければ処理しない
	if (m_pWeaponOperate->GetIdxBone() < 0)return;
	
	// ボーンのモデルに色をつける
	for (auto iterBoneModel : m_vpBoneModel)
	{
		// 通常色に設定
		iterBoneModel->SetColor(InitValu::COLOR_BONE);
	}

	// 選んでいるボーンとその親のそれぞれのモデルを取得
	auto iterTouchBoneModel = m_vpBoneModel.begin() + m_pWeaponOperate->GetIdxBone();

	// 今選んでいるボーンの色を設定する
	(*iterTouchBoneModel)->SetColor(InitValu::COLOR_BONE_PERENT);
}

//**************************************************
// 編集操作当たり判定用モデル選択処理
//**************************************************
void CEdit::CollModelSelectOperate(int nAddIdx)
{
	// 空であれば処理しない
	if (m_vectorCollModel.empty() == true)return;

	// 値追加
	m_nSelectCollModel += nAddIdx;

	// 値を範囲内に設定する
	m_nSelectCollModel = Wrap(m_nSelectCollModel, 0, (int)m_vectorCollModel.size() - 1);

	// 見つける
	m_pCollModelOperate = m_vectorCollModel[m_nSelectCollModel];
}

//**************************************************
// 編集親ボーン切替処理
//**************************************************
void CEdit::CollModelSelectFollowBone(int nAddIdx)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	int nIdxBone = m_pCollModelOperate->GetIdxBone();
	nIdxBone += nAddIdx;

	// 範囲外なら繰り返す
	nIdxBone = Wrap(nIdxBone, -1, (int)m_vBone.size() - 1);

	// ないのであれば
	if (nIdxBone < 0)
	{
		// 設定
		m_pCollModelOperate->SetIdxBone(-1);
		m_pCollModelOperate->SetParentBone(nullptr);
		return;
	}

	// ボーン取得
	CBone::BONE* pBone = &*(m_vBone.begin() + nIdxBone);

	// 設定
	m_pCollModelOperate->SetIdxBone(nIdxBone);
	m_pCollModelOperate->SetParentBone(pBone);
}

//**************************************************
// 編集当たり判定用モデル位置追加処理
//**************************************************
void CEdit::CollModelAddPosition(D3DXVECTOR3 add)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	// 位置追加
	m_pCollModelOperate->AddPosition(add);
}

//**************************************************
// 編集当たり判定用モデル位置追加処理
//**************************************************
void CEdit::CollModelResetPosition(void)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	// 位置追加
	m_pCollModelOperate->SetPosition(InitValu::VEC3_NULL);
}

//**************************************************
// 編集当たり判定用モデル向き追加処理
//**************************************************
void CEdit::CollModelAddRotate(D3DXVECTOR3 axis)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	// 向き追加
	m_pCollModelOperate->AddRotate(axis, ADDROTATE);
}

//**************************************************
// 編集当たり判定用モデル向き追加処理
//**************************************************
void CEdit::CollModelResetRotate(void)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	// 向き設定
	m_pCollModelOperate->SetQuat(InitValu::QUAT_NULL);
}

//**************************************************
// 編集操作当たり判定用モデル強調表示処理
//**************************************************
void CEdit::CollModelHighlightsOperate(void)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	// ボーンの数分回す
	for (auto& iterCollModel : m_vectorCollModel)
	{
		// 同じ名前であれば
		if (iterCollModel->GetName() == m_pCollModelOperate->GetName())
		{
			// 強調表示に設定
			iterCollModel->SetIsHilights(true);
			continue;
		}

		// 非強調表示に設定
		iterCollModel->SetIsHilights(false);
	}
}

//**************************************************
// 編集当たり判定用モデルの親ボーン強調表示処理
//**************************************************
void CEdit::CollModelHighlightsParentBone(void)
{
	// 操る当たり判定用モデルが無ければ処理しない
	if (m_pCollModelOperate == nullptr)return;

	// ボーンが無ければ処理しない
	if (m_vpBoneModel.size() == 0)return;

	// 親ボーンが無ければ処理しない
	if (m_pCollModelOperate->GetIdxBone() < 0)return;

	// ボーンのモデルに色をつける
	for (auto iterBoneModel : m_vpBoneModel)
	{
		// 通常色に設定
		iterBoneModel->SetColor(InitValu::COLOR_BONE);
	}

	// 選んでいるボーンとその親のそれぞれのモデルを取得
	auto iterTouchBoneModel = m_vpBoneModel.begin() + m_pCollModelOperate->GetIdxBone();

	// 今選んでいるボーンの色を設定する
	(*iterTouchBoneModel)->SetColor(InitValu::COLOR_BONE_PERENT);
}