//**************************************************
// 
// ボタン
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "button.h"
#include "word.h"
#include "porygon2D.h"
#include "manager.h"
#include "fontManager.h"
#include "input.h"
#include "myMath.h"

//**************************************************
// ボタンコンストラクタ
//**************************************************
CButton::CButton() : CObject(PRIORITY_8)
{
	m_info.pos = InitValu::VEC3_NULL;		// 位置
	m_info.rot = InitValu::VEC3_NULL;		// 向き
	m_info.fWidth = 0.0f;					// 幅
	m_info.fHeight = 0.0f;					// 高さ
	m_info.colWord = Color::_FULL;			// 文字通常色
	m_info.colWordEmp = Color::_FULL;		// 文字通常色
	m_info.colPolygon = Color::_FULL;		// ポリゴン通常色
	m_info.colPolygonEmp = Color::_FULL;	// ポリゴン通常色
	pWord = nullptr;						// 文字列クラス
	pPolygon2D = nullptr;					// 2Dポリゴン
	m_bTouch = false;						// 触れているか否か
	m_bPress = false;						// 押しているか否か
}

//**************************************************
// ボタンデストラクタ
//**************************************************
CButton::~CButton()
{
	// 文字列削除
	if (pWord != nullptr)
	{
		pWord->Uninit();
		pWord = nullptr;
	}

	// ポリゴン削除
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Uninit();
		pPolygon2D = nullptr;
	}
}

//**************************************************
// ボタン初期化
//**************************************************
void CButton::Init(INFO info, const char* pStrWord, const char* pTexturePassPolygon, int nFontID, const char* pFontName, DWORD format)
{
	// 情報設定
	m_info = info;

	// 文字列初期化
	if (pWord == nullptr)
	{
		pWord = CWord::Create(info.pos, info.colWord, info.fWidth, info.fHeight, pStrWord, format,nFontID, pFontName);
	}

	// ポリゴン初期化
	if (pPolygon2D == nullptr)
	{
		CPolygon2D::Info poly2D_info;
		poly2D_info.pos = info.pos;
		poly2D_info.rot = info.rot;
		poly2D_info.col = info.colPolygon;
		poly2D_info.fHeight = info.fHeight;
		poly2D_info.fWidth = info.fWidth;
		pPolygon2D = CPolygon2D::Create(poly2D_info,pTexturePassPolygon);
	}
}

//**************************************************
// ボタン終了
//**************************************************
void CButton::Uninit(void)
{
	// 文字列削除
	if (pWord != nullptr)
	{
		pWord->Uninit();
		pWord = nullptr;
	}

	// ポリゴン削除
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Uninit();
		pPolygon2D = nullptr;
	}

	// 自身の破棄
	Release();
}

//**************************************************
// ボタン更新
//**************************************************
void CButton::Update(void)
{
	// マウス情報取得
	CInputMouse*pInputMouse = CManager::GetInstance()->GetInstance()->GetInputMouse();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// 触れている状態であれば色をかえる
	if (m_bTouch == true)
	{
		pWord->SetColor(m_info.colWordEmp);
	}
	// 触れていない状態であれば色をかえる
	else if (m_bTouch == false)
	{
		pWord->SetColor(m_info.colWord);
	}

	// 押している状態であれば色をかえる
	if (m_bPress == true)
	{
		pPolygon2D->SetColor(m_info.colPolygonEmp);
	}
	// 押していない状態であれば色をかえる
	else if (m_bPress == false)
	{
		pPolygon2D->SetColor(m_info.colPolygon);
	}

	// 文字列更新
	if (pWord != nullptr)
	{
		pWord->Update();
	}

	// ポリゴン更新
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Update();
	}
}

//**************************************************
// ボタン描画
//**************************************************
void CButton::Draw(void)
{
	// ポリゴン更新
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Draw();
	}

	// 文字列更新
	if (pWord != nullptr)
	{
		pWord->Draw();
	}
}

//***************************************************************
// ボタン生成
//***************************************************************
CButton* CButton::Create(INFO info, const char* pStrWord, const char* pTexturePassPolygon, int nFontID, const char* pFontName, DWORD format)
{
	// ボタンインスタンス生成
	CButton* pTemplate = new CButton;

	if (pTemplate != nullptr)
	{
		pTemplate->Init(info,pStrWord,pTexturePassPolygon,nFontID,pFontName,format);
	}

	return pTemplate;
}

//***************************************************************
// ボタン生成
//***************************************************************
bool CButton::InspectionMouse(void)
{
	// マウス情報取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInstance()->GetInputMouse();

	// マウスの位置取得
	D3DXVECTOR3 posMouse = pInputMouse->GetPosition();

	// ポリゴンの位置と高さと幅を取得
	D3DXVECTOR3 posPoly = pPolygon2D->GetPosition();
	float fWidthPoly = pPolygon2D->GetWidth();
	float fHeightPoly = pPolygon2D->GetHeight();

	// ポリゴンとマウスのが当たっていれば
	if (CMyMath::ColiisionRectTopoint2D(D3DXVECTOR2(posMouse.x, posMouse.y), D3DXVECTOR2(posPoly.x, posPoly.y), fWidthPoly, fHeightPoly) == true)
	{
		// 触れている状態にする
		m_bTouch = true;

		// 左クリックで押したことにする
		if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
		{
			m_bPress = true;
		}
		// クリックしていなければ押していないことにする
		else
		{
			m_bPress = false;
		}

		return true;
	}

	return false;
}