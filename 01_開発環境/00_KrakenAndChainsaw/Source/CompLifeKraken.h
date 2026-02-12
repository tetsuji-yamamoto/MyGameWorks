//***************************************************************
// 
// クラーケン体力処理
// Author Tetesuji Yamamoto
// 
//***************************************************************
#ifndef _COMPONENT_LIFE_KRAKEN_H_
#define _COMPONENT_LIFE_KRAKEN_H_

#include "component.h"

// クラーケン体力処理クラス
class CCompLifeKraken : public CComponent
{
public:
static constexpr int PALAM_DAMAGE_TENTACLE = 2;

	CCompLifeKraken();
	~CCompLifeKraken();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif