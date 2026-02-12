//********************************************************
// 
// ステートベース
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _STATEBASE_H_
#define _STATEBASE_H_

#include "object.h"

// 前方宣言
class CStateMachine;

// 状態管理基底クラス
class CStateBase
{
public:
	CStateBase()
	{
		m_pOwner = nullptr;	// オーナー
		m_nType = -1;		// タイプ
	}

	~CStateBase() {}

	virtual bool	Init			(void)							= 0;
	virtual void	Update			(void)							= 0;
	virtual void	Uninit			(void)							= 0;
	virtual void	Draw			(void)							= 0;
	void			SetOwner		(CObject* pOwner)				{ m_pOwner = pOwner; }
	void			SetStateMachine	(CStateMachine* pStateMachine)	{ m_pStateMachineOwner = pStateMachine;}
	void			SetType			(int nType)						{ m_nType = nType; }

	CObject*		GetOwner		(void)	{ return m_pOwner; }
	CStateMachine*	GetStateMachine	(void)	{ return m_pStateMachineOwner;}
	int				GetType			(void)	{ return m_nType; }

private:
	CObject*		m_pOwner;				// オーナー
	CStateMachine*	m_pStateMachineOwner;	// ステートマシンオーナー
	int				m_nType;				// タイプ
};
#endif