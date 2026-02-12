//****************************************************
// 
// 当たり判定形状メッシュコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_COLLISION_SHAPE_MESH_H_
#define _COMPONENT_COLLISION_SHAPE_MESH_H_

#include "compCollision.h"

// 前方宣言
class CCollisionShapeSpher;
class CCollisionShapeSpherTrigger;
class CCollisionCapsuleTrigger;

// 当たり判定形状クラス
class CCollisionShapeMesh : public CCollision
{
public:

	// コンストラクタ
	CCollisionShapeMesh(LPD3DXMESH pMesh, D3DXMATRIX* pMtx = nullptr):
		m_pMesh(pMesh),
		m_pMtx(pMtx)
	{
		m_posHit = InitValu::VEC3_NULL;	// 当たっている場所
		m_bCollision = false;			// 当たっているか否か
	}

	// デストラクタ
	~CCollisionShapeMesh()
	{}

	void Init();
	void Uninit() { delete this; }
	void Update();
	void Draw() {}
	void Dispatch(CCollision& rCollision);
	void Collision(CCollisionShapeMesh* pCollMesh);
	void Collision(CCollisionShapeSpher* pSpher);
	void Collision(CCollisionShapeSpherTrigger* pSpher) {}
	void Collision(CCollisionCapsuleTrigger* pCapsule) {}

	// ゲッター
	LPD3DXMESH GetMesh(void) { return m_pMesh; }
	D3DXMATRIX* GetMtx(void){return m_pMtx;}

	// メンバー変数
	LPD3DXMESH m_pMesh;		// メッシュ
	D3DXVECTOR3 m_posHit;	// 当たっている場所
	D3DXMATRIX* m_pMtx;		// マトリックス
	bool m_bCollision;		// 当たっているか否か
};
#endif