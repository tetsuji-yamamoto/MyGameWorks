//****************************************************
// 
// 当たり判定形状メッシュコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compCollisionShapeMesh.h"
#include "compCollisionShapeSpher.h"

//****************************************************
// ディスパッチ(送る)
//****************************************************
void CCollisionShapeMesh::Dispatch(CCollision& rCollision)
{
	rCollision.Collision(this);
}

//****************************************************
// 当たり判定(メッシュ)初期化
//****************************************************
void CCollisionShapeMesh::Init()
{

}

//****************************************************
// 当たり判定(メッシュ)更新
//****************************************************
void CCollisionShapeMesh::Update()
{
	// 当たっていないことにする
	m_bCollision = false;

	// 当たった相手の名前削除
	ClearHitedName();
	ClearHitedOwner();
}

//****************************************************
// 当たり判定(メッシュ)
//****************************************************
void CCollisionShapeMesh::Collision(CCollisionShapeMesh* pCollMesh)
{
	return;
}

//****************************************************
//  当たり判定(球)
//****************************************************
void CCollisionShapeMesh::Collision(CCollisionShapeSpher* pSpher)
{
	return;
}