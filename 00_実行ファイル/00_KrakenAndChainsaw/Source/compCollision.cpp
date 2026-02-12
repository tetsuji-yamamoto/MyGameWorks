//********************************************************
// 
// 当たり判定基底コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compCollision.h"
#include "managerCollision.h"

//********************************************************
// 当たった者の名前を探す処理
//********************************************************
bool CCollision::FindHitedName(std::string name) 
{
	// 探す
	for (auto& iter : m_HitedName)
	{
		// あった
		if (iter == name)return true;
	}

	// 無かった
	return false;
}