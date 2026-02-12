//**************************************************
// 
// テンプレート
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "range2D.h"
#include "texture.h"
#include "manager.h"

//**************************************************
// テンプレートデストラクタ
//**************************************************
CRange2D::CRange2D()
{
	m_pVtxBuff = NULL;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//**************************************************
// テンプレートコンストラクタ
//**************************************************
CRange2D::~CRange2D()
{

}

//**************************************************
// テンプレート初期化
//**************************************************
HRESULT CRange2D::Init(void)
{
	return S_OK;
}

//**************************************************
// テンプレート初期化
//**************************************************
void CRange2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	HRESULT hr;
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CREnderer::InitPoligon > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pos = pos;
	m_rot = rot;
	m_col = col;
	m_texturePass = pTexture->Register(pTexturePass);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = pos.x;
	pVtx[0].pos.y = pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + fWidth;
	pVtx[1].pos.y = pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x;
	pVtx[2].pos.y = pos.y + fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + fWidth;
	pVtx[3].pos.y = pos.y + fHeight;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************************
// テンプレート終了
//**************************************************
void CRange2D::Uninit(void)
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

//**************************************************
// テンプレート更新
//**************************************************
void CRange2D::Update(void)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + m_fWidth;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + m_fWidth;
	pVtx[3].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************************
// テンプレート描画
//**************************************************
void CRange2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_texturePass.c_str()));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// カリングしない設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			
}

//***************************************************************
// テンプレート生成
//***************************************************************
CRange2D* CRange2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	// テンプレートインスタンス生成
	CRange2D* pRange2D = new CRange2D;

	// 初期化
	if (pRange2D != nullptr)
	{
		pRange2D->Init(pos, rot, col,fWidth, fHeight,pTexturePass);
	}

	return pRange2D;
}