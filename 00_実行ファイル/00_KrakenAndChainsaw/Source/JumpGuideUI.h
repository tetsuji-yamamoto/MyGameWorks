//***********************************************
// 
// ƒWƒƒƒ“ƒvˆÄ“àUI
// Author Tetsuji Yamamoto
// 
//***********************************************
#ifndef _JUMP_GUIDE_H_
#define _JUMP_GUIDE_H_

#include "object.h"

// ˆÚ“®ˆÄ“àUI
class CJumpGuide : public CObject
{
public:
	static constexpr float	HEIGHT = 30.0f;
	static constexpr float	WIDTH = 90.0f;
	static constexpr float	POS_X = SCREEN_WIDTH * 0.5f;
	static constexpr float	POS_Y = SCREEN_HEIGHT * 0.2f;
	static constexpr float	POS_Z = 0.0f;
	static constexpr int	COUNT = 120;

	CJumpGuide();
	~CJumpGuide();

	HRESULT	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CJumpGuide* Create(void);

private:
	bool m_bNext;	// ŽŸ‚©”Û‚©
};

#endif