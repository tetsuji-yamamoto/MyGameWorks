//**********************************************
// 
// プレイヤーゲーム時状態ジャンプ
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerJump.h"
#include "statePlayerJump.h"
#include "statePlayerNewtral.h"
#include "compMotionSkin.h"
#include "compMovement.h"
#include "compInfo.h"
#include "player.h"
#include "input.h"
#include "camera.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerJump::CStatePllayerJump()
{
	// タイプ設定
	SetType(CPlayer::MOTION_JUMP);
}

//**********************************************
// 初期化
//**********************************************
bool CStatePllayerJump::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// ジャンプできなかったら初期化しない
	if (pOwner->GetComponent<CMovement>()->Jump() == false)
	{
		return false;
	}

	// モーションを待機に設定
	pOwner->GetComponent<CMotionSkin>()->SetMotion(CPlayer::MOTION_JUMP);

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerJump::Update(void)
{
	// キー取得
	CInputKeyboard* pInputKeyBoard = CInputKeyboard::GetkeyBoard();

	// オーナー取得
	CObject* pOwner = GetOwner();

	// ジャンプ中の移動処理
	pOwner->GetComponent<CMovement>()->MoveKeyboardJumping();

	// 着地していれば待機状態にする
	if (pOwner->GetComponent<CInfo>()->hitResult.m_bRanding == true)
	{
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerNewtral);
		return;
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerJump::Uninit(void)
{
	delete this;
}