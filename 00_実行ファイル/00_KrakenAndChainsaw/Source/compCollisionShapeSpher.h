//****************************************************
// 
// 当たり判定形状球コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_COLLISION_SHAPE_SPHER_H_
#define _COMPONENT_COLLISION_SHAPE_SPHER_H_

#include "compCollision.h"

// 前方宣言
class CCollisionShapeMesh;
class CCollisionShapeSpherTrigger;
class CCollisionCapsuleTrigger;

// 当たり判定形状クラス
class CCollisionShapeSpher : public CCollision
{
public:
	// 当たり判定形状球コンストラクタ
	CCollisionShapeSpher( const D3DXVECTOR3 pos, const float fRadius, const  float fPalamAngleNor) :
		m_posOffSet(pos),
		m_fRadius(fRadius),
		m_fPalamAngleNor(fPalamAngleNor)
	{
		m_posCollision = InitValu::VEC3_NULL;		// 位置
		m_posCollisionOld = InitValu::VEC3_NULL;	// 位置
	}

	// 当たり判定形状球デストラクタ
	~CCollisionShapeSpher()
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
	float GetRadius(void) { return m_fRadius; }
	float GetPalamAngleNor(void) { return m_fPalamAngleNor; }

	// メンバー変数
	D3DXVECTOR3 m_posOffSet;								// オフセット位置
	D3DXVECTOR3 m_posCollision;								// 位置
	D3DXVECTOR3 m_posCollisionOld;							// 位置
	float m_fPalamAngleNor;									// 限界傾き
	float m_fRadius;										// 半径
};

#endif