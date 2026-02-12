//****************************************************
// 
// 当たり判定形状球トリガーコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_COLLISION_SHAPE_SPHER_TRIGGER_H_
#define _COMPONENT_COLLISION_SHAPE_SPHER_TRIGGER_H_

#include "compCollision.h"

// 前方宣言
class CCollisionShapeMesh;
class CCollisionShapeSpher;
class CCollisionCapsuleTrigger;

// 当たり判定形状クラス
class CCollisionShapeSpherTrigger : public CCollision
{
public:
	// 当たり判定形状球コンストラクタ
	CCollisionShapeSpherTrigger(D3DXMATRIX* pMatrixParent,const D3DXVECTOR3 posOffSet,const float fRadius) :
		m_pMatrixParent(pMatrixParent),
		m_posOffSet(posOffSet),
		m_fRadius(fRadius)
	{
		m_posCollision = InitValu::VEC3_NULL;
		m_posCollisionOld = InitValu::VEC3_NULL;
		m_bCollision = false;
	}

	// 当たり判定形状球デストラクタ
	~CCollisionShapeSpherTrigger()
	{}

	void Init();
	void Uninit() { delete this; }
	void Update();
	void Draw() {}
	void Dispatch(CCollision& rCollision);
	void Collision(CCollisionShapeMesh* pCollMesh);
	void Collision(CCollisionShapeSpher* pSpher) {}
	void Collision(CCollisionShapeSpherTrigger* pSpher) {}
	void Collision(CCollisionCapsuleTrigger* pCapsule){}

	// ゲッター
	float GetRadius(void) { return m_fRadius; }

	// メンバー変数
	D3DXMATRIX* m_pMatrixParent;	// 参照マトリックス
	D3DXVECTOR3 m_posOffSet;		// オフセット位置
	D3DXVECTOR3 m_posCollision;		// 当たり位置
	D3DXVECTOR3 m_posCollisionOld;	// 古い当たり位置
	D3DXVECTOR3 m_posHit;			// 当たった位置
	D3DXVECTOR3 m_vecHit;			// 当たった向き
	float m_fRadius;				// 半径
	bool m_bCollision;				// 当たったか否か
};

#endif