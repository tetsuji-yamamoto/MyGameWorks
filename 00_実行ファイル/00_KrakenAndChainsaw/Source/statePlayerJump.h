//**********************************************
// 
// プレイヤーゲーム時状態ジャンプ
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATEGAME_JUMP_H_
#define _PALYERSTATEGAME_JUMP_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 移動クラス
class CStatePllayerJump : public CStateBase
{
public:
	CStatePllayerJump();
	~CStatePllayerJump() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};

#endif