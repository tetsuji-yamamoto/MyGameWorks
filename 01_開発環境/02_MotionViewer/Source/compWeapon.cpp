//****************************************************
// 
// 武器
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compWeapon.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

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
	m_apIdxTexture = nullptr;			// テクスチャへのインデックスポインター
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
	m_pParentBone = nullptr;	// 親ボーン
	m_bHilights = false;		// 強調表示の有無

	// 初期化する
	Init();
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
	CTexture* pTexture = CManager::GetTexture();

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

	// テクスチャインデックスのメモリ確保
	m_apIdxTexture = new int[(int)m_dwNumMat];

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		m_apIdxTexture[nCntMat] = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
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
	}
}

//****************************************************
// 武器生成
//****************************************************
void CWeapon::Uninit(void)
{
	// テクスチャインデックスの破棄
	if (m_apIdxTexture != NULL)
	{
		delete[] m_apIdxTexture;
		m_apIdxTexture = NULL;
	}

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
	CTexture* pTexture = CManager::GetTexture();

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
		
		// 強調表示するのであれば
		if (m_bHilights == true)
		{
			// 半透明にする
			mat.MatD3D.Diffuse.a *= 0.5f;
		}

		// マテリアルの設定
		pDevice->SetMaterial(&mat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_apIdxTexture[nCntMat]));

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
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