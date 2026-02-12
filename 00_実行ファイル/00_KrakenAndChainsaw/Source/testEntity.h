//*****************************************************
// 
// テスエンティティ
// Author Tetesuji yamamot
// 
//*****************************************************
#ifndef _TESTENTITY_H_
#define _TESTENTITY_H_

#include "object.h"

// テストエンティティクラス
class CTestEntity : public CObject
{
public:
	CTestEntity();
	~CTestEntity();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif
