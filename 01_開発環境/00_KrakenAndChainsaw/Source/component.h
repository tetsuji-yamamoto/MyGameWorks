//********************************************************
// 
// コンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_H_
#define _COMPONENT_H_


// 前方宣言
class CObject;

// コンポーネントクラス
class CComponent
{
public:

	// 優先度
	typedef enum
	{
		PRIORITY_00 = 0,
		PRIORITY_01,
		PRIORITY_02,
		PRIORITY_03,
		PRIORITY_04,
		PRIORITY_MOTION_00,		// モーション
		PRIORITY_MOTION_01,		// モーション
		PRIORITY_MOTION_02,		// モーション
		PRIORITY_MOTION_03,		// モーション
		PRIORITY_MOTION_04,		// モーション
		PRIORITY_HITBEFORE,		// 当たり判定前
		PRIORITY_COLLISION,		// 当たり判定
		PRIORITY_CORRECTION,	// 修正
		PRIORITY_UI,			// UI
		PRIORITY_MAX
	}PRIORITY;

	CComponent() 
	{
		m_nID = m_nCntID;
		m_nCntID++;
	}

	~CComponent();

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// セッター
	void SetOwner(CObject* pOwner) { m_pOwner = pOwner; }
	void SetName(const std::string& name) { m_name = name; }
	void SetPriority(const int nPriority) { m_nPriority = nPriority; }

	// ゲッター
	CObject* GetOwner(void) { return m_pOwner; }
	std::string& GetName() { return m_name; }
	int GetID(void) { return m_nID; }
	int GetPriority(void) { return m_nPriority; }

private:
	CObject* m_pOwner = nullptr;	// オーナー
	std::string m_name = "";		// 名前
	int m_nID;						// 自身のID
	int m_nPriority;				// 優先度
	static int m_nCntID;			// コンポーネントのIDカウンター
};

// エンティティクラス
class CEntity
{
public:

	// コンストラクタ
	CEntity() {}

	// デストラクタ
	~CEntity() {}

	// コンポーネントを追加する処理
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		// インスタンス生成
		T* component = new T(std::forward<Args>(args)...);
		
		// オーナー設定
		component->SetOwner(this);

		// 初期化処理
		component->Init();

		// コンポーネントをリストへ追加
		m_components.push_back(component);

		// リターン
		return component;
	}

	// コンポーネント取得処理
	template<typename T>T* GetComponent(const std::string& name = "")
	{
		// キャストできるものを探す
		for (auto* iterComp : m_components)
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

		// 無かった
		return nullptr;
	}

	// コンポーネント破棄処理
	template<typename T>void DeleteComponent(const std::string& name = "")
	{
		// キャストできるものを探す
		for (auto iterComp = m_components.begin(); iterComp != m_components.end();)
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
					iterComp = m_components.erase(iterComp);
					continue;
				}
			}

			iterComp++;
		}
	}

	// 破棄処理
	void Uninit()
	{
		// コンポーネントの破棄
		for (auto* iterComp : m_components)
		{
			iterComp->Uninit();
		}
		m_components.clear();

		// 自身の破棄
		delete this;
	}

	// 更新処理
	void Update()
	{
		// すべてのコンポーネントを更新
		for (auto* iterComp : m_components)
		{
			iterComp->Update();
		}
	}

	// 描画処理
	void Draw()
	{
		// すべてのコンポーネントを描画
		for (auto* iterComp : m_components)
		{
			iterComp->Draw();
		}
	}

	// 生成処理
	static CEntity* Create()
	{
		return new CEntity;
	}

	// 更新処理
	static void Update(CEntity*& pEntity)
	{
		if (pEntity != nullptr)
		{
			pEntity->Update();
		}
	}

	// 描画処理
	static void Draw(CEntity*& pEntity)
	{
		if (pEntity != nullptr)
		{
			pEntity->Draw();
		}
	}

	// コンポーネント配列取得
	std::vector<CComponent*>& GetComponentVector(void) { return m_components; }

	// コンポーネントプッシュバック
	void AddComponentPushBack(CComponent* component) { m_components.push_back(component); }

private:
	std::vector<CComponent*> m_components;	// コンポーネントリスト
};

#endif
