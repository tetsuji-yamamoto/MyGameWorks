//****************************************************************************
// 
// デバッグ処理
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef DEBUGPROC_H_
#define DEBUGPROC_H_

#include "main.h"

#define MAX_FONT_SIZE (20)
#define MAX_DEBUG_STR_NUM (2048)
#define DEBUG_COLOR (D3DCOLOR_RGBA(255,255,255,255))

// デバッグクラス
class CDebugprocc
{
public:
	CDebugprocc();
	~CDebugprocc();

	void Init(void);
	void Uninit(void);
	static void Print(const char *fmt,...);
	static void Draw(void);

	static LPD3DXFONT GetFont() { return m_pFont; }

private:
	static LPD3DXFONT m_pFont;	// フォントへのポインタ
	static char m_aStr[MAX_DEBUG_STR_NUM];	// 文字列を格納するバッファ
	static int m_nNumLine;	// 行数
};

#endif