//****************************************************************************
// 
// 背景
// Author tetuji yamamoto
// 
//****************************************************************************
#include "bg.h"
#include "texture.h"

//****************************************************************************
// 背景コンストラクタ
//****************************************************************************
CBG::CBG(int nPriority) : CObject2D(nPriority)
{
	m_type = TYPE_00;		// 種類
	m_fFeedAngle = 0.0f;	// 送り角度
	m_fFeedRate = 0.0f;		// 送り速度
	m_fAddTexX= 0.0f;		// 送り幅
	m_fAddTexY= 0.0f;		// 送り高さ
}

//****************************************************************************
// 背景デストラクタ
//****************************************************************************
CBG::~CBG()
{
}

//****************************************************************************
// 背景初期化
//****************************************************************************
HRESULT CBG::Init(float fFeedRate, float fFeedAngle, TYPE type)
{
	CObject2D::Init();
	CObject2D::SetWidth(SCREEN_WIDTH * 0.5f);
	CObject2D::SetHeight(SCREEN_HEIGHT * 0.5f);

	// 種類設定
	CObject2D::SetType(CObject2D::TYPE_BG);

	switch (type)
	{
	case TYPE_00:
	// テクスチャンのインデックス設定
	CObject2D::SetIdxTexture(CTexture::TEXTURE_BG_0);
		break;

	case TYPE_01:
		// テクスチャンのインデックス設定
		CObject2D::SetIdxTexture(CTexture::TEXTURE_BG_1);
		break;

	case TYPE_02:
		// テクスチャンのインデックス設定
		CObject2D::SetIdxTexture(CTexture::TEXTURE_BG_2);
		break;
	}

	m_type = type;
	m_fFeedAngle = fFeedAngle;
	m_fFeedRate = fFeedRate;
	m_fAddTexX = 0.0f;
	m_fAddTexY = 0.0f;

	return S_OK;
}

//****************************************************************************
// 背景終了
//****************************************************************************
void CBG::Uninit(void)
{
	m_fFeedAngle = 0.0f;
	m_fFeedRate = 0.0f;
	m_fAddTexX = 0.0f;
	m_fAddTexY = 0.0f;
	CObject2D::Uninit();
}

//****************************************************************************
// 背景更新
//****************************************************************************
void CBG::Update(void)
{
	m_fAddTexX += sinf(m_fFeedAngle) * m_fFeedRate;
	m_fAddTexY += cosf(m_fFeedAngle) * m_fFeedRate;

	CObject2D::Update(m_fAddTexX, m_fAddTexY);
}

//****************************************************************************
// 背景描画
//****************************************************************************
void CBG::Draw(void)
{
	CObject2D::Draw();
}

//****************************************************************************
// 背景作成
//****************************************************************************
CBG* CBG::Create(float fFeedRate, float fFeedAngle, TYPE type)
{
	CBG* pBG = NULL;
	int nNumAll = GetObujectNumAll();

	// オブジェクト2Dの生成
	if (pBG == NULL)
	{
		pBG = new CBG(CObject::PRIORITY_0);
		if (pBG != NULL)
		{
			// 初期化処理
			pBG->Init(fFeedRate, fFeedAngle,type);
		}
		else
		{
			MessageBox(NULL, " CBG* CBG::Create() > if ( pBG != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CBG* CBG::Create() > if ( pBG == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pBG;
}