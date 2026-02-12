//**********************************************
// 
// プレイヤーゲーム時状態攻撃
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerAttackStop.h"
#include "statePlayerAttackRearGap.h"
#include "statePlayerNewtral.h"
#include "compMotionSkin.h"
#include "compMovement.h"
#include "player.h"
#include "camera.h"
#include "input.h"
#include "vector.h"
#include "debugproc.h"
#include "debugPoint.h"
#include "compWeapon.h"
#include "compDamage.h"
#include "effectBlood.h"
#include "compInfo.h"

using namespace my_vector;

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerAttackStop::CStatePllayerAttackStop()
{
	// タイプ設定
	SetType(CPlayer::MOTION_STOP);

	m_nCntAttack = 0;
	m_pHitedOwner = nullptr;	// 当たったオーナー
}

//**********************************************
// 初期化
//**********************************************
bool CStatePllayerAttackStop::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// モーションが無ければ処理しない
	if (pMotionSkin == nullptr)return false;

	// モーションを止める
	pMotionSkin->SetIsStopMotion(true);

	// 武器に当たり判定生成とベクターへプッシュバック
	CWeapon* pWeapon = pOwner->GetComponent<CMotionSkin>()->GetEntity()->GetComponent<CWeapon>("chainsaw");
	if (pWeapon != nullptr)
	{
		std::string name = "AttackChainsaw";
		D3DXMATRIX* pMtx = pWeapon->GetPointerMatrix();

		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION, name, pMtx, D3DXVECTOR3(0.0f, 0.0f, -30.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION, name, pMtx, D3DXVECTOR3(0.0f, 0.0f, -50.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION, name, pMtx, D3DXVECTOR3(0.0f, 0.0f, -70.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION, name, pMtx, D3DXVECTOR3(0.0f, 0.0f, -90.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION, name, pMtx, D3DXVECTOR3(0.0f, 0.0f, -110.0f), 10.0f));
	}

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerAttackStop::Update(void)
{
	// キー取得
	CInputKeyboard* pInputkeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// 当たったオーナーなし
	m_pHitedOwner = nullptr;

	// コンポーネントが取得できていなければ処理しない
	if (pMotionSkin == nullptr)return;

	// 攻撃していない
	if (
		pInputkeyBoard->GetPress(DIK_F) == false &&
		pInputJoyPad->GetkeyPress(CInputJoypad::JOYKEY_R1) == false
		)
	{
		// あとすき状態に設定
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerAttackRearGap);
		return;
	}

	// タコに当たっている
	if (SeekCollision() == true)
	{
		// 当たり判定球更新
		for (auto& iterCollSpherTrigger : m_vCompCollSpherTrigger)
		{
			// 当たっていない、次を調べる
			if (iterCollSpherTrigger->m_bCollision == false) continue;

			// 当たっているものの名前を調べる
			for (auto& iterName : iterCollSpherTrigger->GetHitedName())
			{
				// タコに当たっている
				if (iterName == "KrakenBody")
				{
					// 武器のマトリックス取得
					CWeapon* pWeapon = pMotionSkin->GetEntity()->GetComponent<CWeapon>("chainsaw");
					D3DXMATRIX mtxWeapon = *pWeapon->GetPointerMatrix();
					
					// 向きを取得
					D3DXVECTOR3 vec ={0.0f,0.0f,1.0f};
					vec = Vec3MulMtxRotat(vec, mtxWeapon);
					 
					// 当たった位置を取得
					D3DXVECTOR3 pos = iterCollSpherTrigger->m_posHit;

					// 血を出す
					CEffectBlood::Particle(pos, vec * 5.0f, 10.0f, -0.1f, 60, 1);
					break;
				}
			}
		}
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerAttackStop::Uninit(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// モーションを動かす
	pMotionSkin->SetIsStopMotion(false);

	delete this;
}

//**********************************************
// 当たっているか探す処理
//**********************************************
bool CStatePllayerAttackStop::SeekCollision(void)
{
	// 当たり判定球更新
	for (auto& iterCollSpherTrigger : m_vCompCollSpherTrigger)
	{
		// 当たっていない、次を調べる
		if (iterCollSpherTrigger->m_bCollision == false) continue;

		int nCnt = 0;

		// 当たっているものの名前を調べる
		for (auto& iterName : iterCollSpherTrigger->GetHitedName())
		{
			// タコに当たっている
			if (iterName == "KrakenBody")
			{
				// あったオーナーゲット
				m_pHitedOwner = iterCollSpherTrigger->GetHitedOwner()[nCnt];
				return true;
			}
			nCnt++;
		}

		// 当たっているフラグをたてる
		return true;
	}

	// 当たっていなかった
	return false;
}