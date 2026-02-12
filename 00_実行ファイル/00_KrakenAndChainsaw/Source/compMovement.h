//********************************************************
// 
// 当たり判定基底コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_MOVEMENT_H_
#define _COMPONENT_MOVEMENT_H_

#include "component.h"
#include "compCollision.h"
#include "compCollision.h"
#include "hitResult.h"
#include "compInfo.h"

// 移動コンポーネントクラス
class CMovement : public CComponent
{
public:

	CMovement();
	~CMovement();

	void Init() {}
	void Uninit();
	void Update();
	void Draw();

	void Correction(void);
	bool MoveKeyboard(void);
	bool MoveKeyboardJumping(void);
	bool Jump(void);

	// ゲッター

	// セッター
private:
};
#endif