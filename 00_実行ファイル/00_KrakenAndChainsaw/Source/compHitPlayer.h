//********************************************************
// 
// プレイヤーヒット処理コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_PLAYER_HIT_H_
#define _COMPONENT_PLAYER_HIT_H_

#include "component.h"

// コンポーネントクラス
class CPlayerHit : public CComponent
{
public:
	CPlayerHit();
	~CPlayerHit();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}

private:
};
#endif