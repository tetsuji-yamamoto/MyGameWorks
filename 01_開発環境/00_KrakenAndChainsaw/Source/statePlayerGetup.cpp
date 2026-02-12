//**********************************************
// 
// プレイヤー状態吹き飛ばし
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "statePlayerGetup.h"
#include "statePlayerNewtral.h"
#include "object.h"
#include "player.h"
#include "compInfo.h"
#include "compCollisionCapsuleTrigger.h"
#include "compMotionSkin.h"

//**********************************************
// コンストラクタ
//**********************************************
CStatePlayerGetup::CStatePlayerGetup()
{
	// タイプ設定
	SetType(CPlayer::MOTION_GETUP);
}

//**********************************************
// デストラクタ
//**********************************************
CStatePlayerGetup::~CStatePlayerGetup()
{
}

//**********************************************
// 初期化
//**********************************************
bool CStatePlayerGetup::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 情報取得
	CInfo* pInfo = pOwner->GetComponent<CInfo>();

	// モーションの切替
	CMotionSkin* pMotionSkin = pOwner->GetComponent<CMotionSkin>();
	if (pMotionSkin != nullptr)
	{
		// 起き上がりモーション
		pMotionSkin->SetMotion(CPlayer::MOTION_GETUP);
	}

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStatePlayerGetup::Update(void)
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
			// 待機状態に切替
			pOwner->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerNewtral);
		}
	}
}

//**********************************************
// 終了
//**********************************************
void CStatePlayerGetup::Uninit(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 当たり判定生成
	pOwner->AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, std::string("PlayerCapsule"), D3DXVECTOR3(0.0f, 50.0f, 0.0f), InitValu::QUAT_NULL, 100.0f, 50.0f);	// カプセルコライダー

	// 自身の削除
	delete this;
}