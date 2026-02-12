//****************************************************************************
// 
// 背景
// Author tetuji yamamoto
// 
//****************************************************************************
#include "number.h"
#include "renderer.h"
#include "object2D.h"
#include "manager.h"

//****************************************************************************
// 数字
//****************************************************************************
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_nNowNumber = 0;
	m_nIdxTexture = 0;
}

//****************************************************************************
// 数字
//****************************************************************************
CNumber::~CNumber()
{
}

//****************************************************************************
// 数字
//****************************************************************************
void CNumber::Init(D3DXVECTOR3 pos, D3DXCOLOR col,float fWidth, float fHeight, int nNumber)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
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
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = pos.x - fWidth;
	pVtx[0].pos.y = pos.y - fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + fWidth;
	pVtx[1].pos.y = pos.y - fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x - fWidth;
	pVtx[2].pos.y = pos.y + fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + fWidth;
	pVtx[3].pos.y = pos.y + fHeight;
	pVtx[3].pos.z = 0.0f;

	// 数字をセット
	SetNumber(nNumber);

	// テクスチャンのインデックス設定
	m_nIdxTexture = pTexture->Register(TEXTURE_FILE_PASS_NUMBER);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//****************************************************************************
// 数字
//****************************************************************************
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pVtxBuff != NULL)
	{
		MessageBox(NULL, " CNumber::Uninit() > if (m_pVtxBuff != NULL) メモリがNULLではありません", "Error", MB_OK);
	}
}

//****************************************************************************
// 数字
//****************************************************************************
void CNumber::Update(void)
{

}

//****************************************************************************
// 数字
//****************************************************************************
void CNumber::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//****************************************************************************
// 数字
//****************************************************************************
CNumber* CNumber::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber)
{
	CNumber* pNumber = NULL;
	int nNumAll = CObject::GetObujectNumAll();

	// エフェクトショックの生成
	if (pNumber == NULL)
	{
		pNumber = new CNumber;

		if (pNumber != NULL)
		{
			// 初期化処理
			pNumber->Init(pos, col,fWidth, fHeight,nNumber);	// 初期化処理
		}
		else
		{
			MessageBox(NULL, " CNumber* CNumber::Create() > if ( pNumber != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CNumber* CNumber::Create() > if ( pNumber == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pNumber;
}

//****************************************************************************
// 数字
//****************************************************************************
void CNumber::SetNumber(int nNumber)
{
	m_nNowNumber = nNumber;

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fDigitWidth = 1.0f / 10;

	pVtx[0].tex = D3DXVECTOR2(0.0f + fDigitWidth * nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fDigitWidth + fDigitWidth * nNumber, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fDigitWidth * nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fDigitWidth + fDigitWidth * nNumber, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}