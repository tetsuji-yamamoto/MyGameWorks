//*************************************
// 
// オブジェクトX
// Author Tetsuji Yamamoto
// 
//*************************************
#include "objectX.h"
#include "manager.h"
#include "texture.h"

//*************************************
// オブジェクトX処理
//*************************************
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
	ZeroMemory(m_mtxWorld, sizeof(m_mtxWorld));
}

//*************************************
// オブジェクトX処理
//*************************************
CObjectX::~CObjectX()
{
}

//*************************************
// オブジェクトX処理
//*************************************
HRESULT CObjectX::Init(const char *pXFileName)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// オブジェクトタイプ設定
	CObject::SetType(CObject::TYPE_MODEL_X);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// モデルの読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		pXFileName,
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh
	);

	if (FAILED(hr))
	{// 失敗したら
		MessageBox(NULL, "InitModel() > モデWSルを読み込めませんでした", "Error", MB_OK);
	}

	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		m_vmaTexturePass[nCntMat] = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
	}

	// 結果を返す
	return S_OK;
}

//*************************************
// オブジェクトX処理
//*************************************
void CObjectX::Uninit(void)
{
	// マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}

//*************************************
// オブジェクトX処理
//*************************************
void CObjectX::Update(void)
{

}

//*************************************
// オブジェクトX処理
//*************************************
void CObjectX::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

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

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_vmaTexturePass[nCntMat].c_str()));

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//*************************************
// オブジェクトX処理
//*************************************
void CObjectX::Draw(D3DXMATRIX* pMtxPearent)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (pMtxPearent != nullptr)
	{
		m_mtxWorld = *pMtxPearent;
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_vmaTexturePass[nCntMat].c_str()));

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//*************************************
// オブジェクトX処理
//*************************************
void CObjectX::Draw(D3DXMATRIX mtxWorld)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_vmaTexturePass[nCntMat].c_str()));

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//*************************************
// オブジェクトX処理
//*************************************
CObjectX* CObjectX::Create(const char* pXFileName)
{
	// オブジェクトXインスタンス生成
	CObjectX* pObjectX = new CObjectX;
	
	if (pObjectX != nullptr)
	{
		// 初期化処理
		pObjectX->Init(pXFileName);
	}
	else
	{
		MessageBox(NULL, "CObjectX::Create() > if ( pObjectX != nullptr) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pObjectX;
}
