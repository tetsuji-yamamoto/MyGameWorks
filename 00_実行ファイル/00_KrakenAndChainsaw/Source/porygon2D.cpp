//**********************************************
// 
// 2Dポリゴン処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "porygon2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "myMath.h"

//**********************************************
// 2Dポリゴンコンストラクタ
//**********************************************
CPolygon2D::CPolygon2D()
{
	m_pVtxBuff = nullptr;
	m_info.fWidth = 0.0f;
	m_info.fHeight = 0.0f;
	m_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//**********************************************
// 2Dポリゴンデストラクタ
//**********************************************
CPolygon2D::~CPolygon2D()
{
}

//**********************************************
// 2Dポリゴン初期化処理
//**********************************************
void CPolygon2D::Init(Info info, const char* pTexturePass)
{
	// テクスチャ取得
	CTexture*pTexture = CManager::GetInstance()->GetTexture();

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
		return;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 各種変数の初期化
	m_info.fWidth	= info.fWidth;
	m_info.fHeight	= info.fHeight;
	m_info.pos		= info.pos;
	m_info.rot		= info.rot;
	m_info.col		= info.col;

	// テクスチャの読込
	m_mTexturePass = pTexture->Register(pTexturePass);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = info.pos.x - info.fWidth;
	pVtx[0].pos.y = info.pos.y - info.fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = info.pos.x + info.fWidth;
	pVtx[1].pos.y = info.pos.y - info.fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = info.pos.x - info.fWidth;
	pVtx[2].pos.y = info.pos.y + info.fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = info.pos.x + info.fWidth;
	pVtx[3].pos.y = info.pos.y + info.fHeight;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = info.col;
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return;
}

//**********************************************
// 2Dポリゴン初期化処理
//**********************************************
void CPolygon2D::InitLeftBottom(Info info, const char* pTexturePass)
{
	// テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

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
		return;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 各種変数の初期化
	m_info.fWidth = info.fWidth;
	m_info.fHeight = info.fHeight;
	m_info.pos = info.pos;
	m_info.rot = info.rot;
	m_info.col = info.col;

	// テクスチャの読込
	m_mTexturePass = pTexture->Register(pTexturePass);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = info.pos.x;
	pVtx[0].pos.y = info.pos.y - info.fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = info.pos.x + info.fWidth;
	pVtx[1].pos.y = info.pos.y - info.fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = info.pos.x;
	pVtx[2].pos.y = info.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = info.pos.x + info.fWidth;
	pVtx[3].pos.y = info.pos.y;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = info.col;
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return;
}

//**********************************************
// 2Dポリゴン終了処理
//**********************************************
void CPolygon2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pVtxBuff != nullptr)
	{
		MessageBox(NULL, " CPolygon2D::Uninit() > メモリがNULLではありません", "Error", MB_OK);
	}

	// 自身の破棄
	Release();
}

//**********************************************
// 2Dポリゴン更新処理
//**********************************************
void CPolygon2D::Update(void)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	float fLength = sqrtf((m_info.fWidth * m_info.fWidth) + (m_info.fHeight * m_info.fHeight));
	float fAngleA = atan2f(m_info.fWidth, m_info.fHeight);
	float fAngleB = atan2f(m_info.fWidth, -m_info.fHeight);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_info.pos.x + fLength * sinf(m_info.rot.z - fAngleB));
	pVtx[0].pos.y = (m_info.pos.y + fLength * cosf(m_info.rot.z - fAngleB));
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_info.pos.x + fLength * sinf(m_info.rot.z + fAngleB));
	pVtx[1].pos.y = (m_info.pos.y + fLength * cosf(m_info.rot.z + fAngleB));
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_info.pos.x + fLength * sinf(m_info.rot.z - fAngleA));
	pVtx[2].pos.y = (m_info.pos.y + fLength * cosf(m_info.rot.z - fAngleA));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_info.pos.x + fLength * sinf(m_info.rot.z + fAngleA));
	pVtx[3].pos.y = (m_info.pos.y + fLength * cosf(m_info.rot.z + fAngleA));
	pVtx[3].pos.z = 0.0f;

	pVtx[0].col = m_info.col;
	pVtx[1].col = m_info.col;
	pVtx[2].col = m_info.col;
	pVtx[3].col = m_info.col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// 2Dポリゴン更新処理
//**********************************************
void CPolygon2D::UpdateLeftBottom(void)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_info.pos.x - m_info.fWidth;
	pVtx[0].pos.y = m_info.pos.y - m_info.fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_info.pos.x + m_info.fWidth;
	pVtx[1].pos.y = m_info.pos.y - m_info.fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_info.pos.x - m_info.fWidth;
	pVtx[2].pos.y = m_info.pos.y + m_info.fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_info.pos.x + m_info.fWidth;
	pVtx[3].pos.y = m_info.pos.y + m_info.fHeight;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].col = m_info.col;
	pVtx[1].col = m_info.col;
	pVtx[2].col = m_info.col;
	pVtx[3].col = m_info.col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// 2Dポリゴン描画処理
//**********************************************
void CPolygon2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

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
// 2Dポリゴン生成処理
//**********************************************
CPolygon2D* CPolygon2D::Create(Info info, const char* pTexturePass)
{
	//	ポリゴンUIのインスタンス生成
	CPolygon2D* pPolygonUI = new CPolygon2D;

	// 確保できた
	if (pPolygonUI != nullptr)
	{
		// 初期化処理
		pPolygonUI->Init(info, pTexturePass);
	}
	// 出系なかった
	else
	{
		MessageBox(NULL, " CPolygon2D::Create() > メモリを確保できませんでした", "Error", MB_OK);
	}

	return pPolygonUI;
}

//**********************************************
// 2Dポリゴン生成処理
//**********************************************
CPolygon2D* CPolygon2D::CreateLeftBottom(Info info, const char* pTexturePass)
{
	//	ポリゴンUIのインスタンス生成
	CPolygon2D* pPolygonUI = new CPolygon2D;

	// 確保できた
	if (pPolygonUI != nullptr)
	{
		// 初期化処理
		pPolygonUI->InitLeftBottom(info, pTexturePass);
	}
	// 出系なかった
	else
	{
		MessageBox(NULL, " CPolygon2D::Create() > メモリを確保できませんでした", "Error", MB_OK);
	}

	return pPolygonUI;
}

//**********************************************
// 2Dポリゴン幅スケーリング
//**********************************************
void CPolygon2D::ScaleLeftBottomWidth(float fPalam)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 幅設定
	float fWidth = m_info.fWidth * fPalam;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_info.pos.x;
	pVtx[0].pos.y = m_info.pos.y - m_info.fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_info.pos.x + fWidth;
	pVtx[1].pos.y = m_info.pos.y - m_info.fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_info.pos.x;
	pVtx[2].pos.y = m_info.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_info.pos.x + fWidth;
	pVtx[3].pos.y = m_info.pos.y;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}