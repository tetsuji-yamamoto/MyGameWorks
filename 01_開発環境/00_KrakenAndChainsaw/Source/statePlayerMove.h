//**********************************************
// 
// プレイヤーゲーム時状態移動
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATEGAME_MOVE_H_
#define _PALYERSTATEGAME_MOVE_H_

#include "compStateMachine.h"
#include "stateBase.h"

// 移動クラス
class CStatePllayerMove : public CStateBase
{
public:
	CStatePllayerMove();
	~CStatePllayerMove() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};

#endif