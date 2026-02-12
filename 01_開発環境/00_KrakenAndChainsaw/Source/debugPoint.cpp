//*****************************************
// 
// デバッグ用点
// Author Tetsuji Yamamoto
// 
//*****************************************
#include "debugPoint.h"
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "sphere.h"

// 静的メンバー変数宣言
CDebugPoint* CDebugPoint::m_pInstance = nullptr;

//*****************************************
// デバッグ用点コンストラクタ
//*****************************************
CDebugPoint::CDebugPoint()
{
	m_bDisp = true;
	m_bZtest = false;
	m_pSpher = nullptr;
}

//*****************************************
// デバッグ用点デストラクタ
//*****************************************
CDebugPoint::~CDebugPoint()
{
}

//*****************************************
// デバッグ用点初期化
//*****************************************
void CDebugPoint::Init(void)
{
#ifdef _DEBUG
	m_pSpher = CSphere::Create(8, 8, 0.0f, Color::_WHITE,nullptr);
	m_vPoint.reserve(1000);
#endif
}

//*****************************************
// デバッグ用点終了
//*****************************************
void CDebugPoint::Uninit(void)
{
	// 球の破棄
	if (m_pSpher != nullptr)
	{
		m_pSpher->Uninit();
		m_pSpher = nullptr;
	}

	// 位置情報クリア
	m_vPoint.clear();

	delete this;
}

//*****************************************
// デバッグ用点描画
//*****************************************
void CDebugPoint::Draw(void)
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
	

	// 情報分描画する
	for (auto iterPoint = m_vPoint.begin(); iterPoint != m_vPoint.end();)
	{
		m_pSpher->SetMore((*iterPoint).pos, (*iterPoint).col, (*iterPoint).fRadius);
		m_pSpher->DrawWire();

		// タイムが無ければ削除
		if (iterPoint->nDeleteTime <= 0)
		{
			iterPoint = m_vPoint.erase(iterPoint);
			continue;
		}

		// 消す時間を減らす
		iterPoint->nDeleteTime--;

		// 要素を一つずらす
		iterPoint++;
	}
	
	// Zテスト有効
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 小さくしておく
	m_pSpher->SetMore(InitValu::VEC3_NULL, Color::_AQUAMARINE, 0.0f);
#endif
}

//*****************************************
// デバッグ用点追加k処理
//*****************************************
void CDebugPoint::AddPoint(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fRadius, int nDeleteTime)
{
	if (m_bDisp == false)return;

#ifdef _DEBUG
	POINT point;
	point.pos = pos;
	point.col = col;
	point.fRadius = fRadius;
	point.nDeleteTime = nDeleteTime;
	m_vPoint.push_back(point);
#endif // !_DEBUG
}