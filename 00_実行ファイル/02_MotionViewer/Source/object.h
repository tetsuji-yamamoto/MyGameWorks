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

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void DeleteAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void DrawDebug(void);

	static int GetObujectNumAll() { return m_nNumAll; }
	void SetType(TYPE type) { m_type = type; };
	void SetPriority(PRIORITY priority) { m_nPriority = priority; };

	static CObject* GetTop(int nPriority) { return m_aPTop[nPriority]; };
	CObject* GetNext(void) { return m_pNext; };
	TYPE GetType(void) { return m_type; };
	int GetPriority(void) { return m_nPriority; };

	void Release(void);
protected:

private:
	static CObject* m_aPTop[PRIORITY_MAX];	// 先頭のアドレス
	static CObject* m_aPCur[PRIORITY_MAX];	// 最後尾のアドレス
	CObject* m_pPrev;						// 前のオブジェクトのポインタ
	CObject* m_pNext;						// 次のオブジェクトのポインタ
	bool m_bDeath;							//	死亡フラグ

	static int m_nNumAll;	// オブジェクトの総数
	int m_nID;				// 自分自身のID
	int m_nPriority;		// 優先度
	TYPE m_type;			// 種類
};

#endif
