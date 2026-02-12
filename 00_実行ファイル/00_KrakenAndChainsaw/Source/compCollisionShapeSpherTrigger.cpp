//****************************************************
// 
// 当たり判定形状球コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compCollisionShapeSpherTrigger.h"
#include "compCollisionShapeMesh.h"
#include "hit.h"
#include "matrix.h"
#include "vector.h"
#include "object.h"
#include "debugPoint.h"

using namespace hit;
using namespace matrix;
using namespace my_vector;
using namespace Color;

//****************************************************
// 初期化
//****************************************************
void CCollisionShapeSpherTrigger::Init()
{
	// マトリックスが存在していれば
	if (m_pMatrixParent != nullptr)
	{
		// オフセットに親マトリックスを掛けた位置を求める(初期化)
		m_posCollision = Vec3MulMtx(m_posOffSet, *m_pMatrixParent);
		m_posCollisionOld = m_posCollision;
	}
}

//****************************************************
// 更新
//****************************************************
void CCollisionShapeSpherTrigger::Update()
{
	// 当たっていない状態にする
	m_bCollision = false;

	// 古い位置設定
	m_posCollisionOld = m_posCollision;

	// マトリックスが存在していれば
	if (m_pMatrixParent != nullptr)
	{
		// オフセットに親マトリックスを掛けた位置を求める
		m_posCollision = Vec3MulMtx(m_posOffSet, *m_pMatrixParent);
	}

	// 当たったオブジェクトの名前を消す
	ClearHitedName();
	ClearHitedOwner();

#ifdef _DEBUG
	// デバッグ表示
	CDebugPoint::GetInstance()->AddPoint(m_posCollisionOld, _MEDIUMNIOLETED,m_fRadius);
	CDebugPoint::GetInstance()->AddPoint(m_posCollision, _MAGENTA,m_fRadius);
#endif //  _DEBU
}

//****************************************************
// ディスパッチ(送る)
//****************************************************
void CCollisionShapeSpherTrigger::Dispatch(CCollision& rCollision)
{
	rCollision.Collision(this);
}

//****************************************************
// 当たり判定(球とメッシュ)
//****************************************************
void CCollisionShapeSpherTrigger::Collision(CCollisionShapeMesh* pCollMesh)
{
	// 当たり判定用変数
	D3DXVECTOR3 pos = m_posCollision;
	D3DXVECTOR3 posHit;
	D3DXVECTOR3 posOld = m_posCollisionOld;

	// 当たっていたら
	if (CollisionMeshAndSpherTrigger(&posHit, pCollMesh->GetMesh(), pos, posOld, m_fRadius) == true)
	{
		// メッシュコライダーに当たった位置設定
		pCollMesh->m_posHit = posHit;

		// 当たった向き計算
		m_vecHit = Vec3Normalize(pos - posHit);

		// メッシュコライダー当たったと設定
		pCollMesh->m_bCollision = true;

		// 当たった位置設定
		m_posHit = posHit;

		// 当たったフラグを立てる
		m_bCollision = true;

		// 名前リストアップ
		AddHitedName(pCollMesh->GetName());
		AddHitedOwner(pCollMesh->GetOwner());
		pCollMesh->AddHitedName(GetName());
		pCollMesh->AddHitedOwner(GetOwner());
#ifdef _DEBUG
		// デバッグ表示
		CDebugPoint::GetInstance()->AddPoint(m_posHit, Color::_RED, m_fRadius);
#endif //  _DEBU
	}
}