//****************************************************************************
// 
// スコア
// Author tetuji yamamoto
// 
//****************************************************************************
#include "score.h"
#include "fontManager.h"
#include "manager.h"
#include "CompWord.h"
#include "CompPolygon2D.h"
#include <locale.h>

//****************************************************************************
// スコアコンストラクタ
//****************************************************************************
CScore::CScore() : CObject(CObject::PRIORITY_4)
{
	m_nScore = 0;
}

//****************************************************************************
// スコアデストラクタ
//****************************************************************************
CScore::~CScore()
{
}

//****************************************************************************
// スコア初期化
//****************************************************************************
HRESULT CScore::Init(D3DXVECTOR3 startPos, D3DXCOLOR col,float fWidth, float fHeight)
{
	// 文字列作成
	std::string str = "Score : ";
	str += std::to_string(InitValu::INTMAX);

	// 文字列用引数
	CCompWord::Argment WordArg;
	WordArg.col = col;
	WordArg.pos = startPos;
	WordArg.fWidth = fWidth;
	WordArg.fHeight = fHeight;
	WordArg.Str = str;
	WordArg.format = DT_LEFT;

	// ポリゴン用引数
	CCompPolygon2D::Argment Polygon2DArg;
	Polygon2DArg.col = Color::_BLACK;
	Polygon2DArg.col.a = 0.5f;
	Polygon2DArg.fHeight = fHeight;
	Polygon2DArg.fWidth = fWidth;
	Polygon2DArg.pos = startPos;
	Polygon2DArg.rot = InitValu::VEC3_NULL;
	Polygon2DArg.type = CCompPolygon2D::TYPE_CENTER;

	// コンポーネント追加
	AddComponent<CCompPolygon2D>	(CComponent::PRIORITY::PRIORITY_UI,std::string("ScorePolygon2D"), Polygon2DArg);	// ポリゴン
	AddComponent<CCompWord>			(CComponent::PRIORITY::PRIORITY_UI,std::string("ScoreWord"), WordArg);				// 文字

	return S_OK;
}

//****************************************************************************
// スコア終了
//****************************************************************************
void CScore::Uninit(void)
{
	// 自身の破棄
	Release();
}

//****************************************************************************
// スコア更新
//****************************************************************************
void CScore::Update(void)
{
	// 文字列作成
	std::string str = "Score : ";
	str += std::to_string(m_nScore);
	
	// 文字列コンポーネント取得
	CCompWord* pWord = GetComponent<CCompWord>("ScoreWord");
	
	// 文字の設定
	pWord->SetStringWord(str.c_str());
}

//****************************************************************************
// スコア描画
//****************************************************************************
void CScore::Draw(void)
{
	// コンポーネント描画
	DrawComponent();
}

//****************************************************************************
// スコア生成
//****************************************************************************
CScore* CScore::Create(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight)
{
	// スコアインスタンス生成
	CScore* pScore = new CScore;

	if (pScore != nullptr)
	{
		// 初期化処理
		pScore->Init(startPos, col, fWidth, fHeight);
	}

	return pScore;
}

//****************************************************************************
// スコア加算処理
//****************************************************************************
void CScore::AddScore(int nAddScore)
{
	// intの最大数を超えようとしたら止める
	if (m_nScore + nAddScore >= MAXSORE)
	{
		m_nScore = MAXSORE;
	}
	// マイナス域に行こうとしたら止める
	else if (m_nScore + nAddScore <= 0)
	{
		m_nScore = 0;
	}
	// それ以外は足す
	else
	{
		// スコアに加算
		m_nScore += nAddScore;
	}
}