//**********************************************
// 
// プレイヤーゲーム時待機状態待機
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerNewtral.h"
#include "statePlayerMove.h"
#include "statePlayerJump.h"
#include "statePlayerAttackPre.h"
#include "compMotionSkin.h"
#include "player.h"
#include "input.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerNewtral::CStatePllayerNewtral()
{
	// タイプ設定
	SetType(CPlayer::MOTION_NEWTRAL);
}


//**********************************************
// 初期化
//**********************************************
bool CStatePllayerNewtral::Init(void)
{
	// モーションスキンコンポーネント取得
	CMotionSkin * pMotionSkin = GetOwner()->GetComponent<CMotionSkin>();

	// モーションを待機に設定
	if (pMotionSkin != nullptr)
	{
		pMotionSkin->SetMotion(CPlayer::MOTION_NEWTRAL);
	}

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerNewtral::Update(void)
{
	// キー取得
	CInputKeyboard* pInputkeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// 移動するのならば
	if (
		pInputkeyBoard->GetPress(DIK_W) == true ||
		pInputkeyBoard->GetPress(DIK_A) == true ||
		pInputkeyBoard->GetPress(DIK_S) == true ||
		pInputkeyBoard->GetPress(DIK_D) == true ||
		pInputJoyPad->GetLStick() == true
		)
	{
		// 移動状態に設定
		GetOwner()->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerMove);
		return;
	}
	// ジャンプするのならば
	else if (
		pInputkeyBoard->GetPress(DIK_SPACE) == true ||
		pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_A)
		)
	{
		// ジャンプ状態に設定
		GetOwner()->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerJump);
		return;
	}
	// 攻撃するのならば
	else if (
		pInputkeyBoard->GetPress(DIK_F) == true ||
		pInputJoyPad->GetkeyPress(CInputJoypad::JOYKEY_R1) == true
		)
	{
		// 攻撃状態に設定
		GetOwner()->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerAttackPre);
		return;
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerNewtral::Uninit(void)
{
	delete this;
}