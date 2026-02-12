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
#include "XYZAxis.h"
#include "functionBase.h"
#include "functionEdit.h"
#include "component.h"
#include "compWeapon.h"
#include "compCollisionModel.h"
#include "matrix.h"
#include "quaternion.h"

using namespace std;
using namespace nlohmann;

//**************************************************
// 編集コンストラク
//**************************************************
CEdit::CEdit() : CObject(PRIORITY_3)
{	
	m_pMotionSkin = NULL;								// モーション情報
	m_nEditType = 0; 									// 編集タイプ
	m_nNumBone = 0;										// ボーンの総数
	m_nNumMotion = 0;									// モーションの総数
	m_nSelectBoneIdx = 0;								// 今選択しているボーン
	m_nSelectmotion = 0;								// 今選択しているモーション
	m_nSelectKeyInfo = 0;								// 今選択してるキーセット番号
	m_pTouchBone = NULL;								// 触れているボーン
	m_pXYZAxis = NULL;									// XYZ軸
	m_quatCpy = InitValu::QUAT_NULL;					// コピー用クォータニオン
	m_nCntTime = 0;										// タイムカウンター
	m_bOutSave = true;									// オートセーブ
	m_vecCpy = InitValu::VEC3_NULL;						// コピー用ベクトル
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
HRESULT CEdit::InitBinary(std::string motionFilePathBinary)
{
	ios::binary;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// スキンメッシュ用情報の破棄
	if (m_pMotionSkin != NULL)
	{
		m_pMotionSkin->Uninit();
	}

	// 触れいているボーンをリセット
	m_pTouchBone = NULL;

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// 編集用モーション破棄
	m_vMotionEdit.clear();

	// ボーン数リセット
	m_nNumBone = 0;

	// スキンメッシュ用モーション情報インスタンス生成
	m_pMotionSkin = CMotionSkin::Create(motionFilePathBinary);
	
	// 生成できていなければリターン
	if (m_pMotionSkin == NULL)
	{
		return E_FAIL;
	}

	// ボーンの数を取得
	m_nNumBone = m_pMotionSkin->GetNumBone();

	// モーション数を取得
	m_nNumMotion = m_pMotionSkin->GetNumMotion();
	
	// モーションの数分回す
	for (auto& iterMotion : m_pMotionSkin->ReferenceVectorMotion())
	{
		// モーション情報をコピーする
		m_vMotionEdit.push_back(iterMotion);
	}

	// キーセットをボーンに適用
	m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);

	// 触れているボーン無し
	m_pTouchBone = NULL;

	// XYZ軸生成
	if (m_pXYZAxis == NULL)
	{
		m_pXYZAxis = CXYZAxis::Create(InitValu::VEC3_NULL);
	}


	// 結果を返す
	return S_OK;
}

//**************************************************
// 編集初期化
//**************************************************
HRESULT CEdit::Init(std::string motionFilePath)
{
	ios::binary;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// スキンメッシュ用情報の破棄
	if (m_pMotionSkin != NULL)
	{
		m_pMotionSkin->Uninit();
	}

	// 触れいているボーンをリセット
	m_pTouchBone = NULL;

	// ボーンのモデル破棄
	for (auto& iter : m_vpBoneModel)
	{
		iter->Uninit();
	}
	m_vpBoneModel.clear();

	// 編集用モーション破棄
	m_vMotionEdit.clear();

	// ボーン数リセット
	m_nNumBone = 0;

	// スキンメッシュ用モーション情報インスタンス生成
	m_pMotionSkin = CMotionSkin::Create(motionFilePath);

	// 生成できていなければリターン
	if (m_pMotionSkin == NULL)
	{
		return E_FAIL;
	}

	// ボーンの数を取得
	m_nNumBone = m_pMotionSkin->GetNumBone();

	// モーション数を取得
	m_nNumMotion = m_pMotionSkin->GetNumMotion();

	// モーションの数分回す
	for (auto& iterMotion : m_pMotionSkin->ReferenceVectorMotion())
	{
		// モーション情報をコピーする
		m_vMotionEdit.push_back(iterMotion);
	}

	// キーセットをボーンに適用
	m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);

	// 触れているボーン無し
	m_pTouchBone = NULL;

	// XYZ軸生成
	if (m_pXYZAxis == NULL)
	{
		m_pXYZAxis = CXYZAxis::Create(InitValu::VEC3_NULL);
	}

	AddFunction(new CFunctionEditPrts);	// パーツ編集機能追加

	// 結果を返す
	return S_OK;
}

