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
#include "number.h"

#define TIME_NOMAL (120)
#define MAX_TIME_DIGIT (3)

// タイマークラス
class CTimer : public CObject // CObject継承
{
public:
	CTimer();
	~CTimer();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nDigits, int nTime = TIME_NOMAL);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nDigits,int nTime = TIME_NOMAL);
	static void AddTime(int nAddTime) { m_nTime += nAddTime; }
	static bool SecondsCnt(void);

	// セッター
	static void SetTime(int nSetTime) { m_nTime = nSetTime; }

	// ゲッター
	static int GetTime(void) { return m_nTime; }
private:
	CNumber* m_apNumber[MAX_TIME_DIGIT];	// 数字オブジェクトのポインタ
	int m_nDigit;			// 桁数
	static int m_nTime;		// タイム
	static int m_nCntTime;	// タイマーカウント
};

#endif // !_TIMER_H_
