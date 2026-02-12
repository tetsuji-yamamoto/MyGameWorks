//**************************************************
// 
// タイトルロゴ
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "titleLogo.h"
#include "porygon2D.h"

//**************************************************
// タイトルロゴデストラクタ
//**************************************************
CTitleLogo::CTitleLogo()
{
	m_pPolygon2D = nullptr;		// 2Dポリゴン
}

//**************************************************
// タイトルロゴコンストラクタ
//**************************************************
CTitleLogo::~CTitleLogo()
{

}

//**************************************************
// タイトルロゴ初期化
//**************************************************
HRESULT CTitleLogo::Init(void)
{
	return S_OK;
}

//**************************************************
// タイトルロゴ初期化
//**************************************************
void CTitleLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	// 2Dポリゴン生成
	if (m_pPolygon2D == nullptr)
	{
		CPolygon2D::Info poly2D_info;
		poly2D_info.pos = pos;
		poly2D_info.rot = rot;
		poly2D_info.col = col;
		poly2D_info.fHeight = fHeight;
		poly2D_info.fWidth = fWidth;
		m_pPolygon2D = CPolygon2D::Create(poly2D_info,pTexturePass);
	}
}

//**************************************************
// タイトルロゴ終了
//**************************************************
void CTitleLogo::Uninit(void)
{
	// 2Dポリゴン破棄
	if (m_pPolygon2D != nullptr)
	{
		m_pPolygon2D->Uninit();
		m_pPolygon2D = nullptr;
	}

	// 自身の破棄
	Release();
}

//**************************************************
// タイトルロゴ更新
//**************************************************
void CTitleLogo::Update(void)
{
	// 2Dポリゴン更新
	if (m_pPolygon2D != nullptr)
	{
		m_pPolygon2D->Update();
	}
}

//**************************************************
// タイトルロゴ描画
//**************************************************
void CTitleLogo::Draw(void)
{
	// 2Dポリゴン描画
	if (m_pPolygon2D != nullptr)
	{
		m_pPolygon2D->Draw();
	}
}

//***************************************************************
// タイトルロゴ生成
//***************************************************************
CTitleLogo* CTitleLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	// タイトルロゴインスタンス生成
	CTitleLogo* pTitleLogo = new CTitleLogo;

	if (pTitleLogo != nullptr)
	{
		pTitleLogo->Init(pos, rot, col, fWidth, fHeight, pTexturePass);
	}

	return pTitleLogo;
}