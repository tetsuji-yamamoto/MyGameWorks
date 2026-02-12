//***********************************************
// 
// 画面遷移処理
// Author Tetsuji Yamamoto
// 
//***********************************************
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "object.h"

// シーンクラス
class CScene : public CObject	// CObject継承
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RESULT_GAMEOVER,
		MODE_RESULT_GAMECLEAR,
		MODE_TEST,
		MODE_MAX
	}MODE;

	CScene();
	~CScene();

	virtual HRESULT Init(void);
	virtual void Init(MODE mode);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	

	MODE GetMode(void) { return m_mode; }

	// オブジェクト追加
	void AddObj(CObject* pObject)
	{
		m_pObjecs.push_back(pObject);
	}

	// オブジェクト取得
	template<typename T>
	T* GetObj(std::string name = "")
	{
		// キャストできるものを探す
		for (auto* iterObj : m_pObjecs)
		{
			// キャスト出来たら
			if (auto* t = dynamic_cast<T*>(iterObj))
			{
				// 名前取得
				std::string getName = iterObj->GetName();

				// 名前が同じ又は名前が無ければ
				if (getName == name || getName == "")
				{
					// これを返す
					return t;
				}
			}
		}

		// 無かった
		return nullptr;
	}

	// オブジェクト破棄
	template<class T>
	bool DeleteObj(std::string name = "")
	{
		// キャストできるものを探す
		for (auto iterObj = m_pObjecs.begin(); iterObj != m_pObjecs.end();)
		{
			// キャスト出来たら
			if (auto* t = dynamic_cast<T*>(*iterObj))
			{
				// 名前取得
				std::string getName = (*iterObj)->GetName();

				// 名前が同じ又は名前が無ければ
				if (getName == name || getName == "")
				{
					// 削除
					(*iterObj)->Uninit();
					iterObj = m_pObjecs.erase(iterObj);
					return true;
				}
			}

			iterObj++;
		}

		// 無かった
		return false;
	}

	// オブジェクト全削除
	void OllDeleteObjs(void)
	{
		// キャストできるものを探す
		for (auto* iterObj : m_pObjecs)
		{
			iterObj->Uninit();
		}
		m_pObjecs.clear();
	}

private:
	MODE m_mode;
	std::vector<CObject*>	m_pObjecs;	// オブジェクトたち
};

#endif
