//**************************************************
// 
// リザルトランク
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "resultRank.h"
#include "word.h"
#include "porygon2D.h"
#include "manager.h"
#include "fontManager.h"
#include "input.h"
#include "myMath.h"

//**************************************************
// リザルトランクデストラクタ
//**************************************************
CResultRank::CResultRank()
{
	m_info.pos = InitValu::VEC3_NULL;			// 位置
	m_info.rot = InitValu::VEC3_NULL;			// 向き
	m_info.fWidth = 0.0f;					// 幅
	m_info.fHeight = 0.0f;					// 高さ
	m_info.colWord = Color::_FULL;		// 文字通常色
	m_info.colPolygon = Color::_FULL;	// ポリゴン通常色
	m_pWord = nullptr;						// 文字列クラス
	m_pPolygon2D = nullptr;					// 2Dポリゴン
	m_bBlink = false;						// 点滅の有無
	m_nCntBlink = INTERVAL;					// 点滅の間隔
}

//**************************************************
// リザルトランクコンストラクタ
//**************************************************
CResultRank::~CResultRank()
{

}

//**************************************************
// リザルトランク初期化
//**************************************************
void CResultRank::Init(INFO info, const char* pTexturePassPolygon, DWORD format, int nFontID, const char* pFontName)
{
	// 情報設定
	m_info = info;

	// 文字列初期化
	if (m_pWord == nullptr)
	{
		m_pWord = CWord::Create(info.pos, info.colWord, info.fWidth, info.fHeight, info.strWord.c_str(), format, nFontID, pFontName);
	}

	// ポリゴン初期化
	if (m_pPolygon2D == nullptr)
	{
		CPolygon2D::Info poly2D_info;
		poly2D_info.pos = info.pos;
		poly2D_info.rot = info.rot;
		poly2D_info.col = info.colPolygon;
		poly2D_info.fHeight = info.fHeight;
		poly2D_info.fWidth = info.fWidth;
		m_pPolygon2D = CPolygon2D::Create(poly2D_info, pTexturePassPolygon);
	}
}

//**************************************************
// リザルトランク終了
//**************************************************
void CResultRank::Uninit(void)
{
	CObject::UninitComponent();

	// 文字列削除
	if (m_pWord != nullptr)
	{
		m_pWord->Uninit();
		m_pWord = nullptr;
	}

	// ポリゴン削除
	if (m_pPolygon2D != nullptr)
	{
		m_pPolygon2D->Uninit();
		m_pPolygon2D = nullptr;
	}

	// 自身の破棄
	Release();
}

//**************************************************
// リザルトランク更新
//**************************************************
void CResultRank::Update(void)
{
	// 文字列更新
	if (m_pWord != nullptr)
	{
		// 点滅するのであれば色を変える
		if (m_bBlink == true)
		{
			// ゼロ以上で通常色
			if (m_nCntBlink >= 0)
			{
				// 点滅カウンターカウントダウン
				CMyMath::CountDown(&m_nCntBlink, -INTERVAL);

				// 色を設定
				m_pWord->SetColor(m_info.colWord);
			}
			// マイナス域で強調色
			else if (m_nCntBlink < 0)
			{
				// 点滅カウンターカウントダウン
				if (CMyMath::CountDown(&m_nCntBlink, -INTERVAL) == true)
				{
					// カウンターセット
					m_nCntBlink = INTERVAL;
				}

				// 色を設定
				m_pWord->SetColor(m_info.colWordEmp);
			}
		}

		m_pWord->Update();
	}

	// ポリゴン更新
	if (m_pPolygon2D != nullptr)
	{
		m_pPolygon2D->Update();
	}
}

//**************************************************
// リザルトランク描画
//**************************************************
void CResultRank::Draw(void)
{
	// ポリゴン更新
	if (m_pPolygon2D != nullptr)
	{
		m_pPolygon2D->Draw();
	}

	// 文字列更新
	if (m_pWord != nullptr)
	{
		m_pWord->Draw();
	}
}

//***************************************************************
// リザルトランク生成
//***************************************************************
CResultRank* CResultRank::Create(INFO info, const char* pTexturePassPolygon, DWORD format, int nFontID, const char* pFontName)
{
	// リザルトランクインスタンス生成
	CResultRank* pTemplate = new CResultRank;

	if (pTemplate != nullptr)
	{
		pTemplate->Init(info, pTexturePassPolygon, format, nFontID, pFontName);
	}

	return pTemplate;
}

//***************************************************************
// リザルトランク文字列設定
//***************************************************************
void CResultRank::SetWord(std::string str)
{
	// 文字列設定
	m_info.strWord = str;

	if (m_pWord != nullptr)
	{
		m_pWord->SetStringWord(str.c_str());
	}
}