//********************************************************
// 
// 位置ポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compTransfom.h"
#include "renderer.h"
#include "manager.h"
#include "matrix.h"
#include "quaternion.h"

using namespace matrix;
using namespace Quaternion;

//********************************************************
// コンストラクタ
//********************************************************
CTransfom::CTransfom(ArgumentTransfom argment)
{
	m_argument = argment;				// 引数
	m_mtxWorld = InitValu::MTX_NULL;	// ワールドマトリックス
	m_mtxAddRot = InitValu::MTX_NULL;	// 追加の回転マトリックス
}

//********************************************************
// デストラクタ
//********************************************************
CTransfom::~CTransfom()
{

}

//********************************************************
// 初期化
//********************************************************
void CTransfom::Init(void)
{

}

//********************************************************
// 終了
//********************************************************
void CTransfom::Uninit(void)
{
	// 自身の破棄
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CTransfom::Update(void)
{
	m_argument.move.y += m_argument.fGravity;
	m_argument.pos += m_argument.move;

	// マトリックスの作成
	m_mtxWorld = InitValu::MTX_NULL;
	D3DXMATRIX mtxRot = MatrixRotationQuat(m_argument.quat);
	mtxRot = MatrixMultiply(mtxRot,m_mtxAddRot);
	D3DXMATRIX mtxTrans = MatrixTranslation(m_argument.pos);
	m_mtxWorld = MatrixMultiply(m_mtxWorld, mtxRot);
	m_mtxWorld = MatrixMultiply(m_mtxWorld,mtxTrans);
}

//********************************************************
// 描画
//********************************************************
void CTransfom::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// マトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//********************************************************
// クォータニオン掛け合わせ
//********************************************************
void CTransfom::MulQuat(D3DXQUATERNION quat)
{
	m_argument.quat = QuatMultiply(m_argument.quat, quat);
}

//********************************************************
// 回転マトリックスの掛け合わせ
//********************************************************
void CTransfom::MulMtxAddRot(const D3DXMATRIX mtxRot)
{
	m_mtxAddRot = MatrixMultiply(m_mtxAddRot, mtxRot);
}