//**********************************************
// 
// プレイヤーゲーム時攻撃ストップ状態
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATE_ATTACK_STOP_H_
#define _PALYERSTATE_ATTACK_STOP_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 前方宣言
class CCollisionShapeSpherTrigger;

// 攻撃ストップクラス
class CStatePllayerAttackStop : public CStateBase
{
public:
	static constexpr int TIME_ATTACKLIMIT = 30;	// 攻撃限界時間

	CStatePllayerAttackStop();
	~CStatePllayerAttackStop() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}

	bool SeekCollision(void);

private:
	int											m_nCntAttack;				// 攻撃カウンター
	std::vector < CCollisionShapeSpherTrigger*>	m_vCompCollSpherTrigger;	// 当たり判定球配列
	CObject*									m_pHitedOwner;				// 当たったオーナー
};

#endif