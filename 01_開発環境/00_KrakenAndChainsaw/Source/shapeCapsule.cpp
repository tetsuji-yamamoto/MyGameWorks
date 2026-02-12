//**************************************
// 
// カプセル形状
// Author Tetsuji Yamamoto
// 
//**************************************
#include "shapeCapsule.h"
#include "texture.h"
#include "manager.h"
#include "shapeSpher.h"
#include "shapeSylinder.h"
#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

using namespace my_vector;
using namespace matrix;
using namespace Quaternion;

//**************************************
// カプセル形状コンストラクタ
//**************************************
CShapeCapsule::CShapeCapsule()
{
	m_pSylinder = nullptr;
	m_pSpherA = nullptr;
	m_pSpherB = nullptr;
	m_mtxWorld = InitValu::MTX_NULL;
	m_mtxParent = InitValu::MTX_NULL;
	m_quat = InitValu::QUAT_NULL;
	m_posA = InitValu::VEC3_NULL;
	m_posB = InitValu::VEC3_NULL;
	m_pos = InitValu::VEC3_NULL;
}

//**************************************
// カプセル形状デストラクタ
//**************************************
CShapeCapsule::~CShapeCapsule()
{
}

//**************************************
// カプセル形状初期化
//**************************************
HRESULT CShapeCapsule::Init(const D3DXCOLOR col, const int nDivisionX, const float fRadius, const float fHeight)
{
	// 分割数がゼロ以下にならないようにする
	int nDiviX = nDivisionX;
	if (nDivisionX <= 3)
	{
		nDiviX = 3;
	}

	// 位置設定
	m_posA.y = fHeight * 0.5f;
	m_posB.y = -fHeight * 0.5f;

	m_pSylinder = CShapeSylinder::Create(col, nDivisionX, 2, fRadius, fHeight);
	m_pSpherA = CShapeSpher::Create(col, nDivisionX, 8, fRadius);
	m_pSpherB = CShapeSpher::Create(col, nDivisionX, 8, fRadius);

	return S_OK;
}

//**************************************
// カプセル形状終了
//**************************************
void CShapeCapsule::Uninit(void)
{
	if (m_pSpherA != nullptr)
	{
		m_pSpherA->Uninit();
		m_pSpherA = nullptr;
	}

	if (m_pSpherB != nullptr)
	{
		m_pSpherB->Uninit();
		m_pSpherB = nullptr;
	}

	if (m_pSylinder != nullptr)
	{
		m_pSylinder->Uninit();
		m_pSylinder = nullptr;
	}

	// 自分のオブジェクトを破棄
	Release();
}

//**************************************
// カプセル形状更新
//**************************************
void CShapeCapsule::Update(void)
{
	// 長さを計算
	D3DXVECTOR3 axis = m_posA - m_posB;
	float fHeight = Vec3Length(axis);

	// 角度を求めて
	float fAngle = AngleVec3AndVec3(D3DXVECTOR3(0.0f,1.0f,0.0f),axis);

	// クォータニオンで回転マトリックス作成
	D3DXMATRIX mtxRot = MatrixQuatAxis(axis, fAngle);
	
	// 位置マトリックス作成
	D3DXMATRIX mtxTransA = MatrixTranslation(m_posA);
	D3DXMATRIX mtxTransB = MatrixTranslation(m_posB);
	
	// ワールドマトリックス作成
	m_mtxWorld = MatrixWorld(m_pos,m_quat);
	m_mtxWorld = MatrixMultiply(m_mtxWorld,m_mtxParent);
	D3DXMATRIX mtxWorldA = MatrixMultiply(mtxTransA,mtxRot);
	D3DXMATRIX mtxWorldB = MatrixMultiply(mtxTransB,mtxRot);
	mtxWorldA = MatrixMultiply(mtxTransA, m_mtxWorld);
	mtxWorldB = MatrixMultiply(mtxTransB, m_mtxWorld);

	// マトリックスの設定
	m_pSylinder->SetMatrix(m_mtxWorld);
	m_pSpherA->SetMatrix(mtxWorldA);
	m_pSpherB->SetMatrix(mtxWorldB);
}

//**************************************
// カプセル形状描画
//**************************************
void CShapeCapsule::Draw(void)
{
	m_pSpherA->Draw();
	m_pSpherB->Draw();
	m_pSylinder->Draw();
}

//**************************************
// カプセル形状描画
//**************************************
void CShapeCapsule::DrawWire(void)
{
	m_pSpherA->DrawWire();
	m_pSpherB->DrawWire();
	m_pSylinder->DrawWire();
}

//**************************************
// カプセル形状生成
//**************************************
CShapeCapsule* CShapeCapsule::Create(const D3DXCOLOR col, const int nDivisionX, const float fRadius, const float fHeight)
{
	// カプセル形状インスタンス生成
	CShapeCapsule* pSphere = new CShapeCapsule;

	if (pSphere != nullptr)
	{
		// 初期化処理
		pSphere->Init(col,nDivisionX, fRadius,fHeight);
	}
	else
	{
		MessageBox(NULL, " CShapeCapsule::Create() > if (pPlayer != nullptr) メモリを確保できませんでした", "Error", MB_OK);
	}

	return pSphere;
}

//**************************************
// カプセル形状設定
//**************************************
void CShapeCapsule::SetMore(const D3DXVECTOR3 pos, const D3DXQUATERNION quat, const D3DXCOLOR col, const D3DXMATRIX mtx, const float fRadius, const float fHeight)
{
	// 位置設定
	m_pos = pos;

	// 向き設定
	m_quat = quat;

	// マトリックス設定
	m_mtxParent = mtx;

	// 位置設定
	m_posA.y = fHeight * 0.5f;
	m_posB.y = -fHeight * 0.5f;

	// 形状修正
	m_pSpherA->SetMore(col,fRadius);
	m_pSpherB->SetMore(col,fRadius);
	m_pSylinder->SetMore(col,fRadius,fHeight);
}