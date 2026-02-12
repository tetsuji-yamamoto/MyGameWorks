//**********************************************
// 
// プレイヤーゲーム時状態攻撃
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerAttackRearGap.h"
#include "statePlayerAttackStop.h"
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
#include "manager.h"
#include "sound.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePllayerAttackRearGap::CStatePllayerAttackRearGap()
{
	// タイプ設定
	SetType(CPlayer::MOTION_ATTACK_GAP);
}

//**********************************************
// 初期化
//**********************************************
bool CStatePllayerAttackRearGap::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// モーションスキンのエンティティ取得
	CEntity* pEntity = pOwner->GetComponent<CMotionSkin>()->GetEntity();
	
	// モーションを待機に設定
	pOwner->GetComponent<CMotionSkin>()->SetMotion(CPlayer::MOTION_ATTACK_GAP);

	// 武器に当たり判定削除
	pOwner->DeleteComponent<CCollisionShapeSpherTrigger>(std::string("AttackChainsaw00"));
	pOwner->DeleteComponent<CCollisionShapeSpherTrigger>(std::string("AttackChainsaw01"));
	pOwner->DeleteComponent<CCollisionShapeSpherTrigger>(std::string("AttackChainsaw02"));
	pOwner->DeleteComponent<CCollisionShapeSpherTrigger>(std::string("AttackChainsaw03"));
	pOwner->DeleteComponent<CCollisionShapeSpherTrigger>(std::string("AttackChainsaw04"));

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePllayerAttackRearGap::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// コンポーネント取得
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();

	// コンポーネントが取得できていなければ処理しない
	if (pMotionSkin == nullptr)return;

	// モーションが終っていたら待機状態に戻す
	if (pMotionSkin->GetIsMotionEnd() == true)
	{
		// 移動状態に設定
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerNewtral);
	}
}

//**********************************************
// 描画
//**********************************************
void CStatePllayerAttackRearGap::Uninit(void)
{
	// チェンソー切る音ストップ
	CManager::GetInstance()->GetSound()->StopSound(CSound::SOUND_LABEL_BGM_CHAINSAW_CUT);

	delete this;
}