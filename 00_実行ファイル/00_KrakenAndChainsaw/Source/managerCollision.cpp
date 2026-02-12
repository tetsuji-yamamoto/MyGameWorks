//****************************************************
// 
// 当たり判定マネージャークラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "managerCollision.h"
#include "compCollision.h"
#include "compCollisionShapeSpher.h"
#include "compCollisionShapeMesh.h"
#include "myMath.h"

// 静的メンバー変数宣言
CManagerCollision* CManagerCollision::m_pInstance = nullptr;	// 当たり判定クラスポインタ

//****************************************************
// 当たり判定実行処理
//****************************************************
void CManagerCollision::Collision(void)
{
	// 当たり判定のコンポーネント分回す
	for (auto& iterColl : m_vectorCollision)
	{
		// 当たり判定のコンポーネント分回す
		for (auto& iterPartner : m_vectorCollision)
		{
			// 自分自身と判定しないようにする
			if (iterColl->GetID() != iterPartner->GetID())
			{
				iterColl->Dispatch(*iterPartner);
			}
		}
	}
}

//****************************************************
// コンポーネント削除
//****************************************************
void CManagerCollision::DeleteComponent(CCollision* pCollision)
{
	for (auto iter = m_vectorCollision.begin(); iter != m_vectorCollision.end();)
	{
		if (pCollision == *iter)
		{
			iter = m_vectorCollision.erase(iter);
			return;
		}

		// 要素を一つ進める
		iter++;
	}
}
