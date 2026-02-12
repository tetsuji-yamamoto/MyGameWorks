//****************************************************************************
// 
// デバッグ処理
// Author tetuji yamamoto
// 
//****************************************************************************

#include "debugproc.h"
#include "renderer.h"
#include "object2D.h"

// 静的メンバー変数宣言
LPD3DXFONT CDebugprocc::m_pFont = NULL;					// フォントへのポインタ
char CDebugprocc::m_aStr[MAX_DEBUG_STR_NUM] = {};		// 文字列を格納するバッファ
char CDebugprocc::m_aStrRight[MAX_DEBUG_STR_NUM] = {};	// 文字列を格納するバッファ
CObject2D* CDebugprocc::m_pPolygon1 = NULL;
CObject2D* CDebugprocc::m_pPolygon2 = NULL;
bool CDebugprocc::m_bDisp = true;						// 表示切替

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
		FONT_TERMNAL, &m_pFont);

	// バッファのクリア
	memset(&m_aStr[0],NULL,(sizeof(m_aStr)));

	if (m_pPolygon1 == nullptr)
	{
		m_pPolygon1 = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f, 0.0f), InitValu::VEC3_NULL, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f), SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT);
	}

	if (m_pPolygon2 == nullptr)
	{
		m_pPolygon2 = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.5f, 0.0f), InitValu::VEC3_NULL, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f), SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT);
	}
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

	if (m_pPolygon1 != nullptr)
	{
		m_pPolygon1 = nullptr;
	}

	if (m_pPolygon2 != nullptr)
	{
		m_pPolygon2 = nullptr;
	}
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
// デバッグ入力
//****************************************************************************
void CDebugprocc::PrintRight(const char* fmt, ...)
{
	va_list args;
	char aStr[MAX_DEBUG_STR_NUM] = {};
	va_start(args, fmt);

	vsprintf(aStr, fmt, args);

	va_end(args);

	strcat(m_aStrRight, aStr);
}

//****************************************************************************
// デバッグ描画処理
//****************************************************************************
void CDebugprocc::Draw(void)
{
	// 表示しない場合ここで止める
	if (m_bDisp == false)
	{
		// バッファのクリア
		memset(&m_aStr[0], NULL, (sizeof(m_aStr)));
		memset(&m_aStrRight[0], NULL, (sizeof(m_aStrRight)));
		return;
	}

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	// テキストの描画
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// バッファのクリア
	memset(&m_aStr[0], NULL, (sizeof(m_aStr)));

	rect = { (LONG)((float)SCREEN_WIDTH * 0.7f),0,SCREEN_WIDTH,SCREEN_HEIGHT};

	// テキストの描画
	m_pFont->DrawText(NULL, &m_aStrRight[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// バッファのクリア
	memset(&m_aStrRight[0], NULL, (sizeof(m_aStrRight)));
}

void CDebugprocc::FlickeringPolygon(void)
{
	if (m_pPolygon1 != nullptr)
	{
		m_pPolygon1->Uninit();
		m_pPolygon1 = nullptr;
	}
	else
	{
		m_pPolygon1 = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f, 0.0f), InitValu::VEC3_NULL, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f), SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT);
	}

	if (m_pPolygon2 != nullptr)
	{
		m_pPolygon2->Uninit();
		m_pPolygon2 = nullptr;
	}
	else
	{
		m_pPolygon2 = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.5f, 0.0f), InitValu::VEC3_NULL, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f), SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT);
	}
}