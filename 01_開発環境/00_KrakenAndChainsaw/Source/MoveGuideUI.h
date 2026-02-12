//***********************************************
// 
// ˆÚ“®ˆÄ“àUI
// Author Tetsuji Yamamoto
// 
//***********************************************
#ifndef _MOVE_GUIDE_H_
#define _MOVE_GUIDE_H_

#include "object.h"

// ˆÚ“®ˆÄ“àUI
class CMoveGuide : public CObject
{
public:
	static constexpr float	HEIGHT	= 30.0f;
	static constexpr float	WIDTH	= 90.0f;
	static constexpr float	POS_X	= SCREEN_WIDTH * 0.5f;
	static constexpr float	POS_Y	= SCREEN_HEIGHT * 0.2f;
	static constexpr float	POS_Z	= 0.0f;
	static constexpr int	COUNT	= 120;

	CMoveGuide();
	~CMoveGuide();

	HRESULT	Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	static CMoveGuide* Create(void);

private:
	bool m_bNext;	// ŽŸ‚©”Û‚©
};

#endif