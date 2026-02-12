//****************************************************************************************************************
// 
// オブジェクト処理
// Author Tetsuji Yamamoto
// 
//****************************************************************************************************************
#include "object.h"
#include "debugproc.h"

// 静的メンバー変数宣言
CObject* CObject::m_aPTop[PRIORITY_MAX] = {};	// 先頭のアドレス
CObject* CObject::m_aPCur[PRIORITY_MAX] = {};	// 最後尾のアドレス

int CObject::m_nNumAll = 0;						// オブジェクトの総数

//****************************************************************************************************************
// オブジェクトコンストラクタ
//****************************************************************************************************************
CObject::CObject(int nPriority)
{
	// 自信をリストに追加

	// 前のオブジェクトを最後尾のアドレスに設定
	m_pPrev = m_aPCur[nPriority];

	// 前のオブジェクトの次のオブジェクトのポインタを自身に設定
	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = this;
	}

	// 次のオブジェクトのポインタはNULL
	m_pNext = NULL;

	// 先頭のアドレスが無ければ自信を先頭アドレスにする
	if (m_aPTop[nPriority] == NULL)
	{
		m_aPTop[nPriority] = this;
	}

	// 最後尾のオブジェクトを自分自身に設定
	m_aPCur[nPriority] = this;

	m_bDeath = false;			// 死んでない状態
	m_type = TYPE_NONE;			// タイプを設定
	m_nPriority = nPriority;	// 優先度
	m_nNumAll++;				// 総数をカウントアップ
}

//****************************************************************************************************************
// オブジェクトデストラクタ
//****************************************************************************************************************
CObject::~CObject()
{
}

//****************************************************************************************************************
// オブジェクト全破棄処理
//****************************************************************************************************************
void CObject::ReleaseAll(void)
{
	// オブジェクトへのポインタ
	CObject* pObject = NULL;

	// プライオリティの数分回す
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		// 先頭のオブジェクト代入
		pObject = m_aPTop[nCntPri];

		// 次のオブジェクトがNULLになるまでループする
		while (pObject != NULL)
		{
			// 次のオブジェクトポインタを一時保存
			CObject* pObjectNext = pObject->m_pNext;

			// 終了処理
			pObject->Uninit();

			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}

	// フラグが立っているオブジェクトの削除
	DeleteAll();

	// プライオリティの数分回す
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		// 先頭と最後尾のポインタをNULLにする
		m_aPTop[nCntPri] = NULL;
		m_aPCur[nCntPri] = NULL;
	}
}

//****************************************************************************************************************
// オブジェクト全更新処理
//****************************************************************************************************************
void CObject::UpdateAll(void)
{
	// オブジェクトへのポインタ
	CObject* pObject = NULL;

	// プライオリティの数分回す
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		// 先頭のオブジェクト代入
		pObject = m_aPTop[nCntPri];

		// 次のオブジェクトがNULLになるまでループする
		while (pObject != NULL)
		{
			// 次のオブジェクトポインタを一時保存
			CObject* pObjectNext = pObject->m_pNext;

			// 死んでいなければ更新する
			if (pObject->m_bDeath != true)
			{
				// 更新処理
				pObject->Update();
			}

			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}

	// フラグが立っているオブジェクトの削除
	DeleteAll();
}

//****************************************************************************************************************
// オブジェクト全描画処理
//****************************************************************************************************************
void CObject::DrawAll(void)
{
	// プライオリティの数分回す
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		// 先頭のオブジェクト代入
		CObject* pObject = m_aPTop[nCntPri];

		// 次のオブジェクトがNULLになるまでループする
		while (pObject != NULL)
		{
			// 次のオブジェクトポインタを一時保存
			CObject* pObjectNext = pObject->m_pNext;

			// 描画処理
			pObject->Draw();

			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}
}

//****************************************************************************************************************
// オブジェクト破棄処理
//****************************************************************************************************************
void CObject::Release(void)
{
	// 死んでいる状態にする
	m_bDeath = true;
}

//****************************************************************************************************************
// オブジェクトフラグが立っている全オブジェクトの破棄処理
//****************************************************************************************************************
void CObject::DeleteAll(void)
{
	// オブジェクトへのポインタ
	CObject* pObject = NULL;

	// プライオリティの数分回す
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		// 先頭のオブジェクト代入
		pObject = m_aPTop[nCntPri];

		// 次のオブジェクトがNULLになるまでループする
		while (pObject != NULL)
		{
			// 次のオブジェクトポインタを一時保存
			CObject* pObjectNext = pObject->m_pNext;

			// 死亡フラグが立っていたら削除
			if (pObject->m_bDeath == true)
			{
				// 先頭がなくなる場合
				if (pObject->m_pPrev == NULL && pObject->m_pNext != NULL)
				{
					pObject->m_pNext->m_pPrev = NULL;
					m_aPTop[nCntPri] = pObject->m_pNext;
				}
				// 最後尾がなくなる場合
				else if (pObject->m_pPrev != NULL && pObject->m_pNext == NULL)
				{
					pObject->m_pPrev->m_pNext = NULL;
					m_aPCur[nCntPri] = pObject->m_pPrev;
				}
				// リストの途中の場合
				else if (pObject->m_pPrev != NULL && pObject->m_pNext != NULL)
				{
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;
					pObject->m_pPrev->m_pNext = pObject->m_pNext;
				}
				// 最後のひとりだった場合
				else
				{
					m_aPTop[nCntPri] = NULL;
					m_aPCur[nCntPri] = NULL;
				}

				// このオブジェクトを破棄
				delete pObject;

				// 総数カウントダウン
				m_nNumAll--;
			}

			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}
}

//****************************************************************************************************************
// オブジェクトデバッグ表示処理
//****************************************************************************************************************
void CObject::DrawDebug(void)
{
	CDebugprocc::Print("[ s Object おぶじぇくと オブジェクト 物 ]\n");
	CDebugprocc::Print("m_nNumAll : %d\n", m_nNumAll);
	CDebugprocc::Print("\n");
}