//****************************************************
// 
// 情報修正コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compCorrection.h"
#include "object.h"
#include "compMovement.h"
#include "compCollision.h"
#include "vector.h"
#include "matrix.h"

using namespace matrix;
using namespace my_vector;

//****************************************************
// コンストラクタ
//****************************************************
CCorrection::CCorrection()
{
	m_pMovement = nullptr;
}

//****************************************************
// デストラクタ
//****************************************************
CCorrection::~CCorrection()
{

}

//****************************************************
// 初期化
//****************************************************
void CCorrection::Init()
{

}

//****************************************************
// 破棄
//****************************************************
void CCorrection::Uninit()
{
	delete this;
}

//****************************************************
// 更新
//****************************************************
void CCorrection::Update()
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	// 一回は当たっている
	if (pInfo->hitResult.m_nHitCnt > 0)
	{
		// 乗っている状態ならば
		if (pInfo->hitResult.m_bRanding == true)
		{
			// Yの移動量リセット
			pInfo->move.y = 0.0f;
		}

		// 位置を押し出した位置に設定
		//pInfo->pos = pInfo->hitResult.m_posSave;
		//pInfo->pos += pInfo->hitResult.m_vecPush;

		// 乗っている物のマトリックスがあれば
		if (pInfo->hitResult.m_pMtxRanding != nullptr)
		{
			// 逆行列を作成
			D3DXMATRIX mtxInvers = MatrixInverse(*pInfo->hitResult.m_pMtxRanding);

			// それを掛けえた位置を作成
			pInfo->posInverse = Vec3MulMtx(pInfo->pos, mtxInvers);
		}
	}
	else
	{
		pInfo->hitResult.m_pMtxRanding = nullptr;
	}

	// 押し出しベクトルゼロ
	pInfo->hitResult.m_vecPush = InitValu::VEC3_NULL;

	// 情報リセット(乗りフラグはそのまま)
	D3DXMATRIX* pMtx = pInfo->hitResult.m_pMtxRanding;
	bool bRanding = pInfo->hitResult.m_bRanding;
	pInfo->hitResult.Reset();
	pInfo->hitResult.m_pMtxRanding = pMtx;
	pInfo->hitResult.m_bRanding = bRanding;
}

//****************************************************
// 描画
//****************************************************
void CCorrection::Draw()
{

}