//********************************************************
// 
// クラーケン感知コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_KRAKEN_SENSING_H_
#define _COMPONENT_KRAKEN_SENSING_H_

#include "component.h"

// コンポーネントクラス
class CKrakenSensing : public CComponent
{
public:

	CKrakenSensing();
	~CKrakenSensing();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}

	bool GetIsAttaked(const float fHeight, const float fAngle, const float fRange);
	bool GetIsRangePosition(const D3DXVECTOR3 pos,const float fHeight, const float fAngle, const float fRange);

private:
};
#endif