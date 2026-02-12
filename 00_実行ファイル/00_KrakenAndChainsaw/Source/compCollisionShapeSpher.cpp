//****************************************************
// 
// 当たり判定形状球コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compCollisionShapeSpher.h"
#include "compCollisionShapeMesh.h"
#include "hit.h"
#include "matrix.h"
#include "vector.h"
#include "object.h"
#include "debugPoint.h"
#include "input.h"
#include "compInfo.h"
#include "compCorrection.h"

using namespace hit;
using namespace matrix;
using namespace my_vector;

//****************************************************
// 初期化
//****************************************************
void CCollisionShapeSpher::Init()
{ 

}

//****************************************************
// 更新
//****************************************************
void CCollisionShapeSpher::Update()
{
	// 当たった相手の名前削除
	ClearHitedName();

	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	D3DXMATRIX mtx = MatrixWorld(pInfo->pos, pInfo->rot);
	D3DXMATRIX mtxOld = MatrixWorld(pInfo->posOld, pInfo->rot);

	m_posCollision = Vec3MulMtx(m_posOffSet, mtx);
	m_posCollisionOld = Vec3MulMtx(m_posOffSet, mtxOld);

	CDebugPoint::GetInstance()->AddPoint(m_posCollisionOld, Color::_BLUE,m_fRadius);
	CDebugPoint::GetInstance()->AddPoint(m_posCollision, Color::_AQUAMARINE,m_fRadius);

	ClearHitedName();
	ClearHitedOwner();
}

//****************************************************
// ディスパッチ(送る)
//****************************************************
void CCollisionShapeSpher::Dispatch(CCollision& rCollision)
{
	rCollision.Collision(this);
}

//****************************************************
// 当たり判定(球とメッシュ)
//****************************************************
void CCollisionShapeSpher::Collision(CCollisionShapeMesh* pCollMesh)
{
	// キー取得
	CInputKeyboard* pInputKeyBoard = CInputKeyboard::GetkeyBoard();

	if (pInputKeyBoard->GetPress(DIK_D) == true)
	{
		int a = 0;
	}

	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	CHitResultMeshAndSpher hitResultMS;
	CHitResult hitResult =pInfo->hitResult;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 move = pInfo->hitResult.m_moveHit;
	D3DXVECTOR3 pos = m_posCollision;
	D3DXVECTOR3 posOld = m_posCollisionOld;
	D3DXVECTOR3 posMovede = pos + move;
	D3DXVECTOR3 vecDiff = m_posCollision -pInfo->pos;
	float fMovement = pInfo->fMovement;

	// X成分
	pos.x += move.x;
	hitResultMS = CollisionMeshAndSpher(pCollMesh->GetMesh(), pCollMesh->GetMtx(), pos, posOld, m_fRadius, fMovement, m_fPalamAngleNor);
	if (hitResultMS.m_bCollision == true)hitResult.m_bCollision = true;
	hitResult.m_bRanding = hitResultMS.m_bRanding;
	pos = hitResultMS.m_pos;
	
	// Z成分
	pos.z += move.z;
	hitResultMS = CollisionMeshAndSpher(pCollMesh->GetMesh(), pCollMesh->GetMtx(), pos, posOld, m_fRadius, fMovement, m_fPalamAngleNor);
	if (hitResultMS.m_bCollision == true)hitResult.m_bCollision = true;
	hitResult.m_bRanding = hitResultMS.m_bRanding;
	pos = hitResultMS.m_pos;
	
	// Y成分
	pos.y += move.y;
	hitResultMS = CollisionMeshAndSpher(pCollMesh->GetMesh(), pCollMesh->GetMtx(), pos, posOld, m_fRadius, fMovement, m_fPalamAngleNor);
	if (hitResultMS.m_bCollision == true)hitResult.m_bCollision = true;
	hitResult.m_bRanding = hitResultMS.m_bRanding;
	pos = hitResultMS.m_pos;

	if (hitResultMS.m_bCollision == false)
	{
		int a = 0;
	}
	
	// 当たっていたら
	if (hitResult.m_bCollision == true)
	{
		// 衝突カウンター加算
		hitResult.m_nHitCnt++;

		// 法線設定
		hitResult.m_nor = hitResultMS.m_nor;

		// 押し出し用移動量更新
		hitResult.m_moveHit = pos - m_posCollision;
		
		// 当たった位置保存
		hitResult.m_posSave = pos - vecDiff;

		// 押し出すベクトル追加
		hitResult.m_vecPush += pos - m_posCollision;

		GetOwner()->GetComponent<CInfo>()->pos += hitResult.m_vecPush;
		
		// 衝突結果設定
		pInfo->hitResult = hitResult;
		
		// 乗っていたら
		if (hitResult.m_bRanding == true)
		{
			// 当たったもののマトリックスを返す
			pInfo->hitResult.m_pMtxRanding = &pCollMesh->GetOwner()->GetComponent<CInfo>()->mtxWorld;
			pInfo->hitResult.m_mtxRanding = pCollMesh->GetOwner()->GetComponent<CInfo>()->mtxWorld;
		}

		// 次判定するときは当たっていない前提にする
		pInfo->hitResult.m_bCollision = false;

		// 名前の追加
		AddHitedName(pCollMesh->GetName());
		AddHitedOwner(pCollMesh->GetOwner());
		pCollMesh->AddHitedName(GetName());
		pCollMesh->AddHitedOwner(GetOwner());
	}

	return;
}

//****************************************************
//  当たり判定(球と球)
//****************************************************
void CCollisionShapeSpher::Collision(CCollisionShapeSpher* pSpher)
{
	return;
}