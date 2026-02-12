//********************************************************
// 
// ステートマシーンコンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

#include "component.h"

// 前方宣言
class CStateBase;

// 状態管理クラス
class CStateMachine : public CComponent
{
public:
	CStateMachine();
	~CStateMachine();

	void ChangeState(CStateBase* pStateBase);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CStateBase* m_pStateBase;	// 状態管理の基底クラスポインター
};
#endif