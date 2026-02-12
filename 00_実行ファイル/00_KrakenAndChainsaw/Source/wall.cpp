//**********************************************************************
// 
// 壁
// Author Tetsuji Yamamoto
// 
//**********************************************************************
#include "wall.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"

//**********************************************************************
// 壁コンストラクタ
//**********************************************************************
CWall::CWall() : CObject3D(CObject::PRIORITY_7)
{
	m_posA = InitValu::VEC3_NULL;	// ポリゴンの頂点
	m_posB = InitValu::VEC3_NULL;	// ポリゴンの頂点
	m_posC = InitValu::VEC3_NULL;	// ポリゴンの頂点
}

//**********************************************************************
// 壁デストラクタ
//**********************************************************************
CWall::~CWall()
{
}

//**********************************************************************
// 壁初期化
//**********************************************************************
HRESULT CWall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, const char* pTexturePass)
{
	HRESULT hr = CObject3D::Init(pos, rot, col, fWidth, fHeight, fDepth,pTexturePass);

	// 位置取得
	m_posA = CObject3D::GetVtxPos(0);
	m_posB = CObject3D::GetVtxPos(1);
	m_posC = CObject3D::GetVtxPos(2);

	return hr;
}

//**********************************************************************
// 壁終了
//**********************************************************************
void CWall::Uninit(void)
{
	CObject3D::Uninit();
}

//**********************************************************************
// 壁更新
//**********************************************************************
void CWall::Update(void)
{
	CObject3D::Update();
}

//**********************************************************************
// 壁描画
//**********************************************************************
void CWall::Draw(void)
{
	//// 壁の描画
	//CObject3D::Draw();
	
	// テクスチャのインスタンス取得
	//CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャを設定
	//pDevice->SetTexture(0, CManager::GetInstance()->GetRenderer()->GetTextureMT(1));
	//pDevice->SetTexture(0, NULL);

	CObject3D::Draw();
}

//**********************************************************************
// 壁描画
//**********************************************************************
bool CWall::CollisionWall(D3DXVECTOR3* pPos,D3DXVECTOR3 posOld)
{
	bool bCollision = false;
	D3DXVECTOR3 edge0, edge1, edge2;
	D3DXVECTOR3 vecPosA, vecPosB,vecPosOld;
	D3DXVECTOR3 crossPos,crossPoaOld;
	float fDotA, fDotB;

	// ポリゴンの縁ベクトル計算
	edge0 = m_posB - m_posA;
	edge1 = m_posC - m_posA;
	edge2 = m_posA - m_posB;

	// 位置ベクトル計算
	vecPosA = *pPos - m_posA;
	vecPosB = *pPos - m_posB;

	// 縁ベクトルと位置ベクトルを外積計算(古い位置も計算する)
	D3DXVec3Cross(&crossPos, &edge0, &vecPosA);

	// 縁ベクトル二種類と位置ベクトル二種類の内積を計算する
	fDotA = D3DXVec3Dot(&edge0,&vecPosA);
	fDotB = D3DXVec3Dot(&edge2,&vecPosB);

	// 高さが範囲内であったら壁の上下以内
	if (pPos->y <= m_posA.y && pPos->y >= m_posC.y)
	{
		// 内積結果が両方とも+であれば壁の範囲内
		if (fDotA >= 0 && fDotB >= 0)
		{
			// 外積のY成分がマイナスであって古い位置ベクトルの外積がプラスであれば壁にめり込んだ判定
			if (crossPos.y < 0.0f)
			{
				D3DXVECTOR3 vecMove = posOld - *pPos;

				// 法線を外積で算出
				D3DXVECTOR3 nor;
				D3DXVec3Cross(&nor, &edge0, &edge1);
				D3DXVec3Normalize(&nor, &nor);

				// 移動ベクトルと法線の内積を取る
				float fDot = D3DXVec3Dot(&vecMove, &nor);

				// 法線に内積を掛けて移動量を求める
				D3DXVECTOR3 move = nor * fDot;

				// 位置に移動量を足してあげる
				*pPos += move;

				// 当たった判定にする
				bCollision = true;
			}
		}
	}

	// 結果を返す
	return bCollision;
}

//**********************************************************************
// 壁生成
//**********************************************************************
CWall* CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, const char* pTexturePass)
{
	// 壁の生成
	 CWall* pWall = new CWall;

	if (pWall != nullptr)
	{
		// 初期化処理
		pWall->Init(pos, rot, col, fWidth, fHeight, fDepth, pTexturePass);
	}
	else
	{
		MessageBox(NULL, " CWall::Create() > メモリを確保できませんでした", "Error", MB_OK);
	}

	return pWall;
}
