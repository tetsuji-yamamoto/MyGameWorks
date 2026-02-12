//****************************************************************************
// 
// デバッグ処理
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"
#include "font.h"

#define MAX_FONT_SIZE (20)
#define MAX_DEBUG_STR_NUM (2048)

class CObject2D;

// デバッグクラス
class CDebugprocc
{
public:
	CDebugprocc();
	~CDebugprocc();

	void Init(void);
	void Uninit(void);
	static void Print(const char* fmt, ...);
	static void PrintRight(const char* fmt, ...);
	static void Draw(void);

	static LPD3DXFONT GetFont() { return m_pFont; }
	static void FlickeringPolygon(void);

	static void SwitchIsDisp(void) { m_bDisp = !m_bDisp; }

private:
	static LPD3DXFONT m_pFont;					// フォントへのポインタ
	static char m_aStr[MAX_DEBUG_STR_NUM];		// 文字列を格納するバッファ
	static char m_aStrRight[MAX_DEBUG_STR_NUM];	// 右側文字列を格納するバッファ

	static CObject2D* m_pPolygon1;
	static CObject2D* m_pPolygon2;
	static bool m_bDisp;	// 表示切替
};

#endif