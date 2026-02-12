//****************************************************
// 
// コンポーネントマネージャークラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "managerComponent.h"
#include "managerCollision.h"
#include "component.h"

// 静的メンバー変数宣言
CManagerComponent* CManagerComponent::m_pInstance = nullptr;	//	マネージャコンポーネントクラスインスタンス

//****************************************************
// 更新
//****************************************************
void CManagerComponent::Update(void)
{
	// 優先度順に回す
	for (int nPriority = CComponent::PRIORITY_00; nPriority < CComponent::PRIORITY_COLLISION; nPriority++)
	{
		// コンポーネント更新
		for (auto& iterComp : m_components[nPriority])
		{
			iterComp->Update();
		}
	}
}

//****************************************************
// 当たり判定前
//****************************************************
void CManagerComponent::HitBefore(void)
{
	// コンポーネント更新
	for (auto& iterComp : m_components[CComponent::PRIORITY_HITBEFORE])
	{
		iterComp->Update();
	}
}

//****************************************************
// 当たり判定
//****************************************************
void CManagerComponent::Collision(void)
{
	// コンポーネント更新
	for (auto& iterComp : m_components[CComponent::PRIORITY_COLLISION])
	{
		iterComp->Update();
	}

	// 当たり判定マネージャ取得
	CManagerCollision* pManeColl = CManagerCollision::GetInstance();

	// 当たり判定マネージャが無ければ処理しない
	if (pManeColl == nullptr)return;

	// 当たり判定処理実行
	pManeColl->Collision();
}

//****************************************************
// 修正
//****************************************************
void CManagerComponent::Correction(void)
{
	// 修正コンポーネント更新
	for (auto& iterComp : m_components[CComponent::PRIORITY_CORRECTION])
	{
		iterComp->Update();
	}
}

//****************************************************
// コンポーネント削除
//****************************************************
void CManagerComponent::DeleteComponent(CComponent* pComponent)
{
	// 優先度順に回す
	for (int nPriority = CComponent::PRIORITY_00; nPriority < CComponent::PRIORITY_MAX; nPriority++)
	{
		// 探す
		for (auto iterComp = m_components[nPriority].begin(); iterComp != m_components[nPriority].end();)
		{
			// 同じであれば
			if (*iterComp == pComponent)
			{
				// 削除
				m_components[nPriority].erase(iterComp);
				return;
			}

			iterComp++;
		}
	}
}