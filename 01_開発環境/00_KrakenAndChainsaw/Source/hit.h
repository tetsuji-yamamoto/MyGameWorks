//****************************************************
// 
// 当たり判定処理
// Author Tetsuji Yamamoto
// 
//****************************************************

// メッシュと球の衝突結果クラス
class CHitResultMeshAndSpher
{
public:
	CHitResultMeshAndSpher()
	{
		m_pos = InitValu::VEC3_NULL;
		m_nor = InitValu::VEC3_NULL;
		m_bCollision = false;
		m_bRanding = false;
	}

	~CHitResultMeshAndSpher() {}

	void Reset(void)
	{
		m_pos = InitValu::VEC3_NULL;
		m_nor = InitValu::VEC3_NULL;
		m_bCollision = false;
		m_bRanding = false;
	}

	void Set(CHitResultMeshAndSpher result)
	{
		if (result.m_bCollision == true)m_bCollision = result.m_bCollision;
		if (result.m_bRanding == true)m_bRanding = result.m_bRanding;
		m_nor = result.m_nor;
		m_pos = result.m_pos;
	}

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_nor;
	bool m_bCollision;
	bool m_bRanding;
};

// 当たり判定名前空間
namespace hit
{
	float AreaTriangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC);
	bool PolygonInsideOutside(float* pDiff, D3DXVECTOR3 posInter, D3DXVECTOR3 posPolyA, D3DXVECTOR3 posPolyB, D3DXVECTOR3 posPolyC);
	bool CollisionSpherAndLine(D3DXVECTOR3* pOut, D3DXVECTOR3 posSpher, float fRadius, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd);
	bool CollisionMeshAndSpher(D3DXVECTOR3* pOutHitPos, D3DXVECTOR3* pOutNor, LPD3DXMESH pMesh, D3DXMATRIX* pMtx, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius = 10.0f);
	bool CollisionMeshAndSpherTrigger(D3DXVECTOR3* pOutHitPos, LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius = 10.0f);
	CHitResultMeshAndSpher CollisionMeshAndSpher(const LPD3DXMESH& pMesh, D3DXMATRIX* pMtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld, float fRadius, float fMovement, float fAngleNorLimit = D3DX_PI * 0.5f);
	bool CollisionSpherAndCapsuleTrigger(D3DXVECTOR3* pOut, D3DXVECTOR3 posSpher, float fRadius, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd);
	bool CollisionCapsuleAndCapsuleTrigger(D3DXVECTOR3& posHit, D3DXVECTOR3& vecHit, D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2, float fRadiusA, float fRadiusB);
}