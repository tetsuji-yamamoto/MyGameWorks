//**********************************************
// 
// プレイヤーゲーム時状態移動
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerMove.h"
#include "statePlayerNewtral.h"
#include "statePlayerJump.h"
#include "statePlayerAttackPre.h"
#include "compMotionSkin.h"
#include "compMovement.h"
#include "player.h"
#include "input.h"
#include "camera.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerMove::CStatePllayerMove()
{
	// タイプ設定
	SetType(CPlayer::MOTION_MOVE);
}

//**********************************************
// 初期化
//**********************************************
bool CStatePllayerMove::Init(void)
{
	// モーションスキンコンポーネント取得
	CMotionSkin* pMotionSkin = GetOwner()->GetComponent<CMotionSkin>();

	// モーションを待機に設定
	if (pMotionSkin != nullptr)
	{
		pMotionSkin->SetMotion(CPlayer::MOTION_MOVE);
	}

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerMove::Update(void)
{
	// キー取得
	CInputKeyboard* pInputKeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// オーナー取得
	CObject* pOwner = GetOwner();

	// 行動の有無
	bool bAction = false;

	// キー操作
	if (pOwner->GetComponent<CMovement>()->MoveKeyboard() == true) { bAction = true; }

	// ジャンプするのならば
	if (pInputKeyBoard->GetPress(DIK_SPACE) == true ||
		pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		bAction = true;

		// 移動状態に設定
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerJump);
		return;
	}
	// 攻撃するのならば
	else if (pInputKeyBoard->GetPress(DIK_F) == true ||
		pInputJoyPad->GetkeyPress(CInputJoypad::JOYKEY_R1) == true)
	{
		// 移動状態に設定
		GetOwner()->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerAttackPre);
		return;
	}

	// 行動しなければ待機状態にする
	if (bAction == false)
	{
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerNewtral);
		return;
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerMove::Uninit(void)
{
	delete this;
}