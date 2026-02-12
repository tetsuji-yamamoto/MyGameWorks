//**********************************************
// 
// プレイヤー攻撃中状態
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATE_ATTACK_ING_H_
#define _PALYERSTATE_ATTACK_ING_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 前方宣言
class CCollisionShapeSpherTrigger;

// 攻撃中クラス
class CStatePllayerAttackIng : public CStateBase
{
public:
	static constexpr int TIME_INPUT = 3;	// 入力受付時間

	CStatePllayerAttackIng();
	~CStatePllayerAttackIng() {}

	bool	Init	(void);
	void	Update	(void);
	void	Uninit	(void);
	void	Draw	(void) {}

	bool SeekCollision(void);

private:
	int											m_nCntAttack;				// 攻撃カウンター
	int											m_nCntInput;				// 入力カウンター
	std::vector < CCollisionShapeSpherTrigger*>	m_vCompCollSpherTrigger;	// 当たり判定球配列
	CObject*									m_pHitedOwner;				// 当たったオーナー
};

#endif