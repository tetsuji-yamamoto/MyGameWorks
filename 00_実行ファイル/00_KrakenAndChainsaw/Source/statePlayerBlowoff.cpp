//**********************************************
// 
// プレイヤー状態吹き飛ばし
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerBlowoff.h"
#include "statePlayerNewtral.h"
#include "statePlayerGetup.h"
#include "object.h"
#include "player.h"
#include "compInfo.h"
#include "compCollisionCapsuleTrigger.h"
#include "compMotionSkin.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePlayerBlowoff::CStatePlayerBlowoff()
{
	// タイプ設定
	SetType(CPlayer::MOTION_BLOWOFF);
}

//**********************************************
// デストラクタ
//**********************************************
CStatePlayerBlowoff::~CStatePlayerBlowoff() 
{
}

//**********************************************
// 初期化
//**********************************************
bool CStatePlayerBlowoff::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 情報取得
	CInfo* pInfo = pOwner->GetComponent<CInfo>();

	// カプセルコライダー取得
	CCollisionCapsuleTrigger* pCollCapsuleTrii = pOwner->GetComponent<CCollisionCapsuleTrigger>(std::string("PlayerCapsule"));
	if (pCollCapsuleTrii != nullptr)
	{
		// 吹っ飛ばす
		pInfo->move.y += BLOWOFF_ADDMOVE_Y;
	}

	// モーションの切替
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();
	if (pMotionSkin != nullptr)
	{
		pMotionSkin->SetMotion(CPlayer::MOTION_BLOWOFF);
	}

	// 当たり判定を消す
	pOwner->DeleteComponent<CCollisionCapsuleTrigger>(std::string("PlayerCapsule"));

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePlayerBlowoff::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 情報取得
	CInfo* pInfo = pOwner->GetComponent<CInfo>();

	// モーションの切替
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();
	if (pMotionSkin != nullptr)
	{
		// モーションが終っていたら
		if (pMotionSkin->GetIsMotionEnd() == true)
		{
			// 起き上がり状態に切替
			pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePlayerGetup);
		}
	}
}

//**********************************************
// 終了
//**********************************************
void CStatePlayerBlowoff::Uninit(void)
{
	// 自身の削除
	delete this;
}