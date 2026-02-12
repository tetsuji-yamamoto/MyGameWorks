//***********************************************
// 
// 攻撃案内UI
// Author Tetsuji Yamamoto
// 
//***********************************************
#include "AttackGuideUI.h"
#include "CompWord.h"
#include "CompPolygon2D.h"
#include "CompCounter.h"
#include "input.h"

using namespace std;

//***********************************************
// コンストラクタ
//***********************************************
CAttackGuide::CAttackGuide()
{
	m_bNext = false;	// 次か否か
}

//***********************************************
// デストラクタ
//***********************************************
CAttackGuide::~CAttackGuide()
{

}

//***********************************************
// 初期化
//***********************************************
HRESULT CAttackGuide::Init(void)
{
	// 自身をシーンの所有物にする
	CManager::GetInstance()->GetScene()->AddObj(this);

	// ポリゴン2D用変数
	CCompPolygon2D::Argment Polygon2DArg;
	Polygon2DArg.col = Color::_DARKCYAN;
	Polygon2DArg.col.a = 0.0f;
	Polygon2DArg.fHeight = HEIGHT * 2.0f;
	Polygon2DArg.fWidth = WIDTH;
	Polygon2DArg.pos = { POS_X ,POS_Y + HEIGHT,POS_Z };
	Polygon2DArg.rot = InitValu::VEC3_NULL;

	// アイコン用変数
	CCompPolygon2D::Argment Polygon2DIconArg;
	Polygon2DIconArg.col = Color::_WHITE;
	Polygon2DIconArg.col.a = 0.0f;
	Polygon2DIconArg.fHeight = HEIGHT;
	Polygon2DIconArg.fWidth = WIDTH;
	Polygon2DIconArg.pos = { POS_X ,POS_Y  ,POS_Z };
	Polygon2DIconArg.rot = InitValu::VEC3_NULL;
	Polygon2DIconArg.TexturePath = "data\\texture\\AttackGuide.png";

	// 文字用変数
	CCompWord::Argment WordArgment;
	WordArgment.col = Color::_WHITE;
	WordArgment.col.a = 0.0f;
	WordArgment.fHeight = HEIGHT;
	WordArgment.fWidth = WIDTH;
	WordArgment.format = DT_LEFT;
	WordArgment.pos = { POS_X ,POS_Y + HEIGHT + HEIGHT,POS_Z };
	WordArgment.Str = "で攻撃";

	// コンポーネント類追加
	AddComponent<CCompPolygon2D>(string("Polygon2D"), Polygon2DArg);
	AddComponent<CCompPolygon2D>(string("Icon"), Polygon2DIconArg);
	AddComponent<CCompWord>(string("Word"), WordArgment);
	AddComponent<CCounter>(string("MyCounter"), COUNT);

	return S_OK;
}

//***********************************************
// 終了
//***********************************************
void CAttackGuide::Uninit(void)
{
	// 自身の破棄
	Release();
}

//***********************************************
// 更新
//***********************************************
void CAttackGuide::Update(void)
{
	// キー入力ジョイパッド入力取得
	CInputKeyboard* pKeyboard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pJoyPad = CInputJoypad::GetJoypad();

	// カウンター取得
	CCompPolygon2D* pPoly2D = GetComponent<CCompPolygon2D>("Polygon2D");
	CCompPolygon2D* pIcon = GetComponent<CCompPolygon2D>("Icon");
	CCompWord* pWord = GetComponent<CCompWord>("Word");
	CCounter* pCounter = GetComponent<CCounter>("MyCounter");

	// パラメータ取得
	float fPalam = pCounter->GetPalam();

	// 色の薄さをパラメータの値にする
	D3DXCOLOR ColPoly2D = pPoly2D->GetColor();
	D3DXCOLOR ColIcon = pIcon->GetColor();
	D3DXCOLOR ColWord = pWord->GetColor();
	ColPoly2D.a = fPalam;
	ColIcon.a = fPalam;
	ColWord.a = fPalam;
	pPoly2D->SetColor(ColPoly2D);
	pIcon->SetColor(ColIcon);
	pWord->SetColor(ColWord);

	// ジャンプしたら
	if (
		pKeyboard->GetPress(DIK_F) == true ||
		pJoyPad->GetkeyPress(CInputJoypad::JOYKEY_R1) == true
		)
	{
		// カウンターを減らすようにする
		pCounter->SetIsIncrement(false);
	}

	// カウンターが最小まで来たら次へ移る
	if (pCounter->GetIsMin() == true)
	{
		// シーンから削除
		CManager::GetInstance()->GetScene()->DeleteObj<CAttackGuide>();
	}
}

//***********************************************
// 描画
//***********************************************
void CAttackGuide::Draw(void)
{
	// コンポーネント描画
	DrawComponent();
}

//***********************************************
// 生成
//***********************************************
CAttackGuide* CAttackGuide::Create(void)
{
	// インスタンス生成と初期化
	CAttackGuide* pAttackGuide = new CAttackGuide;
	pAttackGuide->Init();

	// このポインターを返す
	return pAttackGuide;
}