//**************************************************
// 編集終了
//**************************************************
void CEdit::Uninit(void)
{
	// 機能削除
	for (auto& iter : m_vpFunction)
	{
		iter->Uninit();
	}
	m_vpFunction.clear();

	// スキンメッシュ用情報の破棄
	if (m_pMotionSkin != NULL)
	{
		m_pMotionSkin->Uninit();
	}

	// XYZ軸破棄
	if (m_pXYZAxis != NULL)
	{
		m_pXYZAxis->Uninit();
		m_pXYZAxis = NULL;
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

	// スキンメッシュ情報が無ければ処理しない
	if (m_pMotionSkin == NULL)
	{
		return;
	}

	// タイマーカウント
	m_nCntTime++;
	m_nCntTime = Wrap(m_nCntTime,0,SAVETIME);

	// オートセーブ
	if (m_nCntTime == SAVETIME)
	{
		// 可能であれば実行
		if (m_bOutSave == true)
		{
			WriteMotion();
			WriteMotionBinary();
		}
	}

	if (pInputKeyboard->GetPress(DIK_LSHIFT) == false)
	{
		// 編集タイプ切替
		if (pInputKeyboard->GetTrigger(DIK_F1) == true)
		{
			// タイプを一つ進める
			m_nEditType++;

			// 最大値まで行ったら0に戻す
			m_nEditType = Wrap(m_nEditType, (int)EDITTYPE_MOTION, (int)EDITTYPE_MAX - 1);

			// 変種タイプによって処理を変える
			switch (m_nEditType)
			{
				// モーション
			case EDITTYPE_MOTION:
				break;

				// プレビュー
			case EDITTYPE_PREVIEW:
				m_pMotionSkin->SetMotion(m_nSelectmotion, 10);
				break;

				// 例外
			default:
				break;
			}
		}
		// モーションを書き込み
		else if (pInputKeyboard->GetTrigger(DIK_F2) == true)
		{
			int nID = MessageBox(NULL, "現在のモーションを書き込みますか？", "Error", MB_YESNO);
			if (nID == IDYES)
			{
				// 書き込めたら
				if (WriteMotion() == true && WriteMotionBinary() == true)
				{
					MessageBox(NULL, "書き込み完了", "メッセージ", MB_YESNO);
				}
			}
		}
		// モーションを読込
		else if (pInputKeyboard->GetTrigger(DIK_F3) == true)
		{
			int nID = MessageBox(NULL, "モーションを読み込みますか？", "Error", MB_YESNO);
			if (nID == IDYES)
			{
				ReadMotionBinary();
			}
		}
		// ボーンの表示非表示
		else if (pInputKeyboard->GetTrigger(DIK_F7) == true)
		{
			CBone::SwitchIsDisp();
		}
		// デバッグ表示用ポリゴン表示非表示
		else if (pInputKeyboard->GetTrigger(DIK_F8) == true)
		{
			CDebugprocc::FlickeringPolygon();
		}
		// XYZ軸表示非表示
		else if (pInputKeyboard->GetTrigger(DIK_F9) == true)
		{
			CXYZAxis::SwitchIsDisp();
		}
	}
	else
	{
		// 当たり判定モデル表示切替
		if (pInputKeyboard->GetTrigger(DIK_F1) == true)
		{
			CCollisionModel::SwitchDisp();
		}
		// デバッグ表示非表示
		else if (pInputKeyboard->GetTrigger(DIK_F2) == true)
		{
			CDebugprocc::SwitchIsDisp();
		}
		// カメラリセット
		else if (pInputKeyboard->GetTrigger(DIK_F3) == true)
		{
			pCamera->SetDistance(500.0f);
			pCamera->SetPositionR(InitValu::VEC3_NULL);
			pCamera->SetRotation(D3DXVECTOR3(-D3DX_PI * 0.25f, 0.0f, 0.0f));
			pCamera->ArrangeV();
		}
		// オートセーブ切替
		else if (pInputKeyboard->GetTrigger(DIK_F4) == true)
		{
			SwitchIsOutSave();
		}
	}
	

	// 変種タイプによって処理を変える
	switch (m_nEditType)
	{
		// モーション
	case EDITTYPE_MOTION:
		editMotion();
		break;

		// プレビュー
	case EDITTYPE_PREVIEW:
		editPreview();
		break;

		// 例外
	default:
		break;
	}

	// スキンメッシュ用情報の更新
	if (m_pMotionSkin != NULL)
	{
		m_pMotionSkin->Update();
	}

	// ボーンをスケーリング
	m_pMotionSkin->BoneScaling();

	// ボーンモデルのすべての色を変える
	m_pMotionSkin->BoneModelOllSetColor(m_nSelectBoneIdx, InitValu::COLOR_BONE, InitValu::COLOR_BONE_PERENT, InitValu::COLOR_BONE_SELECT);

	// デバッグ表示
	Debug();
}

//**************************************************
// 編集描画
//**************************************************
void CEdit::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// スキンメッシュ用情報の描画
	if (m_pMotionSkin != NULL)
	{
		m_pMotionSkin->Draw();
	}
}

