//*********************************************************************
// 
// しぶきエフェクト
// Author Tetsuji Yamamoto
// 
//*********************************************************************
#ifndef _EFFECT_SPLASH_H_
#define _EFFECT_SPLASH_H_

#include "object.h"

// 血エフェクトクラス
class CEffectSplash : public CObject
{
public:
	CEffectSplash();
	~CEffectSplash();

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fGravity, int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffectSplash* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fGravity, int nLife);
	static void Particle(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadiusRandm, float fGravity, int nLife, int nAmount);

private:
	float	m_fRadius;	// 半径
};

#endif
