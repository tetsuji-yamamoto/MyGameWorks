//**************************************
// 
// ワード
// AouthorTetsuji Yamamoto
// 
//**************************************
#ifndef _WORD_H_
#define _WORD_H_

#include "main.h"
#include "object2D.h"

// ワードクラス
class CWord
{
public:
#define FONT_SIZE (200)

	CWord();
	~CWord();

	void Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, char cData[3]);
	void Uninit(void);
	void Update(void);
	void Draw(LPD3DXFONT pFont);

	static CWord* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, char cChar);
private:
	static LPDIRECT3DTEXTURE9 m_pTextureFont;		// フォント用のテクスチャ
	static LPDIRECT3DSURFACE9 m_pSurfaceTexFont;	// フォント用テクスチャのレンダーターゲット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファ
	char m_cChar[3];									// 今の文字
};

#endif // !_WORD_H_
