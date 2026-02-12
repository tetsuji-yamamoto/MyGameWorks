//****************************************
// 
// フォント
// Author Tetsuji Yamamoto
// 
//****************************************
#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"

#define FONT_TERMNAL "terminal"
#define FONT_JF_DOT_M_10 "JFドットM+10"
#define FONT_HG_SOUEI_PRESENS_EB "HG創英ﾌﾟﾚｾﾞﾝｽEB"
#define FONT_OSIGO "推しゴ"
#define FONT_MUKASIMUKASI_FONT "昔々ふぉんと"
#define FONT_MOGIHA_PENJIFont "モギハ・ペン字Font"
#define FONT_たぬき油性マジック "たぬき油性マジック"

// フォントクラス
class CFont
{
public:
	CFont();
	~CFont();

	void Init(LPSTR pFileName, int nFontSize);
	void Uninit(void);
	static CFont* Create(LPSTR pFileName, int nFontSize);

	// ゲッター
	LPD3DXFONT GetFont(void) { return m_pFont; }
	int GetFontSize(void) { return m_nFontSize; }

private:
	LPD3DXFONT m_pFont;	// フォントへのポインタ
	int m_nFontSize;	// フォントサイズ
};

#endif // !_FONT_H_
