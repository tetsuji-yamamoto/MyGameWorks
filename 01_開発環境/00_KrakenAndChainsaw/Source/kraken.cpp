//**************************************************
// 
// クラーケン
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "kraken.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "myMath.h"
#include "component.h"
#include "compCollisionShapeMesh.h"
#include "compCollisionCapsuleTrigger.h"
#include "compInfo.h"
#include "compMovement.h"
#include "compCorrection.h"
#include "compMotionSkin.h"
#include "compStateMachine.h"
#include "compKrakenSensing.h"
#include "compGauge.h"
#include "compDamage.h"
#include "compUpdateMotion.h"
#include "compPartMotion.h"
#include "compLife.h"
#include "stateKrakenNewtralHead.h"
#include "vector.h"
#include "quaternion.h"
#include "effectBlood.h"

using namespace std;
using namespace Quaternion;
using namespace my_vector;

//**************************************************
// クラーケンデストラクタ
//**************************************************
CKraken::CKraken() : CObject(PRIORITY_3)
{
	m_bDeath = false;	// 死亡フラグ
}

//**************************************************
// クラーケンコンストラクタ
//**************************************************
CKraken::~CKraken()
{

}

//**************************************************
// クラーケン初期化
//**************************************************
HRESULT CKraken::Init(void)
{
	return S_OK;
}

//**************************************************
// クラーケン初期化
//**************************************************
void CKraken::Init(D3DXVECTOR3 pos)
{

}

//**************************************************
// クラーケン終了
//**************************************************
void CKraken::Uninit(void)
{
	// 自身の破棄
	Release();
}

//**************************************************
// クラーケン更新
//**************************************************
void CKraken::Update(void)
{

}

//**************************************************
// クラーケン描画
//**************************************************
void CKraken::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// コンポーネント描画
	DrawComponent();
}

//***************************************************************
// クラーケン生成
//***************************************************************
CKraken* CKraken::Create(D3DXVECTOR3 pos)
{
	CKraken* pKraken = new CKraken;
	pKraken->Init(pos);
	return pKraken;
}

//***************************************************************
// クラーケン着地している場合の移動量処理
//***************************************************************
void CKraken::SetMovementWhenLanding(void)
{
	// 情報取得
	CInfo* pInfo = GetComponent<CInfo>();

	// Yの移動量をなくす
	pInfo->move.y = 0.0f;
}

//***************************************************************
// クラーケンの触手に当たり判定をつける処理
//***************************************************************
void CKraken::SetCollisionCapsuleTriggerTentacle(const int nStartBone,const std::string name)
{
	// モーションスキンコンポーネント取得
	CMotionSkin* pMotionSkin = GetComponent<CMotionSkin>();

	int nIdxBone02 = nStartBone;
	int nIdxBone03 = nStartBone + 1;
	int nIdxBone04 = nStartBone + 2;
	int nIdxBone05 = nStartBone + 3;

	string name00 = name;
	string name01 = name;
	string name02 = name;
	string name03 = name;

	D3DXMATRIX* pMtx02 = pMotionSkin->GetBoneMatrixPointer(nIdxBone02);
	D3DXMATRIX* pMtx03 = pMotionSkin->GetBoneMatrixPointer(nIdxBone03);
	D3DXMATRIX* pMtx04 = pMotionSkin->GetBoneMatrixPointer(nIdxBone04);
	D3DXMATRIX* pMtx05 = pMotionSkin->GetBoneMatrixPointer(nIdxBone05);
	D3DXVECTOR3 vecBone0203 = pMotionSkin->GetVec3BoneToBone(nIdxBone02, nIdxBone03);
	D3DXVECTOR3 vecBone0304 = pMotionSkin->GetVec3BoneToBone(nIdxBone03, nIdxBone04);
	D3DXVECTOR3 vecBone0405 = pMotionSkin->GetVec3BoneToBone(nIdxBone04, nIdxBone05);
	D3DXVECTOR3 vecBone0505 = pMotionSkin->GetVec3BoneToBone(nIdxBone05, nIdxBone05);
	D3DXQUATERNION quat0203 = QuatRotationAxis(vecBone0203);
	D3DXQUATERNION quat0304 = QuatRotationAxis(vecBone0304);
	D3DXQUATERNION quat0405 = QuatRotationAxis(vecBone0405);
	D3DXQUATERNION quat0505 = QuatRotationAxis(vecBone0505);
	float fLength0203 = Vec3Length(vecBone0203);
	float fLength0304 = Vec3Length(vecBone0304);
	float fLength0405 = Vec3Length(vecBone0405);
	float fLength0505 = Vec3Length(vecBone0505);

	// カプセルコライダー追加
	AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, name00, vecBone0203 * Parameters::HALF, quat0203, fLength0203, RadiusHitCapsule00, pMtx02);
	AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, name01, vecBone0304 * Parameters::HALF, quat0304, fLength0304, RadiusHitCapsule00, pMtx03);
	AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, name02, vecBone0405 * Parameters::HALF, quat0405, fLength0405, RadiusHitCapsule01, pMtx04);
	AddComponent<CCollisionCapsuleTrigger>(CComponent::PRIORITY_COLLISION, name03, vecBone0405 * Parameters::HALF, quat0405, fLength0405 * Parameters::HALF, RadiusHitCapsule02, pMtx05);
}

//***************************************************************
// クラーケンの触手に当たっているか否かの取得
//***************************************************************
bool CKraken::GetIsHitTentacle(const std::string name, const std::string nameHit)
{
	// モーションスキンコンポーネント取得
	CMotionSkin* pMotionSkin = GetComponent<CMotionSkin>();

	// 名前作成
	string name00 = name + "_00";
	string name01 = name + "_01";
	string name02 = name + "_02";
	string name03 = name + "_03";

	// 取得
	CCollisionCapsuleTrigger* pColl00 = GetComponent<CCollisionCapsuleTrigger>(name00);
	CCollisionCapsuleTrigger* pColl01 = GetComponent<CCollisionCapsuleTrigger>(name01);
	CCollisionCapsuleTrigger* pColl02 = GetComponent<CCollisionCapsuleTrigger>(name02);
	CCollisionCapsuleTrigger* pColl03 = GetComponent<CCollisionCapsuleTrigger>(name03);

	float fPalam = 10.0f;
	int nAmount = 10;

	// 当たっていたらtrueを返す
	if (pColl00->FindHitedName(nameHit) == true) 
	{
		CEffectBlood::Particle(pColl00->m_posHited, pColl00->m_vecHited * fPalam, 50.0f, -0.1f, 60, nAmount);
		return true;
	}
	else if (pColl01->FindHitedName(nameHit) == true) 
	{ 
		CEffectBlood::Particle(pColl01->m_posHited, pColl01->m_vecHited * fPalam, 50.0f, -0.1f, 60, nAmount);
		return true;
	}
	else if (pColl02->FindHitedName(nameHit) == true) 
	{ 
		CEffectBlood::Particle(pColl02->m_posHited, pColl02->m_vecHited * fPalam, 50.0f, -0.1f, 60, nAmount);
		return true; 
	}
	else if (pColl03->FindHitedName(nameHit) == true)
	{
		CEffectBlood::Particle(pColl03->m_posHited, pColl03->m_vecHited * fPalam, 50.0f, -0.1f, 60, nAmount);
		return true;
	}

	return false;
}