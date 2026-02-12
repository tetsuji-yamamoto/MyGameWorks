//**********************************************
// 
// スコア
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _SCORE_H_
#define _SCORE_H_

#include "object.h"

// スコアクラス
class CScore : public CObject // CObject継承
{
public:
	static constexpr int MAXSORE = 99999999;	// 最大スコア

	CScore();
	~CScore();

	HRESULT Init() { return S_OK; }
	HRESULT Init(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CScore*	Create		(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight);
	void			AddScore	(int nAddScore);
	void			SetScore	(int nScore)	{ m_nScore = nScore; }

	// ゲッター
	int GetScore(void) { return m_nScore; }

private:
	int m_nScore;	// スコア
};

#endif // !_SCORE_H_
