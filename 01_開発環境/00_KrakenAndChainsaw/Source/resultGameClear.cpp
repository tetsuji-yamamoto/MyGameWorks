//*******************************************************
// 
// リザルトシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "resultGameClear.h"
#include "word.h"
#include "debugproc.h"
#include "myMath.h"
#include "manager.h"

//*******************************************************
// リザルトシーンコンストラクタ
//*******************************************************
CResultGameClear::CResultGameClear()
{
	m_nCntTime = 0;	// タイマーカウンター
}

//*******************************************************
// リザルトシーンデストラクタ
//*******************************************************
CResultGameClear::~CResultGameClear()
{

}

//*******************************************************
// リザルトシーン初期化
//*******************************************************
void CResultGameClear::Init(MODE mode)
{
	// 親クラスを初期化
	CScene::Init(mode);

	m_pWord = CWord::Create(InitValu::VEC3_CENTER, Color::_YELLOW, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, "GameClear", DT_CENTER);
}

//*******************************************************
// リザルトシーン終了
//*******************************************************
void CResultGameClear::Uninit(void)
{
	// タイトルへ移行するボタン破棄
	if (m_pWord != nullptr)
	{
		m_pWord->Uninit();
		m_pWord = nullptr;
	}

	// 自身の破棄
	CScene::Uninit();
}

//*******************************************************
// リザルトシーン更新
//*******************************************************
void CResultGameClear::Update(void)
{
	// カウント
	if (my_math::CountClamp(m_nCntTime, 0, TIME_CHAGE) == true)
	{
		CManager::GetInstance()->SetFade(CScene::MODE_TITLE);
	}


#ifdef _DEBUG
	CDebugprocc::Print("SCene : [ RESULT GAMEOVER ]\n");
#endif
}

//*******************************************************
// リザルトシーン描画
//*******************************************************
void CResultGameClear::Draw(void)
{
	m_pWord->Draw();
}