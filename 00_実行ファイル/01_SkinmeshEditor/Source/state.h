//**********************************************
// 
// 状態処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _STATE_H_
#define _STATE_H_

#include "main.h"
#include "object.h"

#define STATE_COUNT_NOMAL (60)

// オブジェクト2Dクラス
class CState
{
public:

	// 状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_APPEAR,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	CState();
	~CState();

	void Init(STATE state = STATE_APPEAR, int nAppStateCnt = STATE_COUNT_NOMAL);
	void Uninit();
	void StateCountDown(void);

	// セッター
	void SetState(STATE state) { m_state = state; }
	void SetStateCnt(int nLimit) { m_nStateCnt = nLimit; }

	// ゲッター
	int GetStateCnt(void) { return m_nStateCnt; }
	int GetState(void) { return (int)m_state; }

	static CState* Create(STATE state = STATE_APPEAR, int nAppStateCnt = STATE_COUNT_NOMAL);

private:
	int m_nStateCnt;					// ステートカウンター
	STATE m_state;						// 状態
};

#endif
