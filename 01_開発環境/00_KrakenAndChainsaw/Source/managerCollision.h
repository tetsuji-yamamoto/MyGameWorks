//****************************************************
// 
// 当たり判定マネージャークラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _MANAGER_COLLISION_H_
#define _MANAGER_COLLISION_H_

class CCollision;

// 当たり判定形状クラス
class CManagerCollision
{
public:

	// 生成処理
	static void Create(void)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CManagerCollision;
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
	static CManagerCollision* GetInstance(void)
	{
		return m_pInstance;
	}

	// コンポーネント追加
	void AddComponent(CCollision* pCollision)
	{
		m_vectorCollision.push_back(pCollision);
	}

	void DeleteComponent(CCollision* pCollision);
	void Collision(void);

private:
	CManagerCollision(){}
	~CManagerCollision(){}

	static CManagerCollision* m_pInstance;		// 当たり判定クラスインスタンス
	std::vector<CCollision*> m_vectorCollision;	// 当たり判定コンポーネントリスト
};

#endif