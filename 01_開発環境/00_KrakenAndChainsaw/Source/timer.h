//**********************************************
// 
// タイマー
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "object.h"
#include "object2D.h"

#define TIME_NOMAL (120)
#define MAX_TIME_DIGIT (4)

class CWord;	// 文字

// タイマークラス
class CTimer : public CObject // CObject継承
{
public:
	CTimer();
	~CTimer();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nTime = TIME_NOMAL);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CTimer* Create(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nTime = TIME_NOMAL);

	void AddTime(int nAddTime) { m_nTime += nAddTime; }
	bool SecondsCnt(void);

	// セッター
	void SetTime(int nSetTime) { m_nTime = nSetTime; }

	// ゲッター
	int GetTime(void) { return m_nTime; }
	bool GetIsTimeCount(void) { return m_bTimeCount; }
	bool GetIsTimeOut(void) { return m_bTimeOut; }

	static void SwitchIsCount(void) { m_bCount = !m_bCount; }
private:
	CWord* m_pWord;			// 文字
	int m_nTime;			// タイム
	int m_nCntTime;			// タイマーカウント
	bool m_bTimeCount;		// 時間をカウントしたかの有無
	bool m_bTimeOut;		// 時間切れか否か
	static bool m_bCount;	// タイムカウントの有無
};

#endif // !_TIMER_H_
