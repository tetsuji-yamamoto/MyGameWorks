//*************************************
//
// オブジェクト3D
// Author Tetsuji Yamamoto
//
//*************************************
#include "object3D.h"
#include "manager.h"
#include "camera.h"

//*************************************
// オブジェクトコンストラクタ
//*************************************
CObject3D::CObject3D()
{
	m_pVtxBuff = NULL;
	ZeroMemory(m_mtxWorld,sizeof(m_mtxWorld));
	m_nIdxTexture = -1;
}

//*************************************
// オブジェクトデストラクタ
//*************************************
CObject3D::~CObject3D()
{

}

//*************************************
// オブジェクト初期化処理
//*************************************
HRESULT CObject3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CObject3D::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// テクスチャンのインデックス設定
	m_nIdxTexture = pTexture->Register("date\\texture\\image.jpg");

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -100.0f;
	pVtx[0].pos.y = 0.1f;
	pVtx[0].pos.z = 100.0f;

	pVtx[1].pos.x = 100.0f;
	pVtx[1].pos.y = 0.1f;
	pVtx[1].pos.z = 100.0f;

	pVtx[2].pos.x = -100.0f;
	pVtx[2].pos.y = 0.1f;
	pVtx[2].pos.z = -100.0f;

	pVtx[3].pos.x = 100.0f;
	pVtx[3].pos.y = 0.1f;
	pVtx[3].pos.z = -100.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*************************************
// オブジェクト終了処理
//*************************************
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pVtxBuff != NULL)
	{
		MessageBox(NULL, " CObject2D::Uninit() > if (m_pVtxBuff != NULL) メモリがNULLではありません", "Error", MB_OK);
	}

	Release();
}

//*************************************
// オブジェクト更新処理
//*************************************
void CObject3D::Update(void)
{

}

//*************************************
// オブジェクト描画処理
//*************************************
void CObject3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//*************************************
// オブジェクト生成処理
//*************************************
CObject3D* CObject3D::Create(void)
{
	CObject3D* pObject3D = NULL;
	int nNumAll = GetObujectNumAll();

	// オブジェクト2Dの生成
	if (pObject3D == NULL)
	{
		pObject3D = new CObject3D;
		if (pObject3D != NULL)
		{
			// 初期化処理
			pObject3D->Init();
		}
		else
		{
			MessageBox(NULL, " CObject3D::Create() > if ( pObject3D != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CObject3D::Create() > if ( pObject3D == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pObject3D;
}