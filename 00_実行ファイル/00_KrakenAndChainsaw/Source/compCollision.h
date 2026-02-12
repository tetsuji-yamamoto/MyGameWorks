//********************************************************
// 
// 当たり判定基底コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_COLLISION_H_
#define _COMPONENT_COLLISION_H_

#include "component.h"
#include "managerCollision.h"

// 前方宣言
class CCollisionShapeMesh;
class CCollisionShapeSpher;
class CCollisionShapeSpherTrigger;
class CCollisionCapsuleTrigger;
class CObject;

// 当たり判定基底クラス
class CCollision : public CComponent
{
public:

	CCollision()
	{
		// 当たり判定マネージャに追加
		CManagerCollision::GetInstance()->AddComponent(this);
	}

	~CCollision() 
	{
		// 当たり判定マネージャから削除
		CManagerCollision::GetInstance()->DeleteComponent(this);
	}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Dispatch(CCollision& rCollision) = 0;
	virtual void Collision(CCollisionShapeMesh* pCollMesh) = 0;
	virtual void Collision(CCollisionShapeSpher* pSpher) = 0;
	virtual void Collision(CCollisionShapeSpherTrigger* pSpher) = 0;
	virtual void Collision(CCollisionCapsuleTrigger* pCapsule) = 0;

	bool FindHitedName(std::string name);
	void ClearHitedName(void) { m_HitedName.clear(); }
	void ClearHitedOwner(void) { m_HitedOwner.clear(); }
	void AddHitedName(std::string name) { m_HitedName.push_back(name); }
	void AddHitedOwner(CObject* pObj) { m_HitedOwner.push_back(pObj); }
	std::vector<std::string> GetHitedName(void) { return m_HitedName; }
	std::vector<CObject*> GetHitedOwner(void) { return m_HitedOwner; }
private:
	std::vector<std::string> m_HitedName;	// 当たった物の名前
	std::vector<CObject*> m_HitedOwner;	// 当たった物
};
#endif