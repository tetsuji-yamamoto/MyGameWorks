//*********************************************************************
// 
// 血エフェクト
// Author Tetsuji Yamamoto
// 
//*********************************************************************
#ifndef _EFFECT_BLOOD_H_
#define _EFFECT_BLOOD_H_

#include "object.h"

// 血エフェクトクラス
class CEffectBlood : public CObject
{
public:
	CEffectBlood();
	~CEffectBlood();

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fGravity ,int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffectBlood* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fGravity, int nLife);
	static void Particle(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadiusRandm, float fGravity, int nLife, int nAmount);

private:
	D3DXVECTOR3 m_move;	// 移動量
};

#endif
