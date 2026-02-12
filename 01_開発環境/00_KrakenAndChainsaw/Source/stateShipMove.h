//***************************************************************
// 
// 船移動状態
// Author Tetsuji Yamamoto
// 
//***************************************************************
#ifndef _STATE_SHIP_MOVE_H_
#define _STATE_SHIP_MOVE_H_

#include "stateBase.h"

// 船待機状態クラス
class CStateShipMove : public CStateBase
{
public:
	CStateShipMove();
	~CStateShipMove() {}

	bool Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_posCenter;	// 移動中心位置
};

#endif