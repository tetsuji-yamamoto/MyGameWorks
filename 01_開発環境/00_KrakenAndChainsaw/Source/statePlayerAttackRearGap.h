//**********************************************
// 
// プレイヤーゲーム時攻撃後隙状態
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATE_ATTACK_REAR_GAP_H_
#define _PALYERSTATE_ATTACK_REAR_GAP_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 待機クラス
class CStatePllayerAttackRearGap : public CStateBase
{
public:
	static constexpr int TIME_INPUT = 3;	// 入力受付時間

	CStatePllayerAttackRearGap();
	~CStatePllayerAttackRearGap() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};

#endif