//**********************************************
// 
// タコ頭状態待機
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _KRAKENSTATE_NEWTRAL_HEAD_H_
#define _KRAKENSTATE_NEWTRAL_HEAD_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 待機クラス
class CStateKrakenNewtralHead : public CStateBase
{
public:
	static constexpr int TIME_MOTIONBLEND = 10;			// モーションをブレンドする時間

	CStateKrakenNewtralHead();
	~CStateKrakenNewtralHead() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};
#endif