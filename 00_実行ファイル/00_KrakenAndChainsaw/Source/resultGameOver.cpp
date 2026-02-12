//*******************************************************
// 
// リザルトシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "resultGameOver.h"
#include "word.h"
#include "debugproc.h"
#include "myMath.h"
#include "manager.h"

//*******************************************************
// リザルトシーンコンストラクタ
//*******************************************************
CResultGameOver::CResultGameOver()
{
	m_nCntTime = 0;	// タイマーカウンター
}

//*******************************************************
// リザルトシーンデストラクタ
//*******************************************************
CResultGameOver::~CResultGameOver()
{

}

//*******************************************************
// リザルトシーン初期化
//*******************************************************
void CResultGameOver::Init(MODE mode)
{
	// 親クラスを初期化
	CScene::Init(mode);

	m_pWord = CWord::Create(InitValu::VEC3_CENTER, Color::_RED, SCREEN_WIDTH*0.5f, SCREEN_HEIGHT * 0.3f, "GameOver", DT_CENTER);
}

//*******************************************************
// リザルトシーン終了
//*******************************************************
void CResultGameOver::Uninit(void)
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
void CResultGameOver::Update(void)
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
void CResultGameOver::Draw(void)
{
	m_pWord->Draw();
}