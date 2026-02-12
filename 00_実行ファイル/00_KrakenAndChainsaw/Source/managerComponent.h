//****************************************************
// 
// コンポーネントマネージャークラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _MANAGER_COMPONENT_H_
#define _MANAGER_COMPONENT_H_

#include "component.h"

// コンポーネント形状クラス
class CManagerComponent
{
public:

	// 生成処理
	static void Create(void)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CManagerComponent;
		}
	}

	// 破棄処理
	static void Delete()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	// 取得処理
	static CManagerComponent* GetInstance(void)
	{
		return m_pInstance;
	}

	// コンポーネント追加
	void AddComponent(CComponent* pComponent)
	{
		int nPriority = pComponent->GetPriority();
		m_components[nPriority].push_back(pComponent);
	}

	void Update(void);
	void HitBefore(void);
	void Collision(void);
	void Correction(void);
	void DeleteComponent(CComponent* pComponent);

private:
	CManagerComponent() {}
	~CManagerComponent() {}

	static CManagerComponent* m_pInstance;								//	マネージャコンポーネントクラスインスタンス
	std::vector<CComponent*> m_components[CComponent::PRIORITY_MAX];	// コンポーネントリスト
};

#endif