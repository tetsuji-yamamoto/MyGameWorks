//*************************************
// 
// フォントマネージャー
// Author Tetsuji Yamamoto
// 
//*************************************
#include "fontManager.h"

//*************************************
// フォントマネージャー
//*************************************
CFontManager::CFontManager()
{
	ZeroMemory(m_apFont, sizeof(m_apFont));
}

//*************************************
// フォントマネージャー
//*************************************
CFontManager::~CFontManager()
{
}

//*************************************
// フォントマネージャー
//*************************************
void CFontManager::Init(void)
{
	for (int nCntFont = 0; nCntFont < TYPE_MAX; nCntFont++)
	{
		m_apFont[nCntFont] = CFont::Create((LPSTR)m_apFileName[nCntFont],m_aFontSize[nCntFont]);
	}
}

//*************************************
// フォントマネージャー
//*************************************
void CFontManager::Uninit(void)
{
	for (int nCntFont = 0; nCntFont < TYPE_MAX; nCntFont++)
	{
		if (m_apFont[nCntFont] != NULL)
		{
			m_apFont[nCntFont]->Uninit();
			delete m_apFont[nCntFont];
			m_apFont[nCntFont] = NULL;
		}
	}
}

//*************************************
// フォントマネージャー
//*************************************
CFontManager* CFontManager::Create(void)
{
	CFontManager* pFontMane = NULL;

	// メモリ確保
	pFontMane = new CFontManager;

	if (pFontMane != NULL)
	{
		// 初期化処理
		pFontMane->Init();	// 初期化処理
	}
	else
	{
		MessageBox(NULL, " CFontManager* Create() > if ( pFont != NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pFontMane;
}