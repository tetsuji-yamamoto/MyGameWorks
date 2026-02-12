//********************************************************
// 
// 当たり判定前コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compHitBefore.h"
#include "compInfo.h"
#include "object.h"
#include "vector.h"
#include "matrix.h"

using namespace my_vector;
using namespace matrix;

//********************************************************
// コンストラクタ
//********************************************************
CHitBefore::CHitBefore()
{

}

//********************************************************
// デストラクタ
//********************************************************
CHitBefore::~CHitBefore()
{

}

//********************************************************
// 初期化
//********************************************************
void CHitBefore::Init()
{

}

//********************************************************
// 終了
//********************************************************
void CHitBefore::Uninit() 
{
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CHitBefore::Update()
{
	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 無ければ処理しない
	if (pInfo == nullptr)return;

	// 乗っている物のマトリックスがあれば
	if (pInfo->hitResult.m_pMtxRanding != nullptr)
	{
		// 位置の移動量取得
		D3DXVECTOR3 vecPos = pInfo->pos;
		
		// 追従させる
		pInfo->pos = Vec3MulMtx(pInfo->posInverse, *pInfo->hitResult.m_pMtxRanding);

		// ベクトル取得
		vecPos = pInfo->pos - vecPos;

		// 古い位置を動かす
		pInfo->posOld += vecPos;

		// ワールドマトリックスの作成
		pInfo->mtxWorld = MatrixWorld(pInfo->pos, pInfo->rot);
	}
}

//********************************************************
// 描画
//********************************************************
void CHitBefore::Draw()
{

}