//**************************************************
// 編集生成
//**************************************************
CEdit* CEdit::Create(std::string motionFilePathBinary)
{
	CEdit* pEdit = new CEdit;

	if (pEdit != NULL)
	{
		// 初期化処理
		pEdit->Init(motionFilePathBinary);
	}
	else
	{
		MessageBox(NULL, " CEdit::Create() メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pEdit;
}

//**************************************************
// 編集ボーンを選択する処理
//**************************************************
void CEdit::SelectBone(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// ボーン切替
	if (pInputKeyboard->GetTriggerPress(DIK_UP) == true)
	{
		// 一つ進める
		m_nSelectBoneIdx++;
	}
	// ボーン切替
	else if (pInputKeyboard->GetTriggerPress(DIK_DOWN) == true)
	{
		// 一つ戻す
		m_nSelectBoneIdx--;
	}

	// 最大、最小まで行ったらループする
	m_nSelectBoneIdx = Wrap(m_nSelectBoneIdx, 0, m_nNumBone - 1);
}

//**************************************************
// 編集モーションを選択する処理
//**************************************************
void CEdit::SelectMotionEditMotion(int nSelectNum)
{
	// 0から数えたときのモーション数を計算
	int nNumMotion = m_nNumMotion - 1;
	nNumMotion = Clamp(nNumMotion, 0, m_nNumMotion);

	// モーションを切り替える
	m_nSelectmotion += nSelectNum;
	m_nSelectmotion = Wrap(m_nSelectmotion, 0, nNumMotion);

	// 選択中のモーション情報を取得
	auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;

	// キー数を超えないよにする
	m_nSelectKeyInfo = Wrap(m_nSelectKeyInfo, 0, (*iterMotion).nNumKeyInfo - 1);

	// キーセットをボーンに適用
	m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
}

//**************************************************
// 編集モーションを選択する処理
//**************************************************
void CEdit::SelectMotionEditPreview(int nSelectNum)
{
	// 0から数えたときのモーション数を計算
	int nNumMotion = m_nNumMotion - 1;
	nNumMotion = Clamp(nNumMotion, 0, m_nNumMotion);

	m_nSelectmotion += nSelectNum;
	m_nSelectmotion = Wrap(m_nSelectmotion, 0, nNumMotion);
	m_nSelectKeyInfo = 0;

	// キーセットをボーンに適用
	m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, 0);

	m_pMotionSkin->SetMotion(m_nSelectmotion, 10);
}

//**************************************************
// 編集モーション作成
//**************************************************
void CEdit::editMotion(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// 右クリック中注視点操作
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		pCamera->OperateMousePosV();
	}
	// 左クリック中注視点操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		pCamera->OperateMousePosR();
	}
	// 中ボタンスライド操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		pCamera->OperateMouseSlide();
	}

	// マウスで距離を変える処理
	pCamera->OperateMouseDistance();

	float fAddRotate = ADDROTATE;
	float fAddMovement = ADDPOSITION * (pCamera->GetDistance() / 90.0f);

	// 連続していれば入力量増加
	if (pInputKeyboard->GetRapid() == true)
	{
		fAddRotate *= 10.0f;
	}

	// ボーンのX軸回転(手前)
	if (pInputKeyboard->GetTriggerPress(DIK_W) == true)
	{
		m_pMotionSkin->RotateBone(m_nSelectBoneIdx, fAddRotate, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	}
	// ボーンのX軸回転(奥)
	else if (pInputKeyboard->GetTriggerPress(DIK_S) == true)
	{
		m_pMotionSkin->RotateBone(m_nSelectBoneIdx, fAddRotate, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	}

	// ボーンのY軸回転
	if (pInputKeyboard->GetTriggerPress(DIK_A) == true)
	{
		m_pMotionSkin->RotateBone(m_nSelectBoneIdx, fAddRotate, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	// ボーンのY軸回転
	else if (pInputKeyboard->GetTriggerPress(DIK_D) == true)
	{
		m_pMotionSkin->RotateBone(m_nSelectBoneIdx, fAddRotate, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}

	// ボーンのZ軸回転
	if (pInputKeyboard->GetTriggerPress(DIK_Q) == true)
	{
		m_pMotionSkin->RotateBone(m_nSelectBoneIdx, fAddRotate, D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	}
	// ボーンのZ軸回転
	else if (pInputKeyboard->GetTriggerPress(DIK_E) == true)
	{
		m_pMotionSkin->RotateBone(m_nSelectBoneIdx, fAddRotate, D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	}

	// ボーンの移動X
	if (pInputKeyboard->GetTriggerPress(DIK_F) == true)
	{
		m_pMotionSkin->PositionBone(m_nSelectBoneIdx, D3DXVECTOR3(-fAddMovement, 0.0f, 0.0f));
	}
	// ボーンの移動XX
	else if (pInputKeyboard->GetTriggerPress(DIK_H) == true)
	{
		m_pMotionSkin->PositionBone(m_nSelectBoneIdx, D3DXVECTOR3(fAddMovement, 0.0f, 0.0f));
	}

	// ボーンの移動Y
	if (pInputKeyboard->GetTriggerPress(DIK_R) == true)
	{
		m_pMotionSkin->PositionBone(m_nSelectBoneIdx, D3DXVECTOR3(0.0f, fAddMovement, 0.0f));
	}
	// ボーンの移動XY
	else if (pInputKeyboard->GetTriggerPress(DIK_Y) == true)
	{
		m_pMotionSkin->PositionBone(m_nSelectBoneIdx, D3DXVECTOR3(0.0f, -fAddMovement, 0.0f));
	}

	// ボーンの移動Z
	if (pInputKeyboard->GetTriggerPress(DIK_T) == true)
	{
		m_pMotionSkin->PositionBone(m_nSelectBoneIdx, D3DXVECTOR3(0.0f, 0.0f, fAddMovement));
	}
	// ボーンの移動XZ
	else if (pInputKeyboard->GetTriggerPress(DIK_G) == true)
	{
		m_pMotionSkin->PositionBone(m_nSelectBoneIdx, D3DXVECTOR3(0.0f, 0.0f, -fAddMovement));
	}

	// ボーンの位置リセット
	if (pInputKeyboard->GetTriggerPress(DIK_B) == true)
	{
		m_pMotionSkin->ReSetPositionBone(m_nSelectBoneIdx);
	}

	// ボーンの回転のリセット
	if (pInputKeyboard->GetTriggerPress(DIK_X) == true)
	{
		m_pMotionSkin->ReSetRotateBone(m_nSelectBoneIdx);
	}

	// ボーンすべてリセット
	if (pInputKeyboard->GetTriggerPress(DIK_RETURN) == true)
	{
		m_pMotionSkin->ReSetBone();
	}

	// ボーンの選択処理
	SelectBone();

	// モーションが無ければ処理しない
	if (m_vMotionEdit.empty() == false)
	{
		// モーションの選択
		if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{
			SelectMotionEditMotion(-1);
		}
		// モーションの選択
		else if (pInputKeyboard->GetTrigger(DIK_2) == true)
		{
			SelectMotionEditMotion(1);
		}

		// 選択中のモーション情報ゲット
		auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;

		// キーセットが無ければ処理しない
		if ((*iterMotion).vKeySet.empty() == false)
		{
			// キーセットの選択
			if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
			{
				m_nSelectKeyInfo--;
				m_nSelectKeyInfo = Wrap(m_nSelectKeyInfo, 0, (*iterMotion).nNumKeyInfo - 1);

				// ボーンにキーセットを適用
				m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
			}
			// キーセットの選択
			else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
			{
				m_nSelectKeyInfo++;
				m_nSelectKeyInfo = Wrap(m_nSelectKeyInfo, 0, (*iterMotion).nNumKeyInfo - 1);

				// ボーンにキーセットを適用
				m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
			}
		}
	}

	// モーションの追加
	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		// 一つ増やす
		m_nNumMotion++;

		// モーション情報初期化
		CMotionSkin::MOTION motion;
		motion.bLoop = false;
		motion.nNumKeyInfo = 0;

		// 要素追加
		m_vMotionEdit.push_back(motion);

		// モーション情報追加
		m_pMotionSkin->AddMotion();
	}
	// キーセットの削除
	else if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		// モーションがあれば処理する
		if (m_vMotionEdit.empty() == false)
		{
			// 選択中のモーション情報取得
			auto iterMotion = m_vMotionEdit.end() - 1;

			// 要素削除
			m_vMotionEdit.erase(iterMotion);

			// モーション数を減らす
			m_nNumMotion--;

			// キーセットの上限値設定
			int nNumMotionMax = m_nNumMotion - 1;
			nNumMotionMax = Clamp(nNumMotionMax, 0, nNumMotionMax);

			// 上限下限を超えないようにする
			m_nSelectmotion = Clamp(m_nSelectmotion, 0, nNumMotionMax);

			// モーションを減らす
			m_pMotionSkin->PullMotion();

			// ボーンにキーセットを適用
			m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
		}
	}
	// キーセットの追加
	else if (pInputKeyboard->GetTrigger(DIK_K) == true)
	{
		// モーションがあれば処理する
		if (m_vMotionEdit.empty() == false)
		{
			// 選択中のモーション情報取得
			auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;

			// キーセット情報初期
			CMotionSkin::KEYSET keySet;
			keySet.nFlame = 10;
			keySet.nNumkey = m_nNumBone;

			// リセット用ボーン情報取得
			std::vector<CBone::BONE> boneReset = m_pMotionSkin->GetVectorBoneReset();
			auto iterBoneReset = boneReset.begin();

			// ボーンの数分キーを生成
			for (int nCntKey = 0; nCntKey < m_nNumBone; nCntKey++)
			{
				CMotionSkin::KEY key;

				// 位置と向きを初期値に設定
				key.fPosX = (*iterBoneReset).pos.x;
				key.fPosY = (*iterBoneReset).pos.y;
				key.fPosZ = (*iterBoneReset).pos.z;
				key.quat = (*iterBoneReset).quat;

				// 要素追加
				keySet.vKey.push_back(key);

				// 情報をずらす
				iterBoneReset++;
			}

			//	要素追加
			(*iterMotion).vKeySet.push_back(keySet);

			// キー数追加
			(*iterMotion).nNumKeyInfo++;

			// キーセット追加
			m_pMotionSkin->AddKeySet(m_nSelectmotion);
		}
	}
	// キーセットの削除
	else if (pInputKeyboard->GetTrigger(DIK_I) == true)
	{
		// モーションがあれば処理する
		if (m_vMotionEdit.empty() == false)
		{
			// モーション情報取得
			auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;
			if ((*iterMotion).vKeySet.empty() == false)
			{
				// キーセット情報取得
				auto iterKeyset = (*iterMotion).vKeySet.end() - 1;

				// キーセット削除
				m_pMotionSkin->PullKeySet(m_nSelectmotion);

				// 要素削除
				(*iterMotion).vKeySet.erase(iterKeyset);

				// キーセット数を減らす
				(*iterMotion).nNumKeyInfo--;

				// キーセットの上限値設定
				int nNumKeyMax = (*iterMotion).nNumKeyInfo - 1;
				nNumKeyMax = Clamp(nNumKeyMax, 0, nNumKeyMax);

				// 上限下限を超えないようにする
				m_nSelectKeyInfo = Clamp(m_nSelectKeyInfo, 0, nNumKeyMax);

				// ボーンにキーセットを適用
				m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
			}
		}
	}
	// モーションを適用
	else if (pInputKeyboard->GetTrigger(DIK_F4) == true)
	{
		// モーションがあれば処理する
		if (m_vMotionEdit.empty() == false)
		{
			// 選択中のモーション情報取得
			auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;

			// キーセットがあれば処理する
			if ((*iterMotion).vKeySet.empty() == false)
			{
				// 選択中のキーセット取得
				auto iterKeySet = (*iterMotion).vKeySet.begin() + m_nSelectKeyInfo;

				// キーカウンター
				int nCntKey = 0;

				// キーの数分回す
				for (auto& iterKey : (*iterKeySet).vKey)
				{
					// ボーン情報取得
					CBone::BONE* pBone = m_pMotionSkin->GetBone(nCntKey);

					// キーカウンターカウントアップ
					nCntKey++;

					// 情報が無ければ飛ばす
					if (pBone == NULL)
					{
						continue;
					}

					// 位置と向きを設定
					iterKey.fPosX = pBone->pos.x;
					iterKey.fPosY = pBone->pos.y;
					iterKey.fPosZ = pBone->pos.z;
					iterKey.quat = pBone->quat;
				}
			}

			// ボーンにキーセットを適用
			m_pMotionSkin->ApplicableKeySetBoneToKey(m_nSelectmotion, m_nSelectKeyInfo);
		}
	}
	// モーションをコピー
	else if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{
		// モーションがあれば処理する
		if (m_vMotionEdit.empty() == false)
		{
			// 選択中のモーション情報取得
			auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;

			// キーセットがあれば処理する
			if ((*iterMotion).vKeySet.empty() == false)
			{
				// キーセットの情報を取得
				auto iterKeyset = (*iterMotion).vKeySet.begin() + m_nSelectKeyInfo;

				// キーセット情報をコピー
				m_keySetCpy = (*iterKeyset);
			}
		}
	}
	// モーションをペースト
	else if (pInputKeyboard->GetTrigger(DIK_F6) == true)
	{
		// モーションがあれば処理する
		if (m_vMotionEdit.empty() == false)
		{
			// 選択中のモーション情報取得
			auto iterMotion = m_vMotionEdit.begin() + m_nSelectmotion;
			auto iterMotionSkin = m_pMotionSkin->ReferenceVectorMotion().begin() + m_nSelectmotion;

			// キーセットがあれば処理する
			if ((*iterMotion).vKeySet.empty() == false)
			{
				// キーセットの情報を取得
				auto iterKeySet = (*iterMotion).vKeySet.begin() + m_nSelectKeyInfo;
				auto iterKeysetSkin = (*iterMotionSkin).vKeySet.begin() + m_nSelectKeyInfo;

				// キーカウンター
				int nCntKey = 0;

				// キーの数分回す
				for (auto& iterKey : m_keySetCpy.vKey)
				{
					// 位置を取得
					D3DXVECTOR3 pos, rot;
					D3DXQUATERNION quat;
					pos.x = iterKey.fPosX;
					pos.y = iterKey.fPosY;
					pos.z = iterKey.fPosZ;
					quat = iterKey.quat;

					// ボーンに適用
					m_pMotionSkin->SetPositionBone(nCntKey, pos);
					m_pMotionSkin->SetRotateBoneNotParent(nCntKey, quat);

					nCntKey++;
				}
			}
		}
	}
	// クォータニオンをコピー
	else if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_quatCpy = m_pMotionSkin->GetRotateBone(m_nSelectBoneIdx);
		m_vecCpy = m_pMotionSkin->GetBoneVec3(m_nSelectBoneIdx);
	}
	// クォータニオンを貼り付け
	else if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		D3DXVECTOR3 posSelectParent =  m_pMotionSkin->GetPositionBoneParent(m_nSelectBoneIdx);
		m_pMotionSkin->SetPositionBoneChild(m_nSelectBoneIdx, posSelectParent + m_vecCpy);
	}

	// XYZ軸の位置設定
	if (m_pXYZAxis != NULL)
	{
		// 距離取得
		float fDistance = pCamera->GetDistance();
		float fPalam = fDistance / 100.0f;

		CBone::BONE* pBoneSelect = m_pMotionSkin->GetBone(m_nSelectBoneIdx);

		// ボーンのワールドマトリックす設定
		D3DXMATRIX mtx;
		D3DXMatrixIdentity(&mtx);
		//mtx._41 = pBoneSelect->mtxWorld._41;
		//mtx._42 = pBoneSelect->mtxWorld._42;
		//mtx._43 = pBoneSelect->mtxWorld._43;
		mtx = pBoneSelect->mtxWorld;
		m_pXYZAxis->SetMatrix(mtx);
		m_pXYZAxis->Scaling(fPalam);
	}

	// すべてのボーンのリセット
	if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			for (int nCntKey = 0; nCntKey < m_nNumBone; nCntKey++)
			{
				m_pMotionSkin->ReSetRotateBone(nCntKey);
			}
		}
	}

}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::editPreview(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// 右クリック中注視点操作
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		pCamera->OperateMousePosV();
	}
	// 左クリック中注視点操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		pCamera->OperateMousePosR();
	}
	// 中ボタンスライド操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		pCamera->OperateMouseSlide();
	}

	// マウスで距離を変える処理
	pCamera->OperateMouseDistance();

	// モーションがあれば後を処理する
	if (m_vMotionEdit.empty() == true)
	{
		return;
	}

	// モーションの選択
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		SelectMotionEditPreview(-1);
	}
	// モーションの選択
	else if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		SelectMotionEditPreview(1);
	}

	// 現在選択中のモーション情報を取得
	auto iterMotionEdit = m_vMotionEdit.begin() + m_nSelectmotion;
	auto iterMotionSkin = m_pMotionSkin->ReferenceVectorMotion().begin() + m_nSelectmotion;

	// キーセットが無ければ処理しない
	if ((*iterMotionEdit).vKeySet.empty() == true || (*iterMotionSkin).vKeySet.empty() == true)
	{
		return;
	}

	// キーセットの選択
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_nSelectKeyInfo--;
		m_nSelectKeyInfo = Wrap(m_nSelectKeyInfo, 0, (*iterMotionEdit).nNumKeyInfo - 1);

		// ボーンにキーセットを適用
		m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
	}
	// キーセットの選択
	else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_nSelectKeyInfo++;
		m_nSelectKeyInfo = Wrap(m_nSelectKeyInfo, 0, (*iterMotionEdit).nNumKeyInfo - 1);

		// ボーンにキーセットを適用
		m_pMotionSkin->ApplicableKeySetKeyToBone(m_nSelectmotion, m_nSelectKeyInfo);
	}

	// 現在選択中のキーセット情報取得
	auto iterKeySet = (*iterMotionEdit).vKeySet.begin() + m_nSelectKeyInfo;
	auto iterKeySetSkin = (*iterMotionSkin).vKeySet.begin() + m_nSelectKeyInfo;

	// モーションの再生
	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_pMotionSkin->SetMotion(m_nSelectmotion, 10);
	}

	// ループの有無切替
	if (pInputKeyboard->GetTrigger(DIK_L) == true)
	{
		(*iterMotionEdit).bLoop = !(*iterMotionEdit).bLoop;
		(*iterMotionSkin).bLoop = (*iterMotionEdit).bLoop;
		
		// ループするのであれば
		if ((*iterMotionSkin).bLoop == true)
		{
			// モーションが終っていない状態にする
			m_pMotionSkin->SetIsEndoMotion(false);
		}
	}

	// フレーム数を上げる
	if (pInputKeyboard->GetTriggerPress(DIK_F) == true)
	{
		(*iterKeySet).nFlame++;
	}
	// フレーム数を下げる
	else if (pInputKeyboard->GetTriggerPress(DIK_V) == true)
	{
		(*iterKeySet).nFlame--;
	}

	// フレーム数の上限下限値を超えないようにする
	(*iterKeySet).nFlame = Clamp((*iterKeySet).nFlame, 1, 100000);
	(*iterKeySetSkin).nFlame = (*iterKeySet).nFlame;

	// モーションを更新する
	m_pMotionSkin->UpdateMotion();
}

