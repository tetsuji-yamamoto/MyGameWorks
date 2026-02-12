//*************************************
// 
// フォントマネージャー
// Author Tetsuji Yamamoto
// 
//*************************************
#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include "main.h"
#include "font.h"

// フォントマネージャー
class CFontManager
{
public:

	// 種類
	typedef enum
	{
		TYPE_DEBUG = 0,
		TYPE_SCORE,
		TYPE_MAX
	}TYPE;

	CFontManager();
	~CFontManager();

	void Init(void);
	void Uninit(void);
	static CFontManager* Create(void);
	
	// ゲッター
	CFont* GetFont(TYPE type) { return m_apFont[type]; }

private:
	CFont* m_apFont[TYPE_MAX];	// フォントへのポインター

	const char* m_apFileName[TYPE_MAX] = 
	{
		FONT_JF_DOT_M_10,
		FONT_たぬき油性マジック,
	};

	int m_aFontSize[TYPE_MAX] =
	{
		18,
		200,
	};
};

#endif // !_FONTMANAGER_H_
