//********************************************************
// 
// 当たり判定前コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_HITBEFORE_H_
#define _COMPONENT_HITBEFORE_H_

#include "component.h"

// 移動コンポーネントクラス
class CHitBefore : public CComponent
{
public:

	CHitBefore();
	~CHitBefore();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// ゲッター

	// セッター
private:
};
#endif