//**************************************************
// 編集デバッグ表示
//**************************************************
void CEdit::Debug(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	CBone::BONE* pBone = NULL;

	CDebugprocc::PrintRight("//**********************************************\n");
	CDebugprocc::PrintRight("//  EDIT  >\n");
	CDebugprocc::PrintRight("//**********************************************\n");
	if(m_bOutSave == true)CDebugprocc::PrintRight("オートセーブ TRUE\n");
	if(m_bOutSave == false)CDebugprocc::PrintRight("オートセーブ FALSE\n");

	if (pInputKeyboard->GetPress(DIK_LSHIFT) == false)
	{
		CDebugprocc::PrintRight("編集タイプを変える           : [ F1 ]\n");
		CDebugprocc::PrintRight("モーションを書き込む         : [ F2 ]\n");
		CDebugprocc::PrintRight("モーションを読み込む         : [ F3 ]\n");
		CDebugprocc::PrintRight("ボーンの表示非表示           : [ F7 ]\n");
		CDebugprocc::PrintRight("黒ポリゴンの表示非表示       : [ F8 ]\n");
		CDebugprocc::PrintRight("XYZ軸の表示非表示            : [ F9 ]\n");
	}
	else
	{
		CDebugprocc::PrintRight("当たり判定モデルの表示非表示 : [ F1 ]\n");
		CDebugprocc::PrintRight("デバッグ文字の表示非表示     : [ F2 ]\n");
		CDebugprocc::PrintRight("カメラ位置リセット           : [ F3 ]\n");
		CDebugprocc::PrintRight("オートセーブ切替             : [ F4 ]\n");
	}
	CDebugprocc::PrintRight("\n");
	CDebugprocc::PrintRight("モーション切替           : [ 1 ]    [ 2 ]\n");
	CDebugprocc::PrintRight("キーセット切替           : [ LEFT ] [ RIGHT ]\n");
	CDebugprocc::PrintRight("ボーンの選択             : [ UP ]   [ DOWN ]\n");
	CDebugprocc::PrintRight("\n");
	
	// 変種タイプによって処理を変える
	switch (m_nEditType)
	{
		// モーション
	case EDITTYPE_MOTION:
		CDebugprocc::PrintRight("// Press [ LSHIFT ] ----------------------------\n");
		CDebugprocc::PrintRight("モーション追加,削除  : [ M ] [ J ]\n");
		CDebugprocc::PrintRight("キーセット追加,削除  : [ K ] [ I ]\n");
		CDebugprocc::PrintRight("//----------------------------------------------\n");
		CDebugprocc::PrintRight("キーを適用           : [ F4 ]\n");
		CDebugprocc::PrintRight("キーをコピー         : [ F5 ]\n");
		CDebugprocc::PrintRight("キーをペースト       : [ F6 ]\n");
		CDebugprocc::PrintRight("Quatのコピー         : [ 8 ]\n");
		CDebugprocc::PrintRight("Quatのペースト       : [ 9 ]\n");
		CDebugprocc::PrintRight("ボーンの X軸 回転    : [ W ] [ S ]\n");
		CDebugprocc::PrintRight("ボーンの Y軸 回転    : [ A ] [ D ]\n");
		CDebugprocc::PrintRight("ボーンの Z軸 回転    : [ Q ] [ E ]\n");
		CDebugprocc::PrintRight("ボーンの X軸 移動    : [ F ] [ H ]\n");
		CDebugprocc::PrintRight("ボーンの Y軸 移動    : [ R ] [ Y ]\n");
		CDebugprocc::PrintRight("ボーンの Z軸 移動    : [ T ] [ G ]\n");
		CDebugprocc::PrintRight("\n");
		break;

		// プレビュー
	case EDITTYPE_PREVIEW:
		break;

		// 例外
	default:
		break;
	}

	CDebugprocc::PrintRight("ボーン(空色)     : ボーン\n");
	CDebugprocc::PrintRight("ボーン(オレンジ) : 選択中のボーン\n");
	CDebugprocc::PrintRight("ボーン(青)       : 選択中のボーンの親\n");
	CDebugprocc::PrintRight("\n");

	// 変種タイプによって処理を変える
	switch (m_nEditType)
	{
		// モーション
	case EDITTYPE_MOTION:
		CDebugprocc::Print("//**********************************************\n");
		CDebugprocc::Print("// EDITTYPE_MOTION\n");
		CDebugprocc::Print("//**********************************************\n");
		CDebugprocc::Print("m_nSelectBoneIdx : [%d / %d]\n",m_nSelectBoneIdx, m_nNumBone);
		CDebugprocc::Print("m_nSelectmotion  : [%d / %d]\n",m_nSelectmotion, m_nNumMotion);
		CDebugprocc::Print("m_nNumBone       : %d\n",m_nNumBone);
		CDebugprocc::Print("m_nNumMotion     : %d\n",m_nNumMotion);
		CDebugprocc::Print("\n");

		// モーション情報があれば表示
		if (m_vMotionEdit.empty() == false)
		{
			CDebugprocc::Print("//----------------------------------------------\n");
			CDebugprocc::Print("// MOTION\n");

			// 現在選択中のモーション情報を取得
			auto iterMotionEdit = m_vMotionEdit.begin() + m_nSelectmotion;
			
			// ループの有無
			if ((*iterMotionEdit).bLoop == true)
			{
				CDebugprocc::Print("bLoop  : TRUE\n");
			}
			else
			{
				CDebugprocc::Print("bLoop  : FALSE\n");
			}

			CDebugprocc::Print("nNumKeyInfo  : [%d / %d]\n", m_nSelectKeyInfo,(*iterMotionEdit).nNumKeyInfo);
			CDebugprocc::Print("\n");

			// キーセット情報
			if ((*iterMotionEdit).vKeySet.empty() == false)
			{
				CDebugprocc::Print("//----------------------------------------------\n");
				CDebugprocc::Print("// KEYSET\n");

				// 現在選択中のキーセット情報取得
				auto iterKeySet = (*iterMotionEdit).vKeySet.begin() + m_nSelectKeyInfo;

				CDebugprocc::Print("nFlame  : %d\n", (*iterKeySet).nFlame);
				CDebugprocc::Print("nNumkey : %d\n", (*iterKeySet).nNumkey);
				CDebugprocc::Print("\n");

				// キー情報
				if ((*iterKeySet).vKey.empty() == false)
				{
					// 現在選択中のキー情報取得
					auto iterKey = (*iterKeySet).vKey.begin() + m_nSelectBoneIdx;

					CDebugprocc::Print("//----------------------------------------------\n");
					CDebugprocc::Print("// KEY\n");
					CDebugprocc::Print("m_fPosX : %0.3f\n", (*iterKey).fPosX);
					CDebugprocc::Print("m_fPosY : %0.3f\n", (*iterKey).fPosY);
					CDebugprocc::Print("m_fPosZ : %0.3f\n", (*iterKey).fPosZ);
					CDebugprocc::Print("quat.x : %0.3f\n", (*iterKey).quat.x);
					CDebugprocc::Print("quat.y : %0.3f\n", (*iterKey).quat.y);
					CDebugprocc::Print("quat.z : %0.3f\n", (*iterKey).quat.z);
					CDebugprocc::Print("quat.w : %0.3f\n", (*iterKey).quat.w);
				}							  
			}
		}
		break;

		// プレビュー
	case EDITTYPE_PREVIEW:
		CDebugprocc::Print("//**********************************************\n");
		CDebugprocc::Print("// EDITTYPE_PREVIEW\n");
		CDebugprocc::Print("//**********************************************\n");
		CDebugprocc::PrintRight("ループの切替 : [ L ]\n");
		CDebugprocc::PrintRight("フレーム数の上げ下げ : [ F ] [ V ]\n");
		CDebugprocc::PrintRight("\n");

		// モーション情報があれば表示
		if (m_vMotionEdit.empty() == false)
		{
			CDebugprocc::Print("//----------------------------------------------\n");
			CDebugprocc::Print("// MOTION\n");

			// 現在選択中のモーション情報を取得
			auto iterMotionEdit = m_vMotionEdit.begin() + m_nSelectmotion;

			// キーセット情報がなければ処理しない
			if ((*iterMotionEdit).vKeySet.empty() == false)
			{
				CDebugprocc::Print("//----------------------------------------------\n");
				CDebugprocc::Print("// KEYSET\n");

				// 現在選択中のキーセット情報取得
				auto iterKeySet = (*iterMotionEdit).vKeySet.begin() + m_nSelectKeyInfo;

				CDebugprocc::Print("m_nSelectmotion   : [%d / %d]\n", m_nSelectmotion, m_nNumMotion);
				CDebugprocc::Print("m_nSelectKeyInfo  : [%d / %d]\n", m_nSelectKeyInfo, (*iterMotionEdit).nNumKeyInfo);
				CDebugprocc::Print("nFlame  : %d\n", (*iterKeySet).nFlame);

				// ループの有無
				if ((*iterMotionEdit).bLoop == true)
				{
					CDebugprocc::Print("bLoop  : TRUE\n");
				}
				else
				{
					CDebugprocc::Print("bLoop  : FALSE\n");
				}
				CDebugprocc::Print("\n");
			}
		}
		break;

		// 例外
	default:
		CDebugprocc::Print("** EDITTYPE_XXXX **\n");
		break;
	}
}

