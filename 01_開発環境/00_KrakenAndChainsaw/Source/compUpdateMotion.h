//********************************************************
// 
// モーションの更新コンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_UPDATEMOTION_H_
#define _COMPONENT_UPDATEMOTION_H_

#include "component.h"

// 状態管理クラス
class CUpdateMotion : public CComponent
{
public:
	CUpdateMotion();
	~CUpdateMotion();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif