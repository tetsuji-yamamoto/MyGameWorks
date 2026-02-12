//****************************************************************************
// 
// ”wŒi
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "object2D.h"

// ”wŒiƒNƒ‰ƒX
class CBG : public CObject2D // CObject2DŒp³
{
public:

	// ”wŒiƒ^ƒCƒv
	typedef enum
	{
		TYPE_00 = 0,
		TYPE_01,
		TYPE_02,
		TYPE_MAX
	}TYPE;

	CBG(int nPriority = PRIORITY_NOMAL);
	~CBG();

	HRESULT Init(float fFeedRate, float fFeedAngle, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBG* Create(float fFeedRate, float fFeedAngle, TYPE type);

private:
	TYPE m_type;		// í—Ş
	float m_fFeedAngle;	// ‘—‚èŠp“x
	float m_fFeedRate;	// ‘—‚è‘¬“x
	float m_fAddTexX;	// ‘—‚è•
	float m_fAddTexY;	// ‘—‚è‚‚³
};

#endif