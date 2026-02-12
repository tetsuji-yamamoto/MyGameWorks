//***************************************************************
// 
// プレイヤー最高倍率計算処理
// Author Tetesuji Yamamoto
// 
//***************************************************************
#ifndef _COMPONENT_PALAM_MAXIMUM_PLAYER_H_
#define _COMPONENT_PALAM_MAXIMUM_PLAYER_H_

#include "component.h"

// プレイヤースコア加算処理クラス
class CCompPalamMaximumPlayer : public CComponent
{
public:
	CCompPalamMaximumPlayer();
	~CCompPalamMaximumPlayer();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif