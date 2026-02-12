//********************************************************
// 
// タイマーコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_COUNTER_H_
#define _COMPONENT_COUNTER_H_

#include "component.h"
#include "myMath.h"

// タイマーコンポーネントクラス
class CCounter : public CComponent
{
public:

	CCounter(const int nMaxTIme,const bool bMeasure = true);
	~CCounter();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ResetCount(void){m_nTimeCount = 0;}

	// セッター
	void SetIsIncrement(bool bIncrement){m_bIncrement = bIncrement;}

	// ゲッター
	bool	GetIsMax	(void)	{return m_bMax;}
	bool	GetIsMin	(void)	{return m_bMin;}
	int		GetNow		(void)	{return m_nTimeCount;}
	float	GetPalam	(void)	{return m_fPalam;}

private:
	int		m_nTimeCount;	// タイマーカウンター
	int		m_nMax;			// タイマーカウンター
	bool	m_bMax;			// 最大化否か
	bool	m_bMin;			// 最小化否か
	bool	m_bMeasure;		// 計測の有無
	bool	m_bIncrement;	// インクリメントの有無
	float	m_fPalam;		// パラメータ
};
#endif