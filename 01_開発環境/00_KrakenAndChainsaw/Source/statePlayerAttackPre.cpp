//**********************************************
// 
// プレイヤーゲーム時状態攻撃
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerAttackPre.h"
#include "statePlayerAttackStop.h"
#include "statePlayerAttackIng.h"
#include "statePlayerNewtral.h"
#include "compMotionSkin.h"
#include "compMovement.h"
#include "compInfo.h"
#include "player.h"
#include "camera.h"
#include "input.h"
#include "vector.h"
#include "debugproc.h"
#include "debugPoint.h"
#include "compWeapon.h"
#include "manager.h"
#include "sound.h"
#include "vector.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerAttackPre::CStatePllayerAttackPre()
{
	// タイプ設定
	SetType(CPlayer::MOTION_ATTACK_PRE);

	m_nCntInput = 0; 
	m_bCollision = false;
}

//**********************************************
// 初期化
//**********************************************
bool CStatePllayerAttackPre::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// キー取得
	CInputKeyboard* pInputKeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	//if (pInputJoyPad->GetRStick() == true)
	//{

	//}
	//else
	//{
	//	int nUp = pInputKeyBoard->GetPressCnt(DIK_UP);
	//	int nDown = pInputKeyBoard->GetPressCnt(DIK_DOWN);
	//	int nLeft = pInputKeyBoard->GetPressCnt(DIK_LEFT);
	//	int nRight = pInputKeyBoard->GetPressCnt(DIK_RIGHT);
	//	int nManCnt = nUp + nDown + nLeft + nRight;

	//	// 使うキーの入力時間の合計が超えてない
	//	if (nManCnt < TIME_INPUT)
	//	{
	//		return false;
	//	}
	//}

	//// 攻撃する向きを調べる
	//D3DXVECTOR3 rotAttack = AttackRotationKey();

	//// 向き設定
	//pOwner->GetComponent<CInfo>()->rotDest.y = rotAttack.y;

	// モーションを攻撃に設定
	pOwner->GetComponent<CMotionSkin>()->SetMotion(CPlayer::MOTION_ATTACK_PRE);

	// チェンソー切る音
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_CHAINSAW_CUT);

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerAttackPre::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();
	CWeapon* pWeapon = pMotionSkin->GetEntity()->GetComponent<CWeapon>("chainsaw");
	
	// コンポーネントが取得できていなければ処理しない
	if (pWeapon == nullptr)return;
	if (pMotionSkin == nullptr)return;

	// モーションが終っていたら待機状態に戻す
	if (pMotionSkin->GetIsMotionEnd() == true)
	{
		// 攻撃中状態に設定
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerAttackIng);
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerAttackPre::Uninit(void)
{

	delete this;
}

//**********************************************
// 攻撃する向きを調べる処理
//**********************************************
D3DXVECTOR3 CStatePllayerAttackPre::AttackRotationKey(void)
{
	// キー取得
	CInputKeyboard* pInputKeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// カメラ取得
	CCamera* pCamera = CCamera::GetCamera();

	// カメラの回転取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// 攻撃する向き
	D3DXVECTOR3 rotAttack = pCamera->GetRotation();

	// 攻撃する向きを調べる
	if (pInputKeyBoard->GetPress(DIK_UP) == true)
	{
		if (pInputKeyBoard->GetPress(DIK_LEFT) == true)
		{
			rotAttack.y += D3DX_PI * -0.25f;
		}
		else if (pInputKeyBoard->GetPress(DIK_RIGHT) == true)
		{
			rotAttack.y += D3DX_PI * 0.25f;
		}
		else
		{

		}
	}
	else if (pInputKeyBoard->GetPress(DIK_DOWN) == true)
	{
		if (pInputKeyBoard->GetPress(DIK_LEFT) == true)
		{
			rotAttack.y += D3DX_PI * -0.75f;
		}
		else if (pInputKeyBoard->GetPress(DIK_RIGHT) == true)
		{
			rotAttack.y += D3DX_PI * 0.75f;
		}
		else
		{
			rotAttack.y += D3DX_PI;
		}
	}
	else if (pInputKeyBoard->GetPress(DIK_LEFT) == true)
	{
		rotAttack.y += D3DX_PI * -0.5f;
	}
	else if (pInputKeyBoard->GetPress(DIK_RIGHT) == true)
	{
		rotAttack.y += D3DX_PI * 0.5f;
	}

	if (pInputJoyPad->GetRStick() == true)
	{
		XINPUT_STATE input = pInputJoyPad->GetXInputState();
		rotAttack.y += my_vector::AngleAtan2f(InitValu::VEC2_NULL, D3DXVECTOR2((float)input.Gamepad.sThumbRX, (float)input.Gamepad.sThumbRY));
	}


	// 角度の正規化
	rotAttack.y += D3DX_PI;
	rotAttack = my_vector::AngleNormalize(rotAttack);

	return rotAttack;
}