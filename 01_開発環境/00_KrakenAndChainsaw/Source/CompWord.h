//**************************************
// 
// ワードコンポーネント
// AouthorTetsuji Yamamoto
// 
//**************************************
#ifndef _WORD_H_
#define _WORD_H_

#include "main.h"
#include "object2D.h"
#include "fontManager.h"
#include "component.h"

// ワードクラス
class CCompWord : public CComponent
{
public:
	static constexpr UINT TEXTURE_HEIGHT = 100;	// テクスチャの縦の大きさ

	// 引数情報
	typedef struct Argment
	{
		D3DXVECTOR3		pos			= InitValu::VEC3_NULL;				// 位置
		D3DXCOLOR		col			= Color::_WHITE;					// 色
		float			fWidth		= 0.0f;								// 幅
		float			fHeight		= 0.0f;								// 高さ
		DWORD			format		= DT_CENTER;						// フォーマット
		int				nFontID		= IDR_HG明朝B;						// フォントID
		std::string		FontName	= std::string(FONT_NAME_HG明朝B);	// フォント名
		std::string		Str;											// 文字
	}Argment;

	CCompWord(Argment Arg);
	~CCompWord();

	void Init	(void);
	void Uninit	(void);
	void Update	(void);
	void Draw	(void);

	// セッター
	void SetColor		(D3DXCOLOR col);
	void SetStringWord	(const char* pStr) { if (pStr != NULL) { m_sStrWord = pStr; } }

	// ゲッター
	D3DXCOLOR GetColor(void) { return m_col; }

private:
	LPDIRECT3DTEXTURE9			m_pTextureFont;		// フォント用のテクスチャ
	LPDIRECT3DSURFACE9			m_pSurfaceTexFont;	// フォント用テクスチャのレンダーターゲット
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// 頂点バッファ
	D3DXCOLOR					m_col;				// カラー
	std::string					m_sStrFont;			// フォントの名前
	std::string					m_sStrWord;			// 書き出す文字列
	DWORD						m_format;			// 文字の書き出し形式
	int							m_nStrWidth;		// 文字列の幅
};

#endif // !_WORD_H_
