//**********************************************
// 
// プレイヤー状態吹き飛ばし
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATEGAME_JUMP_H_
#define _PALYERSTATEGAME_JUMP_H_

#include "stateBase.h"

// 吹き飛ばしクラス
class CStatePlayerBlowoff : public CStateBase
{
public:
static constexpr float BLOWOFF_ADDMOVE_Y = 20.0f;

	CStatePlayerBlowoff();
	~CStatePlayerBlowoff();

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};

#endif