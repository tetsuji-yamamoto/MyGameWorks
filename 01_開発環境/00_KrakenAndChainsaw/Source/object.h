//**********************************************
// 
// オブジェクト処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "renderer.h"
#include "component.h"
#include "hitResult.h"
#include "managerComponent.h"
#include "managerCollision.h"

// マクロ定義
#define PRIORITY_NOMAL (3)			// 優先度の初期値 

// オブジェクトクラス
class CObject
{
public:
	// タイプ
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_CHARCTER,
		TYPE_BULLET,
		TYPE_BG,
		TYPE_EXPLOSION,
		TYPE_EFFECT,
		TYPE_SCORE,
		TYPE_MODEL_X,
		TYPE_MAX
	}TYPE;

	// 優先度
	typedef enum
	{
		PRIORITY_0 = 0,
		PRIORITY_1,
		PRIORITY_2,
		PRIORITY_3,
		PRIORITY_4,
		PRIORITY_5,
		PRIORITY_6,
		PRIORITY_7,
		PRIORITY_8,
		PRIORITY_MAX
	}PRIORITY;

	CObject(int nPriority = PRIORITY_NOMAL);
	virtual ~CObject();

	// コンポーネント追加
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		// インスタンス生成
		T* component = new T(std::forward<Args>(args)...);

		// オーナー設定
		component->SetOwner(this);
		
		// 優先度設定
		component->SetPriority(CComponent::PRIORITY_00);

		// 初期化処理
		component->Init();

		// コンポーネントをリストへ追加
		m_components[CComponent::PRIORITY_00].push_back(component);

		// コンポーネントマネージャへ追加
		CManagerComponent::GetInstance()->AddComponent(component);

		// リターン
		return component;
	}

	// コンポーネント追加(優先度あり)
	template<typename T, typename... Args>
	T* AddComponent(CComponent::PRIORITY priority,Args&&... args)
	{
		// インスタンス生成
		T* component = new T(std::forward<Args>(args)...);

		// オーナー設定
		component->SetOwner(this);

		// 優先度設定
		component->SetPriority(priority);

		// 初期化処理
		component->Init();

		// コンポーネントをリストへ追加
		m_components[priority].push_back(component);

		// コンポーネントマネージャへ追加
		CManagerComponent::GetInstance()->AddComponent(component);

		// リターン
		return component;
	}

	// コンポーネント追加(名前あり)
	template<typename T, typename... Args>
	T* AddComponent(const std::string name, Args&&... args)
	{
		// インスタンス生成
		T* component = new T(std::forward<Args>(args)...);

		// オーナー設定
		component->SetOwner(this);

		// 名前設定
		component->SetName(name);

		// 優先度設定
		component->SetPriority(CComponent::PRIORITY_00);

		// 初期化処理
		component->Init();

		// コンポーネントをリストへ追加
		m_components[CComponent::PRIORITY_00].push_back(component);

		// コンポーネントマネージャへ追加
		CManagerComponent::GetInstance()->AddComponent(component);

		// リターン
		return component;
	}

	// コンポーネント追加(優先度と名前あり)
	template<typename T, typename... Args>
	T* AddComponent(CComponent::PRIORITY priority, const std::string name,Args&&... args)
	{
		// インスタンス生成
		T* component = new T(std::forward<Args>(args)...);

		// オーナー設定
		component->SetOwner(this);

		// 名前設定
		component->SetName(name);

		// 優先度設定
		component->SetPriority(priority);

		// 初期化処理
		component->Init();

		// コンポーネントをリストへ追加
		m_components[priority].push_back(component);

		// コンポーネントマネージャへ追加
		CManagerComponent::GetInstance()->AddComponent(component);

		// リターン
		return component;
	}

	// コンポーネント取得
	template<typename T>
	T* GetComponent(const std::string& name = "")
	{
		// 優先度順に回す
		for (int nCntPri = 0; nCntPri < CComponent::PRIORITY_MAX; nCntPri++)
		{
			// キャストできるものを探す
			for (auto* iterComp : m_components[nCntPri])
			{
				// キャスト出来たら
				if (auto* t = dynamic_cast<T*>(iterComp))
				{
					// 名前取得
					std::string myName = iterComp->GetName();

					// 名前が同じならもしくは空ならば
					if (myName == name || myName.empty())
					{
						return t;
					}
				}
			}
		}

		// 無かった
		return nullptr;
	}

	// コンポーネント破棄
	template<typename T>
	bool DeleteComponent(const std::string& name = "")
	{
		// 優先度順に回す
		for (int nCntPri = 0; nCntPri < CComponent::PRIORITY_MAX; nCntPri++)
		{
			// キャストできるものを探す
			for (auto iterComp = m_components[nCntPri].begin(); iterComp != m_components[nCntPri].end();)
			{
				// キャスト出来たら
				if (auto* t = dynamic_cast<T*>(*iterComp))
				{
					// 名前取得
					std::string myName = (*iterComp)->GetName();

					// 名前が同じならもしくは空ならば
					if (myName == name || myName.empty())
					{
						(*iterComp)->Uninit();
						iterComp = m_components[nCntPri].erase(iterComp);
						return true;
					}
				}

				iterComp++;
			}
		}

		// 無かった
		return false;
	}

	virtual HRESULT	Init	(void)	= 0;
	virtual void	Uninit	(void)	= 0;
	virtual void	Update	(void)	= 0;
	virtual void	Draw(	void)	= 0;

	void UninitComponent(void);
	void UpdateComponent(void);
	void DrawComponent	(void);

	static void ReleaseAll	(void);
	static void DeleteAll	(void);
	static void UpdateAll	(void);
	static void DrawAll		(void);
	static void DrawDebug	(void);

	// セッター
	void	SetType		(TYPE type)			{ m_type		= type; };
	void	SetPriority	(PRIORITY priority)	{ m_nPriority	= priority; };
	void	SetName		(std::string name)	{ m_name		= name;}

	// ゲッター
	static int		GetObujectNumAll	(void)			{ return m_nNumAll; }
	static CObject*	GetTop				(int nPriority)	{ return m_aPTop[nPriority]; };
	std::string		GetName				(void)			{ return m_name;}
	CObject*		GetNext				(void)			{ return m_pNext; };
	TYPE			GetType				(void)			{ return m_type; };
	int				GetPriority			(void)			{ return m_nPriority; };

	void Release(void);
private:
	static CObject*				m_aPTop[PRIORITY_MAX];					// 先頭のアドレス
	static CObject*				m_aPCur[PRIORITY_MAX];					// 最後尾のアドレス
	static int					m_nNumAll;								// オブジェクトの総数
	std::vector<CComponent*>	m_components[CComponent::PRIORITY_MAX];	// コンポーネントリスト
	std::string					m_name;									// オブジェクトの名前 
	CObject*					m_pPrev;								// 前のオブジェクトのポインタ
	CObject*					m_pNext;								// 次のオブジェクトのポインタ
	TYPE						m_type;									// 種類
	bool						m_bDeath;								// 死亡フラグ
	int							m_nID;									// 自分自身のID
	int							m_nPriority;							// 優先度
};

#endif
