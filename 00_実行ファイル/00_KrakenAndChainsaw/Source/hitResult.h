//**************************************************************
// 
// 衝突結果クラス
// Author Tetsuji Yamamoto
// 
//**************************************************************
#ifndef _HITRESULT_H_
#define _HITRESULT_H_

// 衝突結果クラス
class CHitResult
{
public:
	CHitResult()
	{
		m_nHitCnt = 0;
		m_bCollision = false;
		m_bRanding = false;
		m_bRandingOld = false;
		m_pMtxRanding = nullptr;
		m_mtxRanding = InitValu::MTX_NULL;
		m_posHit = InitValu::VEC3_NULL;
		m_posSave = InitValu::VEC3_NULL;
		m_moveHit = InitValu::VEC3_NULL;
		m_vecPush = InitValu::VEC3_NULL;
		m_nor = InitValu::VEC3_NULL;
	}
	~CHitResult() {}

	void Reset(void)
	{
		m_nHitCnt = 0;
		m_bCollision = false;
		m_bRanding = false;
		m_bRandingOld = false;
		m_pMtxRanding = nullptr;
		m_mtxRanding = InitValu::MTX_NULL;
		m_posHit = InitValu::VEC3_NULL;
		m_posSave = InitValu::VEC3_NULL;
		m_moveHit = InitValu::VEC3_NULL;
		m_vecPush = InitValu::VEC3_NULL;
		m_nor = InitValu::VEC3_NULL;
	}

	int m_nHitCnt;				// ヒットカウント
	bool m_bCollision;			// 当たったかの有無
	bool m_bRanding;			// 乗っているかどうかの有無
	bool m_bRandingOld;			// 乗っているかどうかの有無
	D3DXMATRIX* m_pMtxRanding;	// 乗っているもののマトリックス
	D3DXMATRIX m_mtxRanding;	// 乗っているもののマトリックス
	D3DXMATRIX m_mtxRandingOld;	// 乗っているもののマトリックス
	D3DXVECTOR3 m_posHit;		// 当たり判定用位置
	D3DXVECTOR3 m_posSave;		// 当たり判定用保存位置
	D3DXVECTOR3 m_moveHit;		// 当たり判定用移動量
	D3DXVECTOR3 m_vecPush;		// 押し出すベクトル
	D3DXVECTOR3 m_nor;			// 法線ベクトル
};
#endif