//****************************************************************************
// 
// デバッグ処理
// Author tetuji yamamoto
// 
//****************************************************************************
#include "debugproc.h"
#include "fontManager.h"
#include "renderer.h"

// 静的メンバー変数宣言
LPD3DXFONT CDebugprocc::m_pFont = nullptr;		// フォントへのポインタ
char CDebugprocc::m_aStr[MAX_DEBUG_STR_NUM] = {};		// 文字列を格納するバッファ
int  CDebugprocc::m_nNumLine = 0;

//****************************************************************************
// デバッグコンストラクタ
//****************************************************************************
CDebugprocc::CDebugprocc()
{

}

//****************************************************************************
// デバッグデストラクタ
//****************************************************************************
CDebugprocc::~CDebugprocc()
{

}

//****************************************************************************
// デバッグ初期化処理
//****************************************************************************
void CDebugprocc::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// デバッグ用表示フォントの生成
	D3DXCreateFont(pDevice, MAX_FONT_SIZE, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"terminal", &m_pFont);

	// バッファのクリア
	memset(&m_aStr[0],NULL,(sizeof(m_aStr)));

}

//****************************************************************************
// デバッグ終了処理
//****************************************************************************
void CDebugprocc::Uninit(void)
{
	//デバッグ表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

	delete this;
}

//****************************************************************************
// デバッグ入力
//****************************************************************************
void CDebugprocc::Print(const char* fmt, ...)
{
	va_list args;
	char aStr[MAX_DEBUG_STR_NUM] = {};
	va_start(args, fmt);

	vsprintf(aStr, fmt, args);

	va_end(args);

	strcat(m_aStr,aStr);
}

//****************************************************************************
// デバッグ描画処理
//****************************************************************************
void CDebugprocc::Draw(void)
{
	RECT rect = { 0,MAX_FONT_SIZE,SCREEN_WIDTH,SCREEN_HEIGHT };

	// テキストの描画
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, DEBUG_COLOR);

	// バッファのクリア
	memset(&m_aStr[0], NULL, (sizeof(m_aStr)));

	// 行数カウントリセット
	m_nNumLine = 0;
}