//*************************************
// 
// オブジェクトX
// Author Tetsuji Yamamoto
// 
//*************************************
#include "objectX.h"
#include "manager.h"

//*************************************
// オブジェクトX処理
//*************************************
CObjectX::CObjectX()
{
	m_apIdxTexture = NULL;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_dwNumMat = NULL;
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
HRESULT CObjectX::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		"date\\model\\HLSLTestBox_001.x",
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

	// テクスチャインデックスのメモリ確保
	m_apIdxTexture = new int[(int)m_dwNumMat];

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		m_apIdxTexture[nCntMat] = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
	}

	return S_OK;
}

//*************************************
// オブジェクトX処理
//*************************************
void CObjectX::Uninit(void)
{
	// マテリアルの破棄
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

	delete[] m_apIdxTexture;
	m_apIdxTexture = NULL;

	Release();
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
	CTexture* pTexture = CManager::GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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
		pDevice->SetTexture(0, pTexture->GetAddress(m_apIdxTexture[nCntMat]));

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//*************************************
// オブジェクトX処理
//*************************************
CObjectX* CObjectX::Create(void)
{
	CObjectX* pObjectX = NULL;
	int nNumAll = GetObujectNumAll();

	// オブジェクトXの生成
	if (pObjectX == NULL)
	{
		pObjectX = new CObjectX;
		if (pObjectX != NULL)
		{
			// 初期化処理
			pObjectX->Init();
		}
		else
		{
			MessageBox(NULL, "CObjectX::Create() > if ( pObjectX != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CObjectX::Create() > if ( pObjectX == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pObjectX;
}
