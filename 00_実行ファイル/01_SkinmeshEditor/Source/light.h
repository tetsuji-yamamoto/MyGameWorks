//****************************************
// 
// ライト
// Author Tetsuji Yamamoto
// 
//****************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

// ライトクラス
class CLight
{
public:
	static constexpr int NUMCAMERA = 1;	// カメラの数

	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_light;
};

#endif // !_LIGHT_H_
