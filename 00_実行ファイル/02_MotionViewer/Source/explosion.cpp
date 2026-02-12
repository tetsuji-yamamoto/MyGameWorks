//****************************************************************************
// 
// 爆発
// Author tetuji yamamoto
// 
//****************************************************************************
#include "explosion.h"
#include "texture.h"

//****************************************************************************
// 爆発コンストラクタ
//****************************************************************************
CExplosion::CExplosion(int nPriority) : CObject2D(nPriority)
{
	m_nCntTime = 0;				// 時間計測カウンター
	m_nLimitTime = 0;			// 時間計測カウンター
	m_nMaxTexPattern = 0;		// テクスチャのパターン数
	m_nNowPattern = 0;			// テクスチャの今のパターン数
	m_nPatternLine = 0;			// パターン列
	m_fTexPatternWidth = 0.0f;	// テクスチャパターン幅
	m_fTexPatternHeight = 0.0f;	// テクスチャパターン高さ
}

//****************************************************************************
// 爆発デストラクタ
//****************************************************************************
CExplosion::~CExplosion()
{

}

//****************************************************************************
// 爆発初期化処理
//****************************************************************************
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLimitTime, int nMaxTexPattern, int nPatternLine)
{
	// オブジェクト2D初期化
	CObject2D::Init(pos, InitValu::VEC3_NULL, col, fWidth, fHeight);
	
	// 種類設定
	CObject2D::SetType(CObject2D::TYPE_EXPLOSION);

	// 各種変数初期化
	m_nCntTime = 0;						// 時間計測カウンター
	m_nLimitTime = nLimitTime;			// 限界時間
	m_nMaxTexPattern = nMaxTexPattern;	// テクスチャのパターン数
	m_nNowPattern = 0;					// テクスチャの今のパターン数
	m_nPatternLine = nPatternLine;		// パターン列
	m_fTexPatternWidth = fWidth;		// テクスチャパターン幅
	m_fTexPatternHeight = fHeight;		// テクスチャパターン高さ

	int nLine = m_nNowPattern / (m_nMaxTexPattern / m_nPatternLine);
	float fTexX = 1.0f / (m_nMaxTexPattern / m_nPatternLine);
	float fTexY = 1.0f / m_nPatternLine;

	// テクスチャンのインデックス設定
	CObject2D::SetIdxTexture(CTexture::TEXTURE_EXPROSION);

	// 一度アップデートする
	CObject2D::Update(fTexX, fTexY, m_nNowPattern, nLine);

	return S_OK;
}

//****************************************************************************
// 爆発終了処理
//****************************************************************************
void CExplosion::Uninit(void)
{
	CObject2D::Uninit();
}

//****************************************************************************
// 爆発更新処理
//****************************************************************************
void CExplosion::Update(void)
{
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	int nChangeTime = m_nLimitTime / m_nMaxTexPattern;

	m_nCntTime++;	// カウンター加算

	if (m_nCntTime >= nChangeTime)
	{
		m_nCntTime = 0;
		m_nNowPattern++;
		if (m_nNowPattern >= m_nMaxTexPattern)
		{
			CObject2D::Uninit();
			return;
		}
	}

	int nLine = m_nNowPattern / (m_nMaxTexPattern / m_nPatternLine);
	float fTexX = 1.0f / (m_nMaxTexPattern / m_nPatternLine);
	float fTexY = 1.0f / m_nPatternLine;

	CObject2D::Update(fTexX, 1.0f, m_nNowPattern, nLine);
}

//****************************************************************************
// 爆発描画処理
//****************************************************************************
void CExplosion::Draw(void)
{
	CObject2D::Draw();
}

//****************************************************************************
// 爆発クリエイト処理
//****************************************************************************
CExplosion* CExplosion::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLimitTime, int nMaxTexPattern, int nPatternLine)
{
	CExplosion* pExplosion = NULL;
	int nNumAll = GetObujectNumAll();

	// オブジェクト2Dの生成
	if (pExplosion == NULL)
	{

		pExplosion = new CExplosion(CObject::PRIORITY_5);
		if (pExplosion != NULL)
		{
			// 初期化処理
			pExplosion->Init(pos,col, fWidth,fHeight,nLimitTime, nMaxTexPattern, nPatternLine);
		}
		else
		{
			MessageBox(NULL, " CExplosion* CExplosion::Create() > if ( pExplosion != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CExplosion* CExplosion::Create() > if ( pExplosion == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pExplosion;
}