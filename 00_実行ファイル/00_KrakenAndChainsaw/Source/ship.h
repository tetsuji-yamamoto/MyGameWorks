//**************************************************
// 
// 船
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _SHIP_H_
#define _SHIP_H_

#include "object.h"
#include "myMath.h"
#include "compInfo.h"

// 船クラス
class CShip : public CObject
{
public:
	CShip();
	~CShip();

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CShip* Create(D3DXVECTOR3 pos);

private:
};

#endif