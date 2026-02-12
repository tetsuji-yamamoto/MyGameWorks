//****************************************************
// 
// 当たり判定形状カプセルトリガーコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_COLLISION_CAPSULE_TRIGGER_H_
#define _COMPONENT_COLLISION_CAPSULE_TRIGGER_H_

#include "compCollision.h"

// 前方宣言
class CCollisionShapeMesh;
class CCollisionShapeSpher;
class CCollisionShapeSpherTrigger;

// 当たり判定形状クラス
class CCollisionCapsuleTrigger : public CCollision
{
public:
	// 当たり判定形状球コンストラクタ
	CCollisionCapsuleTrigger(const D3DXVECTOR3 posOffset,const D3DXQUATERNION quatOffset,const float fHeight, const float fRadius,D3DXMATRIX* pMatrixParent = nullptr) :
		m_posOffSet(posOffset),
		m_quatOffSet(quatOffset),
		m_fRadius(fRadius),
		m_fHeight(fHeight),
		m_pMatrixParent(pMatrixParent)
	{
		m_matrixWorld = InitValu::MTX_NULL;	// マトリックス
		m_vecHited = InitValu::VEC3_NULL;	// 当たった向き
		m_posOffSetA = InitValu::VEC3_NULL;	// オフセット位置A
		m_posOffSetB = InitValu::VEC3_NULL;	// オフセット位置B
		m_posHited = InitValu::VEC3_NULL;	// 当たった
		m_posHitA = InitValu::VEC3_NULL;	// 当たる位置A
		m_posHitB = InitValu::VEC3_NULL;	// 当たる位置B
		m_bCollision = false;				// 当たったか否か
	}

	// 当たり判定形状球デストラクタ
	~CCollisionCapsuleTrigger()
	{}

	void Init();
	void Uninit() { delete this; }
	void Update();
	void Draw() {}
	void Dispatch(CCollision& rCollision) { rCollision.Collision(this); }
	void Collision(CCollisionShapeMesh* pCollMesh) {}
	void Collision(CCollisionShapeSpher* pSpher) {}
	void Collision(CCollisionShapeSpherTrigger* pSpher);
	void Collision(CCollisionCapsuleTrigger* pCapsule);

	// ゲッター
	float GetRadius(void) { return m_fRadius; }
	bool GetIsCollision(void) { return m_bCollision; }

	// セッター

	// メンバー変数
	D3DXMATRIX* m_pMatrixParent;			// 参照マトリックス
	D3DXMATRIX m_matrixWorld;				// マトリックス
	D3DXVECTOR3 m_posOffSet;				// オフセット位置
	D3DXQUATERNION m_quatOffSet;			// オフセット位置
	D3DXVECTOR3 m_posOffSetA;				// オフセット位置A
	D3DXVECTOR3 m_posOffSetB;				// オフセット位置B
	D3DXVECTOR3 m_vecHited;					// 当たった向き
	D3DXVECTOR3 m_posHited;					// 当たった
	D3DXVECTOR3 m_posHitA;					// 当たる位置A
	D3DXVECTOR3 m_posHitB;					// 当たる位置B
	float m_fRadius;						// 半径
	float m_fHeight;						// 高さ
	bool m_bCollision;						// 当たったか否か
};

#endif