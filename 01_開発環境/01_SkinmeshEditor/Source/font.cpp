//****************************************
// 
// フォント
// Author Tetsuji Yamamoto
// 
//****************************************
#include "font.h"
#include "renderer.h"
#include "object.h"

//****************************************
// フォント
//****************************************
CFont::CFont()
{
	m_pFont = NULL;
	m_nFontSize = 0;
}

//****************************************
// フォント
//****************************************
CFont::~CFont()
{
}

//****************************************
// フォント
//****************************************
void CFont::Init(LPSTR pFileName,int nFontSize)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_nFontSize = nFontSize;

	// デバッグ用表示フォントの生成
	D3DXCreateFont(pDevice, 
		nFontSize,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		pFileName, &m_pFont);
}

//****************************************
// フォント
//****************************************
void CFont::Uninit(void)
{
	//デバッグ表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//****************************************
// フォント
//****************************************
CFont* CFont::Create(LPSTR pFileName, int nFontSize)
{
	CFont* pFont = NULL;
	int nNumAll = CObject::GetObujectNumAll();

	// メモリ確保
	pFont = new CFont;

	if (pFont != NULL)
	{
		// 初期化処理
		pFont->Init(pFileName, nFontSize);	// 初期化処理
	}
	else
	{
		MessageBox(NULL, " CFont* Create() > if ( pFont != NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pFont;
}
