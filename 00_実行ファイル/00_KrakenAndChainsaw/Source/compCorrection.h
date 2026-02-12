//********************************************************
// 
// 修正コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_CORRECTION_H_
#define _COMPONENT_CORRECTION_H_

#include "component.h"

// 前方宣言
class CMovement;

// 移動コンポーネントクラス
class CCorrection : public CComponent
{
public:

	CCorrection();
	~CCorrection();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// ゲッター

	// セッター
private:
	CMovement* m_pMovement;		// 移動処理コンポーネント
};
#endif