//**********************************************
// 
// プレイヤーゲーム時状態
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATE_ATTACK_PRE_H_
#define _PALYERSTATE_ATTACK_PRE_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 待機クラス
class CStatePllayerAttackPre : public CStateBase
{
public:
	static constexpr int TIME_INPUT = 3;	// 入力受付時間

	CStatePllayerAttackPre();
	~CStatePllayerAttackPre() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}

	D3DXVECTOR3 AttackRotationKey(void);
private:
	int m_nCntInput;	// 入力カウンター
	bool m_bCollision;	// 当たっているか否か
};

#endif