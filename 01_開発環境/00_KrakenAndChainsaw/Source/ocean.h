//*********************************************************************
// 
// 海
// Author Tetsuji Yamamoto
// 
//*********************************************************************
#ifndef _OCEAN_H_
#define _OCEAN_H_

#include "object.h"

// 海クラス
class COcean : public CObject
{
public:
	static constexpr float WIDTH = 50000.0f;	// 幅
	static constexpr float DIPS = 50000.0f;		// 奥行き
	static constexpr float ALPHA = 0.99f;		// 透明度

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static COcean* Create(void);

private:
};

#endif
