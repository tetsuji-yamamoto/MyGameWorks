//****************************************************
// 
// 当たり判定形状球コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compCollisionCapsuleTrigger.h"
#include "compCollisionShapeSpherTrigger.h"
#include "hit.h"
#include "compInfo.h"
#include "matrix.h"
#include "vector.h"
#include "object.h"
#include "debugPoint.h"
#include "debugCapsule.h"

using namespace hit;
using namespace matrix;
using namespace my_vector;

//****************************************************
// 初期化
//****************************************************
void CCollisionCapsuleTrigger::Init()
{
	// 親マトリックスが設定されていなければ
	if (m_pMatrixParent == nullptr)
	{
		// オーナー取得
		CObject* pOwner = GetOwner();

		// 情報取得
		CInfo* pInfo = pOwner->GetComponent<CInfo>();

		// 情報が無ければ処理しない
		if (pInfo != nullptr)
		{
			// マトリックス設定
			m_pMatrixParent = &pInfo->mtxWorld;
		}
	}

	// オフセット位置設定
	m_posOffSetA.y = m_fHeight * 0.5f;
	m_posOffSetB.y = -m_fHeight * 0.5f;
}

//****************************************************
// 更新
//****************************************************
void CCollisionCapsuleTrigger::Update()
{
	// 当たっていない状態にする
	m_bCollision = false;

	// 当たった名前を空にする
	ClearHitedName();
	ClearHitedOwner();

	// ワールドマトリックス作成
	m_matrixWorld = MatrixWorld(m_posOffSet, m_quatOffSet);

	// 親マトリックスがあれば処理
	if (m_pMatrixParent != nullptr)
	{
		// 当たる位置を設定
		m_matrixWorld = MatrixMultiply(m_matrixWorld, *m_pMatrixParent);
		m_posHitA = Vec3MulMtx(m_posOffSetA, m_matrixWorld);
		m_posHitB = Vec3MulMtx(m_posOffSetB, m_matrixWorld);

		CDebugPoint::GetInstance()->AddPoint(m_posHitA,Color::_YELLOW);
		CDebugPoint::GetInstance()->AddPoint(m_posHitB,Color::_YELLOW);
	}
	else
	{
		// ワールドマトリックス作成
		m_matrixWorld = MatrixWorld(m_posOffSet, m_quatOffSet);
	}

	// デバッグ表示
	CDebugCapsule::GetInstance()->AddCapsule(m_posOffSet, m_quatOffSet, Color::_LIGHTGREEN, *m_pMatrixParent, m_fHeight, m_fRadius);
}

//****************************************************
// 当たり判定(カプセルと球)
//****************************************************
void CCollisionCapsuleTrigger::Collision(CCollisionShapeSpherTrigger* pSpher)
{
	// 同じ名前であれば処理しない
	if (pSpher->GetName() == GetName())return;

	D3DXVECTOR3 posB1 = pSpher->m_posCollision;
	D3DXVECTOR3 posB2 = pSpher->m_posCollision;
	float fRadiusB = pSpher->m_fRadius;

	// 当たっていたら
	if (CollisionCapsuleAndCapsuleTrigger(m_posHited, m_vecHited, m_posHitA, m_posHitB, posB1, posB2, m_fRadius, fRadiusB) == true)
	{
		// 当たっているフラグを立てる
		m_bCollision = true;

		// 互いに名前追加
		AddHitedName(pSpher->GetName());
		AddHitedOwner(pSpher->GetOwner());
		pSpher->AddHitedName(GetName());
		pSpher->AddHitedOwner(GetOwner());

		// デバッグ表示
		CDebugPoint::GetInstance()->AddPoint(m_posHited, Color::_BLUEVIOLET);
		CDebugCapsule::GetInstance()->AddCapsule(m_posOffSet, m_quatOffSet, Color::_CORAL, *m_pMatrixParent, m_fHeight, m_fRadius);
	}
}

//****************************************************
// 当たり判定(カプセルとカプセル)
//****************************************************
void CCollisionCapsuleTrigger::Collision(CCollisionCapsuleTrigger* pCapsule)
{
	// 同じ名前であれば処理しない
	if (pCapsule->GetName() == GetName())return;

	// 当たっていたら
	if (CollisionCapsuleAndCapsuleTrigger(m_posHited,m_vecHited,m_posHitA, m_posHitB, pCapsule->m_posHitA, pCapsule->m_posHitB, m_fRadius, pCapsule->m_fRadius) == true)
	{
		// 当たっているフラグを立てる
		m_bCollision = true;

		// 名前追加
		AddHitedName(pCapsule->GetName());
		AddHitedOwner(pCapsule->GetOwner());

		// デバッグ表示
		CDebugPoint::GetInstance()->AddPoint(m_posHited,Color::_BLUEVIOLET);
		CDebugCapsule::GetInstance()->AddCapsule(m_posOffSet, m_quatOffSet, Color::_CORAL,*m_pMatrixParent,m_fHeight,m_fRadius);
	}
}