//**************************************************
// 編集モーション書き込み
//**************************************************
bool CEdit::WriteMotion(void)
{
	// ファイルを開く
	ofstream ofs(CManager::GetFilePathMotionSkinWrite());

	// ファイルが開けなかったら処理しない
	if (ofs.is_open() == NULL)
	{
		MessageBox(NULL, "CEdit::WriteMotion() > ファイルが開けませんでした", "Error", MB_OK);
		return false;
	}

	ofs << "#*******************************************************************************\n";
	ofs << "#\n";
	ofs << "# スキンメッシュ情報情報(書き込み)\n";
	ofs << "# Author Tetsuji Yamamoto\n";
	ofs << "#\n";
	ofs << "#*******************************************************************************\n";
	ofs << "XFILEPASS = " << m_pMotionSkin->GetXFilePass() << "\t# Xファイルの相対パス\n";
	ofs << "NUMBONE = " << m_pMotionSkin->GetNumBone() << "\t# ボーンの数\n";
	ofs << "NUMVTX = " << m_pMotionSkin->GetNumVtx() << "\t# 頂点数\n";
	ofs << "\n";

	ofs << "#*******************************************************************************\n";
	ofs << "# コンポーネント\n";
	ofs << "#*******************************************************************************\n";

	CEntity* pEntity = m_pMotionSkin->GetEntity();

	// コンポーネントを書き込む
	for (auto& iterComp : pEntity->GetComponentVector())
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
	for (auto& iterBone : m_pMotionSkin->GetVectorBoneReset())
	{
		ofs << "BONE_SET\n";
		ofs << "\tBONE_IDX = " << iterBone.nIdx << "\n";
		ofs << "\tBONE_CHILD_IDX = " << iterBone.nIdxChild << " \n";
		ofs << "\tBONE_PARENT_IDX = " << iterBone.nIdxParent << " \n";
		ofs << "\tBONE_POS = ";
		ofs << fixed;
		ofs << setprecision(3)<<iterBone.pos.x << " ";
		ofs << setprecision(3)<<iterBone.pos.y << " ";
		ofs << setprecision(3)<<iterBone.pos.z << "\n";
		ofs << "\tBONE_OFFSETPOS = ";
		ofs << setprecision(3)<<iterBone.posOffset.x << " ";
		ofs << setprecision(3)<<iterBone.posOffset.y << " ";
		ofs << setprecision(3)<<iterBone.posOffset.z << "\n";
		ofs << "BONE_END\n";
		ofs << "\n";
	}

	ofs << "#*******************************************************************************\n";
	ofs << "# 頂点\n";
	ofs << "#*******************************************************************************\n";

	// 頂点情報
	for (auto& iterVtx : m_pMotionSkin->GetVectorVtxSkin())
	{
		ofs << "VTX_SET\n";
		ofs << "\tVTX_IDXBONE_0 = " << iterVtx.aIdxBone[0] << "\n";
		ofs << "\tVTX_IDXBONE_1 = " << iterVtx.aIdxBone[1] << "\n";
		ofs << "\tVTX_WAGHT_0 = " << iterVtx.aVtxWaght[0] << "\n";
		ofs << "\tVTX_WAGHT_1 = " << iterVtx.aVtxWaght[1] << "\n";
		ofs << "\tVTX_NOROFFSET = ";
		ofs << setprecision(3) << iterVtx.norOffSet.x << " ";
		ofs << setprecision(3) << iterVtx.norOffSet.y << " ";
		ofs << setprecision(3) << iterVtx.norOffSet.z << "\n";
		ofs << "\tVTX_POSOFFSET = ";
		ofs << setprecision(3)<<iterVtx.posOffSet.x << " ";
		ofs << setprecision(3)<<iterVtx.posOffSet.y << " ";
		ofs << setprecision(3)<<iterVtx.posOffSet.z << "\n";
		ofs << "VTX_END\n";
		ofs << "\n";
	}
	
	// モーションの個数カウンター
	int nCntMotion = 0;

	// モーションの数分書き込む
	for (auto iterMotionEdit : m_vMotionEdit)
	{
		ofs << "#*******************************************************************************\n";
		ofs << "# モーション["<< nCntMotion << "]\n";
		ofs << "#*******************************************************************************\n";
		ofs << "MOTION_SET\n";
		ofs << "\tMOTION_LOOP = "<< (int)iterMotionEdit.bLoop <<"\n";
		ofs << "\tMOTION_NUMKEYINFO = "<< iterMotionEdit.nNumKeyInfo << "\n";
		ofs << "\n";

		// キーセットカウンター
		int nCntKeySet = 0;

		// キーセットの数分回す
		for (auto iterKeySet : iterMotionEdit.vKeySet)
		{
			ofs << "\tKEYSET_SET # [" << nCntKeySet << "/" << iterMotionEdit.nNumKeyInfo << "\n";

			ofs << "\t\tKEYSET_FLAME = "<< iterKeySet.nFlame <<"\t# フレーム数\n";
			ofs << "\t\tKEYSET_NUMKEY = "<< iterKeySet.nNumkey <<"\t# キー数\n";
			ofs << "\n";

			// キーカウンター
			int nCntKey = 0;

			// キーの数分回す
			for (auto iterKey : iterKeySet.vKey)
			{
				ofs << "\t\tKEY_SET\t# ["<< nCntKey << "/" << iterKeySet.nNumkey <<"]\n";
				ofs << "\t\t\tKEY_POS = ";
				ofs << setprecision(3)<<iterKey.fPosX<< " ";
				ofs << setprecision(3)<<iterKey.fPosY<< " ";
				ofs << setprecision(3)<<iterKey.fPosZ<< "\n";
				ofs << "\t\t\tKEY_QUAT = ";
				ofs << setprecision(3)<<iterKey.quat.x << " ";
				ofs << setprecision(3)<<iterKey.quat.y << " ";
				ofs << setprecision(3)<<iterKey.quat.z << " ";
				ofs << setprecision(3)<<iterKey.quat.w << "\n";
				ofs << "\t\tKEY_END\n";
				ofs << "\n";

				// キーカウンター増加
				nCntKey++;
			}
			ofs << "\tKEYSET_END\n";
			ofs << "\n";
		}

		ofs << "MOTION_END\n";
		ofs << "\n";

		// モーションカウンターカウントアップ
		nCntMotion++;
	}

	// ファイルを閉じるH
	ofs.close();

	return true;
}

