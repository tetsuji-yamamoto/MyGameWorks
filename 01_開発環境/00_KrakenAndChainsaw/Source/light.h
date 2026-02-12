//****************************************
// 
// ライト
// Author Tetsuji Yamamoto
// 
//****************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

#define MAX_LIGHT (3)	// ライトの数

// ライトクラス
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	D3DLIGHT9* GetLight(int nIdx);
private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];
};

#endif // !_LIGHT_H_
