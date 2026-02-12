//***************************************************************
// 
// 船待機状態
// Author Tetsuji Yamamoto
// 
//***************************************************************
#ifndef _STATE_SHIP_NEWTRAL_H_
#define _STATE_SHIP_NEWTRAL_H_

#include "stateBase.h"

// 船待機状態クラス
class CStateShipNewtral : public CStateBase
{
public:
	CStateShipNewtral();
	~CStateShipNewtral() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);

private:

};

#endif