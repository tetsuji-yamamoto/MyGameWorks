//****************************************************************************
// 
// îwåiÉ}ÉlÅ[ÉWÉÉÅ[
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef _BGMANAGER_H_
#define _BGMANAGER_H_

#include "main.h"
#include "object2D.h"
#include "bg.h"

#define MAX_BG_NUM (3)

// îwåiÉNÉâÉX
class CBGManager : public CObject // CObjectåpè≥
{
public:

	CBGManager();
	~CBGManager();

	HRESULT Init() { return S_OK; }
	HRESULT Init(int nBGNum, float fFeedRate, float fFeedAngle);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBGManager* Create(int nBGNum, float fFeedRate, float fFeedAngle);

private:
	CBG* m_apBG[MAX_BG_NUM];
};

#endif