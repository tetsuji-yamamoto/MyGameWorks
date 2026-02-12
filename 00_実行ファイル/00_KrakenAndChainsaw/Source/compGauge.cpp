//******************************************************************
// 
// ゲージコンポーネント
// Author Tetsuji Yamamoto
// 
//******************************************************************
#include "compGauge.h"
#include "porygon2D.h"
#include "myMath.h"

using namespace my_math;

//******************************************************************
// コンストラクタ
//******************************************************************
CGauge::CGauge()
{
	m_ppolygon = nullptr;	// ポリゴン
	m_nMax = 0;				// 最大値
	m_nNow = 0;				// 現在の値
}

//******************************************************************
// コンストラクタ
//******************************************************************
CGauge::CGauge(const int nMax, const D3DXVECTOR3 pos, const  D3DXVECTOR3 rot, const  D3DXCOLOR col, const  float fWidth, const  float fHeight, const char* pTexturePass)
{
	CPolygon2D::Info poly2D_info;
	poly2D_info.pos = pos;
	poly2D_info.rot = rot;
	poly2D_info.col = col;
	poly2D_info.fHeight = fHeight;
	poly2D_info.fWidth = fWidth;
	m_ppolygon = CPolygon2D::CreateLeftBottom(poly2D_info, pTexturePass);
	m_nMax = nMax;
	m_nNow = nMax;
}

//******************************************************************
// デストラクタ
//******************************************************************
CGauge::~CGauge() 
{
	
}

//******************************************************************
// 初期化
//******************************************************************
void CGauge::Init()
{

}

//******************************************************************
// 終了
//******************************************************************
void CGauge::Uninit()
{
	// ポリゴン削除
	if (m_ppolygon != nullptr)
	{
		m_ppolygon->Uninit();
		m_ppolygon = nullptr;
	}

	// 地震の削除
	delete this;
}

//******************************************************************
// 更新
//******************************************************************
void CGauge::Update()
{
	// ポリゴン更新
	if (m_ppolygon != nullptr)
	{
		// バイ率計算
		float fPalam = Palam(m_nNow, m_nMax);

		// 幅をスケール
		m_ppolygon->ScaleLeftBottomWidth(fPalam);
	}
}

//******************************************************************
// 描画
//******************************************************************
void CGauge::Draw()
{
	// ポリゴン描画
	if (m_ppolygon != nullptr)
	{
		m_ppolygon->Draw();
	}
}