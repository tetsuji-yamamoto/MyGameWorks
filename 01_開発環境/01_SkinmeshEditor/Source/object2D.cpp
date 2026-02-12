//**********************************************
// 
// オブジェクト2D処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//**********************************************
// オブジェクト2Dコンストラクタ
//**********************************************
CObject2D::CObject2D(int nPriority) :CObject(nPriority)
{
	m_pVtxBuff = NULL;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//**********************************************
// オブジェクト2Dデストラクタ
//**********************************************
CObject2D::~CObject2D()
{
}

//**********************************************
// オブジェクト2D初期化処理
//**********************************************
HRESULT CObject2D::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
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
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	float fWCenter = SCREEN_WIDTH * 0.5f;
	float fHCenter = SCREEN_HEIGHT * 0.5f;
	float fWidth = 200.0f;

	m_pos = D3DXVECTOR3(fWCenter, fHCenter, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = fWCenter - fWidth;
	pVtx[0].pos.y = fHCenter - fWidth;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = fWCenter + fWidth;
	pVtx[1].pos.y = fHCenter - fWidth;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = fWCenter - fWidth;
	pVtx[2].pos.y = fHCenter + fWidth;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = fWCenter + fWidth;
	pVtx[3].pos.y = fHCenter + fWidth;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//**********************************************
// オブジェクト2D初期化処理
//**********************************************
HRESULT CObject2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
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
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pos = pos;
	m_rot = rot;
	m_col = col;

	// テクスチャの読込
	m_mTexturePass = pTexture->Register(pTexturePass);

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

	return S_OK;
}

//**********************************************
// オブジェクト2D終了処理
//**********************************************
void CObject2D::Uninit(void)
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

//**********************************************
// オブジェクト2D更新処理
//**********************************************
void CObject2D::Update(void)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	float fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z - D3DX_PI * 0.75f));
	pVtx[0].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z - D3DX_PI * 0.75f));
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z + D3DX_PI * 0.75f));
	pVtx[1].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z + D3DX_PI * 0.75f));
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z - D3DX_PI * 0.25f));
	pVtx[2].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z - D3DX_PI * 0.25f));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z + D3DX_PI * 0.25f));
	pVtx[3].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z + D3DX_PI * 0.25f));
	pVtx[3].pos.z = 0.0f;

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// オブジェクト2D更新処理
//**********************************************
void CObject2D::Update(float fPatternWidth, float fPatternHeight, int nNowPattern, int nTexLine)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	float fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z - D3DX_PI * 0.75f));
	pVtx[0].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z - D3DX_PI * 0.75f));
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z + D3DX_PI * 0.75f));
	pVtx[1].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z + D3DX_PI * 0.75f));
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z - D3DX_PI * 0.25f));
	pVtx[2].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z - D3DX_PI * 0.25f));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_pos.x + m_fWidth * sinf(m_rot.z + D3DX_PI * 0.25f));
	pVtx[3].pos.y = (m_pos.y + m_fHeight * cosf(m_rot.z + D3DX_PI * 0.25f));
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex.x = 0.0f + fPatternWidth * nNowPattern;
	pVtx[0].tex.y = 0.0f + fPatternHeight * nTexLine;

	pVtx[1].tex.x = fPatternWidth + fPatternWidth * nNowPattern;
	pVtx[1].tex.y = 0.0f + fPatternHeight * nTexLine;

	pVtx[2].tex.x = 0.0f + fPatternWidth * nNowPattern;
	pVtx[2].tex.y = fPatternHeight + fPatternHeight * nTexLine;

	pVtx[3].tex.x = fPatternWidth + fPatternWidth * nNowPattern;
	pVtx[3].tex.y = fPatternHeight + fPatternHeight * nTexLine;

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// オブジェクト2D更新処理
//**********************************************
void  CObject2D::Update(float fAddTexX, float fAddTexY)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = NULL;

	float fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	float fAngleA = atan2f(m_fWidth, m_fHeight);
	float fAngleB = atan2f(m_fWidth, -m_fHeight);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x + fLength * sinf(m_rot.z - fAngleB));
	pVtx[0].pos.y = (m_pos.y + fLength * cosf(m_rot.z - fAngleB));
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_pos.x + fLength * sinf(m_rot.z + fAngleB));
	pVtx[1].pos.y = (m_pos.y + fLength * cosf(m_rot.z + fAngleB));
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_pos.x + fLength * sinf(m_rot.z - fAngleA));
	pVtx[2].pos.y = (m_pos.y + fLength * cosf(m_rot.z - fAngleA));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_pos.x + fLength * sinf(m_rot.z + fAngleA));
	pVtx[3].pos.y = (m_pos.y + fLength * cosf(m_rot.z + fAngleA));
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex.x = 0.0f + fAddTexX;
	pVtx[0].tex.y = 0.0f + fAddTexY;

	pVtx[1].tex.x = 1.0f + fAddTexX;
	pVtx[1].tex.y = 0.0f + fAddTexY;

	pVtx[2].tex.x = 0.0f + fAddTexX;
	pVtx[2].tex.y = 1.0f + fAddTexY;

	pVtx[3].tex.x = 1.0f + fAddTexX;
	pVtx[3].tex.y = 1.0f + fAddTexY;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}


//**********************************************
// オブジェクト2D描画処理
//**********************************************
void CObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_mTexturePass.c_str()));

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

//**********************************************
// オブジェクト2D生成処理
//**********************************************
CObject2D* CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	CObject2D* pObject2D = NULL;

	// オブジェクト2Dの生成
	if (pObject2D == NULL)
	{
		pObject2D = new CObject2D;
		if (pObject2D != NULL)
		{
			// 初期化処理
			pObject2D->Init(pos, rot, col, fWidth, fHeight, pTexturePass);
		}
		else
		{
			MessageBox(NULL, " CObject2D* CObject2D::Create() > if (pObject2D != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CObject2D* CObject2D::Create() > if (pObject2D == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return pObject2D;
}