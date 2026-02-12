//*******************************************************
// 
// チュートリアルシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "tutorial.h"
#include "debugproc.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"
#include "screen.h"
#include "object2D.h"
#include "input.h"

//*******************************************************
// チュートリアルシーンコンストラクタ
//*******************************************************
CTutorial::CTutorial()
{
	m_pScreen = nullptr;	// 画面用ポリゴン
	m_pScreen2 = nullptr;	// 画面用ポリゴン
	m_nCnt = 0;// カウンター
}

//*******************************************************
// チュートリアルシーンデストラクタ
//*******************************************************
CTutorial::~CTutorial()
{
}

//*******************************************************
// チュートリアルシーン初期化
//*******************************************************
void CTutorial::Init(MODE mode)
{

	if (m_pScreen == nullptr)
	{
		m_pScreen = CObject2D::Create(InitValu::VEC3_CENTER,InitValu::VEC3_NULL,Color::_FULL,SCREEN_WIDTH * 0.7f,SCREEN_HEIGHT * 0.75f, "data\\texture\\setumei.png");
	}
}

//*******************************************************
// チュートリアルシーン終了
//*******************************************************
void CTutorial::Uninit(void)
{
	if (m_pScreen != nullptr)
	{
		m_pScreen->Uninit();
		m_pScreen = nullptr;
	}

	// 自身の破棄
	CScene::Uninit();
}

//*******************************************************
// チュートリアルシーン更新
//*******************************************************
void CTutorial::Update(void)
{
	CTexture* texture = CManager::GetInstance()->GetTexture();

	// 入力取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// ゲームに移行
	if (pInputMouse->GetTrigger(CInputMouse::MOUSEBUTTON_LEFT) == true ||
		pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		m_nCnt++;

		switch (m_nCnt)
		{
		case 1:
			break;

		case 2:
			CManager::GetInstance()->SetFade(CScene::MODE_GAME);
			break;

		default:
			break;
		}

	}

#ifdef _DEBUG
	CDebugprocc::Print("SCene : [ TITLE ]\n");
#endif
}

//*******************************************************
// チュートリアルシーン描画
//*******************************************************
void CTutorial::Draw(void)
{
}