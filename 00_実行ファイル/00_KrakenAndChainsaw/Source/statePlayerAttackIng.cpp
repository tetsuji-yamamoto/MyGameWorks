//**********************************************
// 
// プレイヤー状態攻撃中
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerAttackIng.h"
#include "statePlayerAttackStop.h"
#include "statePlayerAttackRearGap.h"
#include "statePlayerNewtral.h"
#include "compMotionSkin.h"
#include "compMovement.h"
#include "compCollisionShapeSpherTrigger.h"
#include "player.h"
#include "camera.h"
#include "input.h"
#include "vector.h"
#include "debugproc.h"
#include "debugPoint.h"
#include "compWeapon.h"
#include "compDamage.h"
#include "sound.h"

using namespace std;

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerAttackIng::CStatePllayerAttackIng()
{
	// タイプ設定
	SetType(CPlayer::MOTION_ATTACK_ING);

	m_pHitedOwner = nullptr;
	m_pHitedOwner = nullptr;
}

//**********************************************
// 初期化
//**********************************************
bool CStatePllayerAttackIng::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// モーションが無ければ処理しない
	if (pMotionSkin == nullptr)return false;

	// モーションを攻撃中に設定
	pOwner->GetComponent<CMotionSkin>()->SetMotion(CPlayer::MOTION_ATTACK_ING);

	// オーナーに当たり判定生成とベクターへプッシュバック
	CWeapon* pWeapon = pOwner->GetComponent<CMotionSkin>()->GetEntity()->GetComponent<CWeapon>("chainsaw");
	if (pWeapon != nullptr)
	{
		// 武器のマトリックスポインター取得
		D3DXMATRIX* pMtx = pWeapon->GetPointerMatrix();

		// コンポーネント追加とプッシュバック
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION,string("AttackChainsaw00"),pMtx,D3DXVECTOR3(0.0f, 0.0f, -30.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION,string("AttackChainsaw01"),pMtx,D3DXVECTOR3(0.0f, 0.0f, -50.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION,string("AttackChainsaw02"),pMtx,D3DXVECTOR3(0.0f, 0.0f, -70.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION,string("AttackChainsaw03"),pMtx,D3DXVECTOR3(0.0f, 0.0f, -90.0f), 10.0f));
		m_vCompCollSpherTrigger.push_back(pOwner->AddComponent<CCollisionShapeSpherTrigger>(CComponent::PRIORITY_COLLISION,string("AttackChainsaw04"),pMtx,D3DXVECTOR3(0.0f, 0.0f, -110.0f), 10.0f));
	}

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerAttackIng::Update(void)
{
	// キー取得
	CInputKeyboard* pInputkeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CWeapon* pWeapon = pOwner->GetComponent<CMotionSkin>()->GetEntity()->GetComponent<CWeapon>("chainsaw");
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// 当たった者はいない
	m_pHitedOwner = nullptr;

	// コンポーネントが取得できていなければ処理しない
	if (pWeapon == nullptr)return;
	if (pMotionSkin == nullptr)return;

	// モーションが終っていたら
	if (pMotionSkin->GetIsMotionEnd() == true)
	{
		// あとすき状態に設定
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerAttackRearGap);
	}

	// 武器が何かしらに当たっていれば
	if (SeekCollision() == true)
	{
		// タコにダメージ
		if (m_pHitedOwner != nullptr)
		{
			CDamage* pDamage = m_pHitedOwner->GetComponent<CDamage>();
			if (pDamage != nullptr)
			{
				pDamage->SetIsDamage(true);
			}
		}

		// デバッグ位置追加
		CDebugPoint::GetInstance()->AddPoint(pWeapon->GetHitPosition(), Color::_CORAL, 10.0f, 60);

		// 攻撃ストップ状態に設定
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerAttackStop);
		return;
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerAttackIng::Uninit(void)
{
	delete this;
}

//**********************************************
// 当たっているか探す処理
//**********************************************
bool CStatePllayerAttackIng::SeekCollision(void)
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