//********************************************************
// 
// 移動コンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compMovement.h"
#include "compCollision.h"
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "manager.h"
#include "vector.h"
#include "matrix.h"

using namespace matrix;
using namespace my_vector;

//********************************************************
// 移動コンポーネント
//********************************************************
CMovement::CMovement()
{
}

//********************************************************
// 移動コンポーネント
//********************************************************
CMovement::~CMovement()
{

}

//********************************************************
// 移動コンポーネント
//********************************************************
void CMovement::Uninit()
{
	delete this;
}

//********************************************************
// 移動コンポーネント
//********************************************************
void CMovement::Update()
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	// 最短の角度差を求める
	float fAngleDiff = AngleShortestDiff(pInfo->rotDest.y,pInfo->rot.y);

	// 角度を足す
	pInfo->rot.y += fAngleDiff * 0.2f;
	
	// 角度の正規化
	pInfo->rot.y = AngleNormalize(pInfo->rot.y);

	// 古い位置の更新
	pInfo->posOld = pInfo->pos;

	// 重力追加
	pInfo->move.y += pInfo->fGravity;

	// 移動の摩擦計算
	pInfo->move.x += (-pInfo->move.x * pInfo->fFriction);
	pInfo->move.z += (-pInfo->move.z * pInfo->fFriction);

	// 移動量追加
	pInfo->pos += pInfo->move;

	// 位置が限界程度まで達したら上空へ戻す
	if (pInfo->pos.y <= -15000.0f)
	{
		pInfo->pos.y = 2000.0f;
		pInfo->posOld = pInfo->pos;

		// 縦移動量リセット
		pInfo->move.y = 0.0f;
	}

	// ワールドマトリックスの作成
	pInfo->mtxWorld = MatrixWorld(pInfo->pos, pInfo->rot);

	// 当たり判定用位置設定
	pInfo->hitResult.m_posHit = pInfo->pos;

	// 当たり判定用移動量設定
	pInfo->hitResult.m_moveHit = pInfo->move;
}

//********************************************************
// 移動コンポーネント
//********************************************************
void CMovement::Draw()
{

}

//********************************************************
// 移動コンポーネントキー操作
//********************************************************
bool CMovement::MoveKeyboard(void)
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return false;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// カメラの回転取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();
	D3DXVECTOR3 rotMove = rotCamera;

	// 動くか否か
	bool bMove = false;

	// 奥移動
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		// 左移動
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * -0.25f;
		}
		// 右移動
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * 0.25f;
		}
		else
		{

		}

		// 移動の有無
		bMove = true;
	}
	// 手前移動
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		// 左移動
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * -0.75f;
		}
		// 右移動
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * 0.75f;
		}
		else
		{
			// 移動量追加
			rotMove.y += D3DX_PI;
		}

		// 移動の有無
		bMove = true;
	}
	// 左移動
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		// 移動量追加
		rotMove.y += D3DX_PI * -0.5f;

		// 移動の有無
		bMove = true;
	}
	// 右移動
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		// 移動量追加
		rotMove.y += D3DX_PI * 0.5f;

		// 移動の有無
		bMove = true;
	}

	// ジョイスティック操作
	if (pInputJoyPad->GetLStick() == true)
	{
		XINPUT_STATE input = pInputJoyPad->GetXInputState();
		rotMove.y += my_vector::AngleAtan2f(InitValu::VEC2_NULL, D3DXVECTOR2((float)input.Gamepad.sThumbLX, (float)input.Gamepad.sThumbLY));
		bMove = true;
	}

	// 動くのであれば移動量追加
	if (bMove == true)
	{
		// 移動量計算
		pInfo->move.x += sinf(rotMove.y) * (pInfo->fMovement);
		pInfo->move.z += cosf(rotMove.y) * (pInfo->fMovement);

		// 目的の移動方向の設定
		pInfo->rotDest.y = rotMove.y + D3DX_PI;

		// 角度の正規化
		pInfo->rotDest.y = AngleNormalize(pInfo->rotDest.y);
	}

	return bMove;
}

//********************************************************
// 移動コンポーネントキー操作ジャンプ中
//********************************************************
bool CMovement::MoveKeyboardJumping(void)
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return false;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// カメラの回転取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();
	D3DXVECTOR3 rotMove = rotCamera;

	// 動くか否か
	bool bMove = false;

	// 奥移動
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		// 左移動
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * -0.25f;
		}
		// 右移動
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * 0.25f;
		}
		else
		{

		}

		// 移動の有無
		bMove = true;
	}
	// 手前移動
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		// 左移動
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * -0.75f;
		}
		// 右移動
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * 0.75f;
		}
		else
		{
			// 移動量追加
			rotMove.y += D3DX_PI;
		}

		// 移動の有無
		bMove = true;
	}
	// 左移動
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		// 移動量追加
		rotMove.y += D3DX_PI * -0.5f;

		// 移動の有無
		bMove = true;
	}
	// 右移動
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		// 移動量追加
		rotMove.y += D3DX_PI * 0.5f;

		// 移動の有無
		bMove = true;
	}

	// ジョイスティック操作
	if (pInputJoyPad->GetLStick() == true)
	{
		XINPUT_STATE input = pInputJoyPad->GetXInputState();
		rotMove.y += my_vector::AngleAtan2f(InitValu::VEC2_NULL, D3DXVECTOR2((float)input.Gamepad.sThumbLX, (float)input.Gamepad.sThumbLY));
		bMove = true;
	}

	// 動くのであれば移動量追加
	if (bMove == true)
	{
		// 移動向きの正規化
		rotMove = AngleNormalize(rotMove);

		// 移動計算向き
		float fRotMoveY = rotMove.y + D3DX_PI;
		fRotMoveY = AngleNormalize(fRotMoveY);

		float fDiffRot = fabsf(fRotMoveY - pInfo->rot.y);
		fDiffRot = AngleNormalize(fDiffRot);

		float fPalam = 1.0f - fabsf(fDiffRot / D3DX_PI);

		// 移動量計算
		pInfo->move.x += sinf(rotMove.y) * (pInfo->fMovement * fPalam);
		pInfo->move.z += cosf(rotMove.y) * (pInfo->fMovement * fPalam);
	}

	return bMove;
}

//********************************************************
// 移動コンポーネントジャンプキー操作
//********************************************************
bool CMovement::Jump(void)
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return false;

	// 乗っている場合のみ処理
	if (pInfo->hitResult.m_bRanding == false)return false;

	// 乗っていない状態にする
	pInfo->hitResult.m_bRanding = false;

	// ジャンプ量追加
	pInfo->move.y += 20.0f;

	return true;
}

//********************************************************
// 移動コンポーネント情報修正
//********************************************************
void CMovement::Correction(void)
{
	
}