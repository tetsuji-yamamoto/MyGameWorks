//*****************************************************
// 
// パーティクル
// Author Tetsuji Yamamoto
// 
//*****************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "object.h"
#include "effect.h"

#define MAX_PARTICLE_AMOUNT (100)

// パーティクルクラス
class CParticle : CObject	// CObject継承
{
public:

	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fSpeed,int nAmount);

private:
};

#endif // !_PARTICLE_H_
