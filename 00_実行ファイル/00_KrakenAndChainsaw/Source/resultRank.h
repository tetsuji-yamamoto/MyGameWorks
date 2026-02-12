//**********************************************************
// 
// リザルトランククラス
// Author Tetsuji Yamamoto
// 
//**********************************************************
#ifndef  _RESULTRANK_H_
#define  _RESULTRANK_H_

#include "object.h"
#include "fontManager.h"

// 前方宣言
class CWord;		// 文字列クラス
class CPolygon2D;	// 2Dポリゴン

// リザルトランククラス
class CResultRank : public CObject
{
public:
	static constexpr int INTERVAL = 30;	// 点滅の間隔

	// 情報
	typedef struct
	{
		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 rot;			// 向き
		float fWidth;				// 幅
		float fHeight;				// 高さ
		D3DXCOLOR colWord;			// 文字通常色
		D3DXCOLOR colWordEmp;		// 文字強調色
		D3DXCOLOR colPolygon;		// ポリゴン通常色
		std::string strWord;		// 文字列
	}INFO;

	CResultRank();
	~CResultRank();

	HRESULT Init(void) { return S_OK; }
	void Init(INFO info, const char* pTexturePassPolygon = NULL, DWORD format = DT_CENTER, int nFontID = IDR_HG明朝B, const char* pFontName = FONT_NAME_HG明朝B);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	

	static CResultRank* Create(INFO info, const char* pTexturePassPolygon = NULL, DWORD format = DT_CENTER, int nFontID = IDR_HG明朝B, const char* pFontName = FONT_NAME_HG明朝B);

	// ゲッター
	
	// セッター
	void SetWord(std::string str);
	void SetIsBlink(bool bBlink) { m_bBlink = bBlink; }

private:
	CWord* m_pWord;				// 文字列クラス
	CPolygon2D* m_pPolygon2D;	// 2Dポリゴン
	INFO m_info;				// 情報
	bool m_bBlink;				// 点滅の有無
	int m_nCntBlink;			// 点滅の有無
};

#endif // ! _RESULTRANK_H_
