//**************************************
// 
// ワード
// AouthorTetsuji Yamamoto
// 
//**************************************
#include "word.h"
#include "debugproc.h"
#include "manager.h"

#define WORD_SIZE (100)

// 静的メンバー変数宣言
LPDIRECT3DTEXTURE9 CWord::m_pTextureFont = NULL;	// フォント用のテクスチャ
LPDIRECT3DSURFACE9 CWord::m_pSurfaceTexFont = NULL;	// フォント用テクスチャのレンダーターゲット

//**************************************
// ワード
//**************************************
CWord::CWord()
{
	m_pVtxBuff = NULL;
	ZeroMemory(m_cChar, sizeof(m_cChar));
}

//**************************************
// ワード
//**************************************
CWord::~CWord()
{
}

//**************************************
// ワード
//**************************************
void CWord::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, char cData[3])
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_cChar[nCnt] = cData[nCnt];
	}

	 HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CWord::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
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

	if (m_pTextureFont == NULL)
	{
		// テクスチャの生成
		hr = pDevice->CreateTexture(
			FONT_SIZE,
			FONT_SIZE,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pTextureFont,
			NULL);
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, "CWord::Init() > pDevice->CreateTexture() > 失敗", "Error", MB_OK);
		return;
	}

	if (m_pSurfaceTexFont == NULL)
	{
		// テクスチャからレンダーターゲット用のサーフェスを取得
		hr = m_pTextureFont->GetSurfaceLevel(0, &m_pSurfaceTexFont);
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, "CWord::Init() > GetSurfaceLevel() > 失敗", "Error", MB_OK);
		return;
	}
}

//**************************************
// ワード
//**************************************
void CWord::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// サーフェイスの破棄
	if (m_pSurfaceTexFont != NULL)
	{
		m_pSurfaceTexFont->Release();
		m_pSurfaceTexFont = NULL;
	}

	// テクスチャの破棄
	if (m_pTextureFont != NULL)
	{
		m_pTextureFont->Release();
		m_pTextureFont = NULL;
	}

	if (m_pVtxBuff != NULL  || m_pSurfaceTexFont != NULL || m_pTextureFont != NULL)
	{
		MessageBox(NULL, " CWord::Uninit() > メモリがNULLではありません", "Error", MB_OK);
	}
}

//**************************************
// ワード
//**************************************
void CWord::Update(void)
{

}

//**************************************
// ワード
//**************************************
void CWord::Draw(LPD3DXFONT pFont)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// デバッグフォントの取得
	CDebugprocc* pDebu = CManager::GetDebugprocc();

	RECT rect = { 0,0,FONT_SIZE,FONT_SIZE };

	// レンダーターゲットとしてテクスチャに描画
	pDevice->SetRenderTarget(0, m_pSurfaceTexFont);
	
	pDevice->Clear(0, NULL,
	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	
	// テキストの描画
	pFont->DrawText(NULL, m_cChar, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	
	// バックバッファを取得
	LPDIRECT3DSURFACE9 pBuckBuff;
	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBuckBuff);
	
	// レンダーターゲットとしてバックバッファに描画
	pDevice->SetRenderTarget(0, pBuckBuff);
	
	// テクスチャを設定
	pDevice->SetTexture(0, m_pTextureFont);
	
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

//**************************************
// ワード
//**************************************
CWord* CWord::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, char cChar)
{
	CWord* pWord = NULL;
	int nNumAll = CObject::GetObujectNumAll();

	// エフェクトショックの生成
	if (pWord == NULL)
	{
		pWord = new CWord;

		if (pWord != NULL)
		{
			// 初期化処理
			pWord->Init(pos, col, fWidth, fHeight, &cChar);	// 初期化処理
		}
		else
		{
			MessageBox(NULL, " CWord::Create() > if ( pWord != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CWord::Create() > if ( pWord == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pWord;
}

