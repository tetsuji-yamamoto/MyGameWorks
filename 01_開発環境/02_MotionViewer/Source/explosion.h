//****************************************************************************
// 
// 爆発
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
#include "object2D.h"


// 爆発クラス
class CExplosion : public CObject2D	// CObject2D継承
{
public:
	CExplosion(int nPriority = PRIORITY_NOMAL);
	~CExplosion();

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLimitTime, int nMaxTexPattern, int nPatternLine);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion* Create(D3DXVECTOR3 pos, D3DXCOLOR col,float fWidth,float fHeight,int nLimitTime, int nMaxTexPattern, int nPatternLine);

private:
	int m_nCntTime;							// 時間計測カウンター
	int m_nLimitTime;						// 時間計測カウンター
	int m_nMaxTexPattern;					// テクスチャのパターン数
	int m_nNowPattern;						// テクスチャの今のパターン数
	int m_nPatternLine;						// パターン列
	float m_fTexPatternWidth;				// テクスチャパターン幅
	float m_fTexPatternHeight;				// テクスチャパターン高さ
};

#endif // !_EXPLOSION_H_
