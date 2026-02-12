//****************************************************
// 
// 武器
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compWeapon.h"
#include "compCollisionShapeSpher.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shader.h"
#include "ShaderManager.h"

using namespace std;

//****************************************************
// 武器基底コンストラクタ
//****************************************************
CWeapon::CWeapon()
{
	m_pParentBone = nullptr;			// 親ボーン
	m_pMesh = nullptr;					// メッシュ
	m_pBuffMat = nullptr;				// マテリアル
	m_dwNumMat = 0;						// マテリアルの数
	m_pos = InitValu::VEC3_NULL;		// 位置
	m_quat = InitValu::QUAT_NULL;		// 向き
	m_mtxWorld = InitValu::MTX_NULL;	// ワールドマトリックス
	m_nIdxBone = -1;					// ボーンインデックス
	m_bHilights = false;				// 強調表示の有無
}

//****************************************************
// 武器コンストラクタ
//****************************************************
CWeapon::CWeapon(const int nIdxBone, const D3DXVECTOR3 pos, const D3DXQUATERNION quat, const std::string& XFilePath)
	: m_nIdxBone(nIdxBone)
	, m_pos(pos)
	, m_quat(quat)
	, m_XFilePath(XFilePath)
{
	m_pParentBone = nullptr;			// 親ボーン
	m_bHilights = false;				// 強調表示の有無
}

//****************************************************
// 武器デストラクタ
//****************************************************
CWeapon::~CWeapon()
{

}

//****************************************************
// 武器初期化
//****************************************************
void CWeapon::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// Xファイル読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		m_XFilePath.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh
	);

	// 読込失敗
	if (FAILED(hr))
	{
		MessageBox(NULL, "CWeapon::CWeapon() > モデルを読み込めませんでした", "Error", MB_OK);

		// 結果を返す
		return;
	}

	// 頂点情報を取得----------------------------------------------------------------------------------------
	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		string texturePath = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
		m_vsTexturePath.push_back(texturePath);
	}
}

//****************************************************
// 武器生成
//****************************************************
void CWeapon::Update(void)
{
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを追加
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親ボーンが存在していたら
	if (m_pParentBone != nullptr)
	{
		// 親ボーンのマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_pParentBone->mtxWorld);

		// 当たり判定球に親マトリックスを設定
		for (auto& iterCollSpherTrigger : m_vCompCollSpherTrigger)
		{
			// 当たり判定球親マトリックスの設定
			iterCollSpherTrigger->m_pMatrixParent = &m_pParentBone->mtxWorld;
		}
	}

	// 当たっていない状態にする
	m_bCollision = false;

	// 当たり判定球更新
	for (auto& iterCollSpherTrigger : m_vCompCollSpherTrigger)
	{
		// 当たっていれば
		if (iterCollSpherTrigger->m_bCollision == true)
		{
			// 当たっているフラグをたて位置を設定
			m_bCollision = true;
			m_posHit = iterCollSpherTrigger->m_posHit;
		}

		// 更新
		iterCollSpherTrigger->Update();
	}
}

//****************************************************
// 武器生成
//****************************************************
void CWeapon::Uninit(void)
{
	// マテリアルバッファの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// 当たり判定球破棄
	for (auto& iterCollSpherTrigger : m_vCompCollSpherTrigger)
	{
		iterCollSpherTrigger->Uninit();
	}
	m_vCompCollSpherTrigger.clear();

	// 自身の破棄
	delete this;
}

//****************************************************
// 武器生成
//****************************************************
void CWeapon::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// マネージャーの取得
	CManager* pMana = CManager::GetInstance();

	// シェーダーの取得
	int nIDShader = CManager::GetInstance()->GetIDShader();
	CShader* pShaderClass = CShaderManager::GetInstance()->GetShader(nIDShader);
	LPD3DXEFFECT pShader = pShaderClass->GetShader();

	// シェーダー起動
	pShader->Begin(0, 0);

	// パラメータ(グローバル変数の設定)
	int nIDMtxWorld = pMana->GetIDPramHandlMtxWorld();
	pShader->SetMatrix(pShaderClass->GetParamaterHandleHandle(nIDMtxWorld), &m_mtxWorld);

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// 表示用マテリアル作成
		D3DXMATERIAL mat = pMat[nCntMat];

		// 色取得
		D3DXVECTOR4 defuuse = {
		mat.MatD3D.Diffuse.r,
		mat.MatD3D.Diffuse.g,
		mat.MatD3D.Diffuse.b,
		mat.MatD3D.Diffuse.a
		};

		// 透明であれば
		if (defuuse.w < 1.0f)
		{
			// Zテストオフ
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		// パラメータ(グローバル変数の設定)
		int nIDDeffuse = pMana->GetIDPramHandlDeffuse();
		pShader->SetVector(pShaderClass->GetParamaterHandleHandle(nIDDeffuse), &defuuse);

		// マテリアルの設定
		pDevice->SetMaterial(&mat.MatD3D);

		// テクスチャの取得
		LPDIRECT3DTEXTURE9 pTex = pTexture->GetAddress(m_vsTexturePath[nCntMat].c_str());

		// テクスチャの設定
		pDevice->SetTexture(0, pTex);

		// パス起動,テクスチャの有無によりパスを変更
		if (pTex != nullptr)pShader->BeginPass(3);
		else pShader->BeginPass(2);

		// 変更をGPUに反映
		pShader->CommitChanges();

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);

		// パス終了
		pShader->EndPass();

		// カリングの設定
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// Zテストオン
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// シェーダー終了
	pShader->End();
}

//****************************************************
// 武器回転
//****************************************************
void CWeapon::AddRotate(D3DXVECTOR3 axis, float fAngle)
{
	// 加えるクォータニオン
	D3DXQUATERNION quatAdd;

	// クオータニオンを求める
	D3DXQuaternionRotationAxis(&quatAdd, &axis, fAngle);

	// クオータニオンを掛け合わせる
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatAdd);
}

//****************************************************
// 武器衝突判定球トリガー追加処理
//****************************************************
void CWeapon::AddCollSpherTrigger(D3DXVECTOR3 posOffset, float fRadius)
{
	//// 当たり判定球トリガーインスタンス生成
	//CCollisionShapeSpherTrigger* m_pCollSpherTrigger = new CCollisionShapeSpherTrigger(nullptr, posOffset, fRadius);
	//
	//// 配列に追加
	//m_vCompCollSpherTrigger.push_back(m_pCollSpherTrigger);

	GetOwner()->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION, std::string("AttackChainsaw"), &m_mtxWorld, posOffset, fRadius);
}

//****************************************************
// 武器のスフィアコライダー削除
//****************************************************
void CWeapon::DeleteSpherColliderTrgger(void)
{
	// 全削除
	for (auto iterComp = m_vCompCollSpherTrigger.begin(); iterComp != m_vCompCollSpherTrigger.end();)
	{
		(*iterComp)->Uninit();
		iterComp = m_vCompCollSpherTrigger.erase(iterComp);
	}
	m_vCompCollSpherTrigger.clear();
}