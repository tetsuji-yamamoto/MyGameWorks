//**********************************************
// 
// プレイヤーゲーム時状態待機
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATE_NEWtRAL_H_
#define _PALYERSTATE_NEWtRAL_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 待機クラス
class CStatePllayerNewtral : public CStateBase
{
public:
	CStatePllayerNewtral();
	~CStatePllayerNewtral() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};
#endif