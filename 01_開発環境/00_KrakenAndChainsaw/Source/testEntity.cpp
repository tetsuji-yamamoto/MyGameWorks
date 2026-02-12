//*****************************************************
// 
// テスエンティティ
// Author Tetesuji yamamot
// 
//*****************************************************
#include "testEntity.h"
#include "manager.h"
#include "compStateMachine.h"
#include "compUpdateMotion.h"
#include "compMotionSkin.h"

//********************************************************
// コンストラクタ
//********************************************************
CTestEntity::CTestEntity()
{

}

//********************************************************
// デストラクタ
//********************************************************
CTestEntity::~CTestEntity()
{

}

//********************************************************
// 初期化
//********************************************************
HRESULT CTestEntity::Init(void)
{
	float fMovement = CManager::GetInstance()->GetMovementPlayer();
	float fFriction = CManager::GetInstance()->GetFrictionPlayer();
	float fGravity = CManager::GetInstance()->GetGravityPlayer();

	AddComponent<CStateMachine>();																																				// ステートマシーン
	AddComponent<CUpdateMotion>(CComponent::PRIORITY_MOTION_00);																												// モーション更新
	AddComponent<CMotionSkin>(CComponent::PRIORITY_MOTION_01, CManager::GetInstance()->GetFilePathMotionPlayer().c_str());	
	
	return S_OK;
}

//********************************************************
// 終了
//********************************************************
void CTestEntity::Uninit(void)
{
	// 自身の破棄
	Release();
}

//********************************************************
// 更新
//********************************************************
void CTestEntity::Update(void)
{
	// コンポーネントの更新
	UpdateComponent();
}

//********************************************************
// 描画
//********************************************************
void CTestEntity::Draw(void)
{
	// コンポーネントの描画
	GetComponent<CMotionSkin>()->Draw();
}