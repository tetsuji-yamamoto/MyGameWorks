//***********************************************
// 
// çUåÇàƒì‡UI
// Author Tetsuji Yamamoto
// 
//***********************************************
#ifndef _ATTACK_GUIDE_H_
#define _ATTACK_GUIDE_H_

#include "object.h"

// à⁄ìÆàƒì‡UI
class CAttackGuide : public CObject
{
public:
	static constexpr float	HEIGHT = 30.0f;
	static constexpr float	WIDTH = 90.0f;
	static constexpr float	POS_X = SCREEN_WIDTH * 0.5f;
	static constexpr float	POS_Y = SCREEN_HEIGHT * 0.2f;
	static constexpr float	POS_Z = 0.0f;
	static constexpr int	COUNT = 120;

	CAttackGuide();
	~CAttackGuide();

	HRESULT	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CAttackGuide* Create(void);

private:
	bool m_bNext;	// éüÇ©î€Ç©
};

#endif