//**************************************************
// 編集モーション読込
//**************************************************
bool CEdit::ReadMotion(void)
{
	// 初期化する
	HRESULT hr = Init(CManager::GetFilePathMotionSkinRead());
	
	// 失敗した
	if (FAILED(hr))
	{
		return false;
	}
	
	return true;
}

//**************************************************
// 編集モーション書き込み(バイナリ)
//**************************************************
bool CEdit::WriteMotionBinary(void)
{
	// ファイルを開く
	ofstream ofs(CManager::GetFilePathMotionSkinWriteBinary(),ios::binary);

	// ファイルが開けなかったら処理しない
	if (ofs.is_open() == NULL)
	{
		MessageBox(NULL, "CEdit::WriteMotionBinary() > ファイルが開けませんでした", "Error", MB_OK);
		return false;
	}

	// コンポーネント配列取得
	CEntity* pEntity = m_pMotionSkin->GetEntity();
	std::vector<CComponent*>& vectorComp = pEntity->GetComponentVector();

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
	string XFilePath = m_pMotionSkin->GetXFilePass();
	int nSizeFilePath = XFilePath.size();
	ofs.write((const char*)&nSizeFilePath, sizeof(int));

	// 武器コンポーネント数書き込み
	int nNumCompWeapon = (int)vectorWeapom.size();
	ofs.write((const char*)&nNumCompWeapon, sizeof(int));

	// ボーンの数
	int nNumBone = m_pMotionSkin->GetNumBone();
	ofs.write((const char*)&nNumBone, sizeof(int));

	// 頂点数
	int nNumVtx = m_pMotionSkin->GetNumVtx();
	ofs.write((const char*)&nNumVtx, sizeof(int));

	// モーション数
	ofs.write((const char*)&m_nNumMotion, sizeof(int));

	// Xファイルの相対パス
	for (int nCnt = 0; nCnt < nSizeFilePath; nCnt++)
	{
		char cData = XFilePath[nCnt];
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

	// ボーン情報
	vector< CBone::BONE> vBone = m_pMotionSkin->GetVectorBoneReset();
	for (auto& iterBone : vBone)
	{
		// ボーン情報
		CBone::BONE bone = iterBone;
		ofs.write((const char*)&bone, sizeof(CBone::BONE));
	}

	// 頂点情報
	vector< CMotionSkin::VERTEXSKIN> vVtxSkin = m_pMotionSkin->GetVectorVtxSkin();
	for (auto& iterVtx : vVtxSkin)
	{
		CMotionSkin::VERTEXSKIN vtxSkin = iterVtx;
		ofs.write((const char*)&vtxSkin, sizeof(CMotionSkin::VERTEXSKIN));
	}

	// モーション情報
	for (auto& iterMotion : m_vMotionEdit)
	{
		CMotionSkin::MOTION motion = iterMotion;

		// ループ
		int nLoop = (int)motion.bLoop;
		ofs.write((const char*)&nLoop, sizeof(int));
		
		// キーセットの数
		ofs.write((const char*)&motion.nNumKeyInfo, sizeof(int));

		// キーセット
		for (auto& iterKeySet : motion.vKeySet)
		{
			CMotionSkin::KEYSET keySet = iterKeySet;
			
			// フレーム数
			ofs.write((const char*)&keySet.nFlame, sizeof(int));

			// キー数
			ofs.write((const char*)&keySet.nNumkey, sizeof(int));

			// キー
			for (auto& iterKey : keySet.vKey)
			{
				CMotionSkin::KEY key = iterKey;
				
				// 位置
				ofs.write((const char*)&key.fPosX, sizeof(float));
				ofs.write((const char*)&key.fPosY, sizeof(float));
				ofs.write((const char*)&key.fPosZ, sizeof(float));

				// 向き
				ofs.write((const char*)&key.quat, sizeof(D3DXQUATERNION));
			}
		}
	}

	// ファイルを閉じる
	ofs.close();

	return true;
}

//**************************************************
// 編集モーション読込(バイナリ)
//**************************************************
bool CEdit::ReadMotionBinary(void)
{
	// 初期化する
	HRESULT hr = Init(CManager::GetFilePathMotionSkinRead());

	// 失敗した
	if (FAILED(hr))
	{
		return false;
	}

	return true;

	return true;
}