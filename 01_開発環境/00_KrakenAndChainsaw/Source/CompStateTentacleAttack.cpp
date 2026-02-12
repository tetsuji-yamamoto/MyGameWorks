//**********************************************
// 
// タコ触手状態攻撃コンポーネントクラス
// Author Tetsuji Yamamoto
//
//**********************************************
#include "CompStateTentacleAttack.h"
#include "CompStateTentacleNewtral.h"
#include "compPartMotion.h"
#include "CompCounter.h"
#include "compCollisionCapsuleTrigger.h"
#include "manager.h"
#include "kraken.h"
#include "quaternion.h"
#include "vector.h"

using namespace Quaternion;
using namespace my_vector;
using namespace std;

//********************************************************
// コンストラクタ
//********************************************************
CCompStateTentacleAttack::CCompStateTentacleAttack(Argument arg)
{
	m_Arg			= arg;		// 引数情報保存
	m_bAttack		= false;	// 攻撃してるか否か
	m_pMyCounter	= nullptr;	// カウンターのポインター
}

//********************************************************
// デストラクタ
//********************************************************
CCompStateTentacleAttack::~CCompStateTentacleAttack()
{

}

//********************************************************
// 初期化
//********************************************************
void CCompStateTentacleAttack::Init(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 特定部位モーションコンポーネント取得
	CPartMotion* pPartMotionTentacle = m_Arg.NewtralArg.pPartMotion;

	// 攻撃モーションに設定
	pPartMotionTentacle->SetMotion(CKraken::MOTION_ATTACK_PRE, TIME_MOTIONBLEND);

	// カウンター設定
	string Name = "Counter" + m_Arg.CapsuleColliderName;
	m_pMyCounter = pOwner->AddComponent<CCounter>(Name,ATTACKTIME_MAX);
}

//********************************************************
// 終了
//********************************************************
void CCompStateTentacleAttack::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 特定部位モーションコンポーネント取得
	CPartMotion* pPartMotionTentacle = m_Arg.NewtralArg.pPartMotion;

	// カウンターがあれば処理
	if (m_pMyCounter != nullptr)
	{
		// 今の時間を取得
		int nCntNow = m_pMyCounter->GetNow();

		// 一定の時間が経って、まだ攻撃してないとき
		if (nCntNow >= ATTACKTIME_00_NONE && m_bAttack == false)
		{
			// 攻撃用のカプセルコライダーを出す
			AddCapsuleCollider();

			// 攻撃している状態にする
			m_bAttack = true;
		}

		// タイマーがマックスであれば
		if (m_pMyCounter->GetIsMax() == true)
		{
			// 当たり判定削除
			pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);
			pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);
			pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);
			pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);

			// カウンター削除
			pOwner->DeleteComponent<CCounter>(m_pMyCounter->GetName());
			m_pMyCounter = nullptr;
		}
	}

	// モーションが終っていれば
	if (pPartMotionTentacle->GetIsMotionEnd() == true)
	{
		// 待機状態の引数設定
		CCompStateTentacleNewtral::Argument NewtralArg = m_Arg.NewtralArg;

		// 待機状態コンポーネント設定
		pOwner->AddComponent<CCompStateTentacleNewtral>(NewtralArg.Name,NewtralArg);

		// 自身を破棄
		pOwner->DeleteComponent<CCompStateTentacleAttack>(GetName());

		// 処理終了
		return;
	}
}

//********************************************************
// 更新
//********************************************************
void CCompStateTentacleAttack::Uninit(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 当たり判定削除
	pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);
	pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);
	pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);
	pOwner->DeleteComponent<CCollisionCapsuleTrigger>(m_Arg.CapsuleColliderName);

	// 自身の破棄
	delete this;
}

//********************************************************
// カプセルコライダー追加
//********************************************************
void CCompStateTentacleAttack::AddCapsuleCollider(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// モーションスキンコンポーネント取得
	CMotionSkin* pMotionSkin = GetOwner()->GetComponent<CMotionSkin>();

	// 特定部位モーション情報取得
	CPartMotion* pPartMotion = m_Arg.NewtralArg.pPartMotion;

	// ボーンのインデックス取得
	std::vector<int> BoneIdx = pPartMotion->GetBoneIdx();

	// ボーンのインデックス取得
	int nIdx02 = BoneIdx[BONEIDX_02];
	int nIdx03 = BoneIdx[BONEIDX_03];
	int nIdx04 = BoneIdx[BONEIDX_04];
	int nIdx05 = BoneIdx[BONEIDX_05];

	D3DXMATRIX* pMtx02 = pMotionSkin->GetBoneMatrixPointer(nIdx02);
	D3DXMATRIX* pMtx03 = pMotionSkin->GetBoneMatrixPointer(nIdx03);
	D3DXMATRIX* pMtx04 = pMotionSkin->GetBoneMatrixPointer(nIdx04);
	D3DXMATRIX* pMtx05 = pMotionSkin->GetBoneMatrixPointer(nIdx05);
	D3DXVECTOR3 vecBone0203 = pMotionSkin->GetVec3BoneToBone(nIdx02, nIdx03);
	D3DXVECTOR3 vecBone0304 = pMotionSkin->GetVec3BoneToBone(nIdx03, nIdx04);
	D3DXVECTOR3 vecBone0405 = pMotionSkin->GetVec3BoneToBone(nIdx04, nIdx05);
	D3DXVECTOR3 vecBone0505 = pMotionSkin->GetVec3BoneToBone(nIdx05, nIdx05);
	D3DXQUATERNION quat0203 = QuatRotationAxis(vecBone0203);
	D3DXQUATERNION quat0304 = QuatRotationAxis(vecBone0304);
	D3DXQUATERNION quat0405 = QuatRotationAxis(vecBone0405);
	D3DXQUATERNION quat0505 = QuatRotationAxis(vecBone0505);
	float fLength0203 = Vec3Length(vecBone0203);
	float fLength0304 = Vec3Length(vecBone0304);
	float fLength0405 = Vec3Length(vecBone0405);
	float fLength0505 = Vec3Length(vecBone0505);

	// カプセルコライダー追加
	pOwner->AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, m_Arg.CapsuleColliderName, vecBone0203 * OffsetScale, quat0203, fLength0203, RadiusHitCapsule00, pMtx02);
	pOwner->AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, m_Arg.CapsuleColliderName, vecBone0304 * OffsetScale, quat0304, fLength0304, RadiusHitCapsule00, pMtx03);
	pOwner->AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, m_Arg.CapsuleColliderName, vecBone0405 * OffsetScale, quat0405, fLength0405, RadiusHitCapsule01, pMtx04);
	pOwner->AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, m_Arg.CapsuleColliderName, vecBone0405 * OffsetScale, quat0405, fLength0405 * LengthScale, RadiusHitCapsule02, pMtx05);
}