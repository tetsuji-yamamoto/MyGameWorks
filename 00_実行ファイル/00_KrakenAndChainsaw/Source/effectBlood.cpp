//*********************************************************************
// 
// 血エフェクト
// Author Tetsuji Yamamoto
// 
//*********************************************************************
#include "effectBlood.h"
#include "compPolygon3D.h"
#include "compTransfom.h"
#include "compLife.h"
#include "compTexture.h"
#include "camera.h"
#include "matrix.h"
#include "quaternion.h"
#include "vector.h"
#include "myMath.h"

using namespace my_math;
using namespace matrix;
using namespace my_vector;
using namespace Quaternion;

//*********************************************************************
// コンストラクタ
//*********************************************************************
CEffectBlood::CEffectBlood()
{

}

//*********************************************************************
// デストラクタ
//*********************************************************************
CEffectBlood::~CEffectBlood()
{

}

//*********************************************************************
// 初期化
//*********************************************************************
void CEffectBlood::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fGravity, int nLife)
{
	CTransfom::ArgumentTransfom argTransform;
	CLife::LifeArgument atgLife;
	CTextures::TexturesArgument argTextures;
	CPolygon3D::Polygon3DArgument argupolygon;

	argTransform.pos = pos;
	argTransform.move = move;
	argTransform.fGravity = fGravity;

	atgLife.nLife = nLife;
	atgLife.nMax = nLife;
	atgLife.nMin = 0;

	argupolygon.col = Color::_BLUE;
	argupolygon.fDips = 0.0f;
	argupolygon.fHeight = 100.0f;

	argTextures.texturePath = "data\\texture\\blood_001.png";

	AddComponent<CTransfom>(argTransform);	// トランスフォーム
	AddComponent<CLife>(atgLife);			// 寿命
	AddComponent<CTextures>(argTextures);	// テクスチャ
	AddComponent<CPolygon3D>(argupolygon);	// ポリゴン
}

//*********************************************s************************
// 初期化
//*********************************************************************
HRESULT CEffectBlood::Init(void)
{
	return S_OK;
}

//*********************************************************************
// 終了
//*********************************************************************
void CEffectBlood::Uninit(void)
{
	// コンポーネント破棄
	CObject::Release();
}

//*********************************************************************
// 更新
//*********************************************************************
void CEffectBlood::Update(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 vecCamera = pCamera->GetPositionV() - pCamera->GetPositionR();

	// トランスフォーム取得
	CTransfom* pTransform = GetComponent<CTransfom>();

	// 体力取得
	CLife* pLife = GetComponent<CLife>();

	// 寿命を減らす
	pLife->AddLife(-1);

	// ゼロになったら削除
	if (pLife->GetIsMin() == true)
	{
		Uninit();
		return;
	}

	// 移動量
	D3DXVECTOR3 move = pTransform->GetMove();

	// 移動角度求める
	float fAngle = AngleVec3AndVec3(move, InitValu::VEC3_Y);

	// 移動ベクトルからクォータニオンを作成
	D3DXQUATERNION quatMove = QuatRotationAxis(vecCamera, fAngle);

	// マトリックスに変換
	D3DXMATRIX mtxQuat = MatrixRotationQuat(quatMove);

	// カメラの逆行列取得
	D3DXMATRIX mtxViewInvers = pCamera->GetMtxViewInvers();

	// カメラの逆行列設定
	GetComponent<CTransfom>()->SetMtxAddRot(InitValu::MTX_NULL);
	GetComponent<CTransfom>()->MulMtxAddRot(mtxViewInvers);
	GetComponent<CTransfom>()->MulMtxAddRot(mtxQuat);

	// コンポーネント更新
	CObject::UpdateComponent();
}

//*********************************************************************
// 描画
//*********************************************************************
void CEffectBlood::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	////αブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// コンポーネント描画
	CObject::DrawComponent();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//*********************************************************************
// 生成
//*********************************************************************
CEffectBlood* CEffectBlood::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fGravity, int nLife)
{
	// インスタンス生成
	CEffectBlood* pEffectBlood = new CEffectBlood;

	// 初期化
	if (pEffectBlood != nullptr)
	{
		pEffectBlood->Init(pos, move, fGravity, nLife);
	}

	// 返す
	return pEffectBlood;
}

//*********************************************************************
// 生成
//*********************************************************************
void CEffectBlood::Particle(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fRadiusRandm, float fGravity, int nLife, int nAmount)
{
	for (int nCnt = 0; nCnt < nAmount; nCnt++)
	{
		D3DXVECTOR3 vec;
		vec.x = move.x + Random(fRadiusRandm) - Random(fRadiusRandm);
		vec.y = move.y + Random(fRadiusRandm) - Random(fRadiusRandm);
		vec.z = move.z + Random(fRadiusRandm) - Random(fRadiusRandm);

		int nLifeR = Random(nLife, nLife / 2);

		// 生成
		Create(pos, vec, fGravity, nLifeR);
	}
}