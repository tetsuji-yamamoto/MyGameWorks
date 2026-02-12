//********************************************************
// 
// コンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

// 前方宣言
class CEntity;	// エンティティ

// コンポーネントクラス
class CComponent
{
public:
	CComponent() {}
	virtual ~CComponent() {}

	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	void SetOwner(CEntity* pOwner) { m_pOwner = pOwner; }
	void SetName(const std::string& name) { m_name = name; }
	std::string& GetName() { return m_name; }

private:
	CEntity* m_pOwner = nullptr;	// オーナー
	std::string m_name = "";		// 名前
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
	template<typename T,typename... Args>
	T* AddComponent(Args&&... args)
	{
		// インスタンス生成
		T* component = new T(std::forward<Args>(args)...);
		
		// オーナー設定
		component->SetOwner(this);

		// コンポーネントをリストへ追加
		m_components.push_back(component);

		// リターン
		return component;
	}

	// コンポーネント取得処理
	template<typename T>
	T* GetComponent(const std::string& name = "")
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

	// コンポーネント配列取得
	std::vector<CComponent*>& GetComponentVector(void) { return m_components; }

	// コンポーネントプッシュバック
	void AddComponentPushBack(CComponent* component) { m_components.push_back(component); }

private:
	std::vector<CComponent*> m_components;	// コンポーネントリスト
};

#endif
