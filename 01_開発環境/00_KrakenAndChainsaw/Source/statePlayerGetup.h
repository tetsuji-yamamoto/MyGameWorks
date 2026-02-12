//**********************************************
// 
// プレイヤー状態起き上がり
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PALYERSTATEGAME_GETUP_H_
#define _PALYERSTATEGAME_GETUP_H_

#include "stateBase.h"

// 起き上がりクラス
class CStatePlayerGetup : public CStateBase
{
public:
	CStatePlayerGetup();
	~CStatePlayerGetup();

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
};

#endif