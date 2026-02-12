//*****************************************
// 
// デバッグ用カプセル
// Author Tetsuji Yamamoto
// 
//*****************************************
#include "debugCapsule.h"
#include "shapeCapsule.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバー変数宣言
CDebugCapsule* CDebugCapsule::m_pInstance = nullptr;

//*****************************************
// デバッグ用カプセルコンストラクタ
//*****************************************
CDebugCapsule::CDebugCapsule()
{
	m_bDisp = true;
	m_bZtest = false;
	m_bWire = true;
	m_pCapsule = nullptr;
}

//*****************************************
// デバッグ用カプセルデストラクタ
//*****************************************
CDebugCapsule::~CDebugCapsule()
{
}

//*****************************************
// デバッグ用カプセル初期化
//*****************************************
void CDebugCapsule::Init(void)
{
#ifdef _DEBUG
	m_pCapsule = CShapeCapsule::Create(Color::_NULL, 8, 10, 50);
	m_vCapsule.reserve(1000);
#endif
}

//*****************************************
// デバッグ用カプセル終了
//*****************************************
void CDebugCapsule::Uninit(void)
{
	if (m_pCapsule != nullptr)
	{
		m_pCapsule = nullptr;
	}

	// 位置情報クリア
	m_vCapsule.clear();

	delete this;
}

//*****************************************
// デバッグ用カプセル描画
//*****************************************
void CDebugCapsule::Draw(void)
{
#ifdef _DEBUG
	// 移さなければ処理しない
	if (m_bDisp == false)return;

	// Zテストを無効にする
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// Zテスト無効
	if (m_bZtest == false)
	{
		// Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//// ワイヤー表示
	//if (m_bWire == true)
	//{
	//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//}

	// 情報分描画する
	for (auto iterCapsule = m_vCapsule.begin(); iterCapsule != m_vCapsule.end();)
	{
		// 形状設定描画
		m_pCapsule->SetMore((*iterCapsule).pos, (*iterCapsule).quat, (*iterCapsule).col, (*iterCapsule).mtx, (*iterCapsule).fRadius, (*iterCapsule).fHeight);
		m_pCapsule->Update();

		// ワイヤー表示
		if (m_bWire == true)
		{
			m_pCapsule->DrawWire();
		}
		else
		{
			m_pCapsule->Draw();
		}

		// タイムが無ければ削除
		if (iterCapsule->nDeleteTime <= 0)
		{
			iterCapsule = m_vCapsule.erase(iterCapsule);
			continue;
		}

		// 消す時間を減らす
		iterCapsule->nDeleteTime--;

		// 要素を一つずらす
		iterCapsule++;
	}

	// Zテスト有効
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//// ワイヤー表示オフ
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	// 小さくしておく
	m_pCapsule->SetMore(InitValu::VEC3_NULL, InitValu::QUAT_NULL,Color::_NULL,  InitValu::MTX_NULL, 0.0f, 0.0f);

#endif
}

//*****************************************
// デバッグ用カプセル追加処理
//*****************************************
void CDebugCapsule::AddCapsule(const D3DXVECTOR3 posOffset, const D3DXQUATERNION quat,const D3DXCOLOR col, const D3DXMATRIX mtxPerent, const float fHeight, const float fRadius, const int nDeleteTime)
{
	// うつさなければ処理しない
	if (m_bDisp == false)return;

#ifdef _DEBUG
	CAPSULE capsule;
	capsule.quat = quat;
	capsule.pos = posOffset;
	capsule.col = col;
	capsule.mtx = mtxPerent;
	capsule.fRadius = fRadius;
	capsule.fHeight = fHeight;
	capsule.nDeleteTime = nDeleteTime;
	m_vCapsule.push_back(capsule);
#endif // !_DEBUG
}
