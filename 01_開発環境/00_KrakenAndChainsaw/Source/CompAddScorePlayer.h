//***************************************************************
// 
// プレイヤースコア加算処理
// Author Tetesuji Yamamoto
// 
//***************************************************************
#ifndef _COMPONENT_ADDSCORE_PLAYER_H_
#define _COMPONENT_ADDSCORE_PLAYER_H_

#include "component.h"

// プレイヤースコア加算処理クラス
class CCompAddScorePlayer : public CComponent
{
public:
	CCompAddScorePlayer();
	~CCompAddScorePlayer();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif