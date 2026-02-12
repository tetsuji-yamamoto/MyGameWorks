//********************************************************
// 
// ステートベースコンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compStateMachine.h"
#include "stateBase.h"

//**********************************************
// 状態管理コンストラクタ
//**********************************************
CStateMachine::CStateMachine()
{
	m_pStateBase = nullptr;	// 状態管理基底クラスポインター
}

//**********************************************
// 状態管理デストラクタ
//**********************************************
CStateMachine::~CStateMachine()
{
}

//**********************************************
// 状態管理変更
//**********************************************
void CStateMachine::ChangeState(CStateBase* pStateBase)
{
	// タイプの取得
	int nTypeNew = pStateBase->GetType();
	int nTypeNow;

	if (m_pStateBase != nullptr) { nTypeNow = m_pStateBase->GetType(); }
	else { nTypeNow = -1; }

	// 同じステートであれば処理しない
	if (nTypeNew == nTypeNow) 
	{
		return;
	}

	// オブジェクトと自身を設定
	pStateBase->SetOwner		(GetOwner());
	pStateBase->SetStateMachine	(this);

	// 新しいステートを初期化出来なかったらステートを変えない
	if (pStateBase->Init() == false)
	{
		pStateBase->Uninit();
		pStateBase = nullptr;
		return;
	}

	// 状態があれば破棄する
	if (m_pStateBase != nullptr)
	{
		m_pStateBase->Uninit();
		m_pStateBase = nullptr;
	}

	// 新しいステートをセットする
	m_pStateBase = pStateBase;
}

//**********************************************
// 状態管理初期化
//**********************************************
void CStateMachine::Init(void)
{

}

//**********************************************
// 状態管理終了
//**********************************************
void CStateMachine::Uninit(void)
{
	// 状態管理基底クラス破棄
	if (m_pStateBase != nullptr)
	{
		m_pStateBase->Uninit();
		m_pStateBase = nullptr;
	}

	// 自身の破棄
	delete this;
}

//**********************************************
// 状態管理更新
//**********************************************
void CStateMachine::Update(void)
{
	// 状態があれば更新する
	if (m_pStateBase != nullptr)
	{
		m_pStateBase->Update();
	}
}

//**********************************************
// 状態管理描画
//**********************************************
void CStateMachine::Draw(void)
{
	// 状態管理基底クラス描画
	if (m_pStateBase != nullptr)
	{
		m_pStateBase->Draw();
	}
}