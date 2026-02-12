//****************************************************************************
// 
// 状態
// Author tetuji yamamoto
// 
//****************************************************************************
#include "state.h"
#include "object.h"

//****************************************************************************
// 状態
//****************************************************************************
CState::CState()
{
	m_state = CState::STATE_NONE;
	m_nStateCnt = 0;
}

//****************************************************************************
// 状態
//****************************************************************************
CState::~CState()
{

}

//****************************************************************************
// 状態
//****************************************************************************
void CState::Init(STATE state, int nAppStateCnt)
{
	m_state = state;
	m_nStateCnt = nAppStateCnt;
}

//****************************************************************************
// 状態
//****************************************************************************
void CState::Uninit()
{
	delete this;
}

//****************************************************************************
// 状態
//****************************************************************************
void CState::StateCountDown(void)
{
	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_APPEAR:
		m_nStateCnt--;
		if (m_nStateCnt <= 0)
		{// カウントがゼロになったら
			m_state = STATE_NONE;
		}
		break;

	case STATE_DAMAGE:
		m_nStateCnt--;
		if (m_nStateCnt <= 0)
		{
			m_state = STATE_NONE;	// 通常状態に設定
		}
		break;

	case STATE_DEATH:
		break;
	}
}

//****************************************************************************
// 状態
//****************************************************************************
CState* CState::Create(STATE state, int nAppStateCnt)
{
	CState* pState = NULL;
	int nNumAll = CObject::GetObujectNumAll();

	// オブジェクト2Dの生成
	if (pState == NULL)
	{
		pState = new CState;

		if (pState != NULL)
		{
			// 初期化処理
			pState->Init(state, nAppStateCnt);
		}
		else
		{
			MessageBox(NULL, " CEnemy* CEnemy::Create() > if ( pEnemy != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CEnemy* CEnemy::Create() > if ( pEnemy == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return pState;
}