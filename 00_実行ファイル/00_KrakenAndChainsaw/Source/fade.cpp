//*************************************************
// 
// フェード処理
// Author Tetsuji Yamamoto
// 
//*************************************************
#include "fade.h"
#include "manager.h"
#include "easing.h"
#include "scene.h"
#include "screen.h"

//*************************************************
// フェード処理コンストラクタ
//*************************************************
CFade::CFade()
{
	m_fade = FADE_NONE;				// フェード状態
	m_modeNext = CScene::MODE_MAX;	// 次のモード
	m_nCntFrame = 0;				// フレームカウンター
	m_pScreen = nullptr;				// スクリーンポインタ
}

//*************************************************
// フェード処理デストラクタ
//*************************************************
CFade::~CFade()
{

}

//*************************************************
// フェード処理初期化
//*************************************************
void CFade::Init(CScene::MODE modeNext)
{
	// スクリーンポリゴン生成
	if (m_pScreen == nullptr)
	{
		m_pScreen = CScreen::Create(Color::_BLACK);
	}

	//フェードイン状態に
	m_fade = FADE_IN;
	
	//次の画面(モード)を設定
	m_modeNext = modeNext;

	// フレームカウンターセット
	m_nCntFrame = FADE_FRAME;
}

//*************************************************
// フェード処理終了
//*************************************************
void CFade::Uninit(void)
{
	// スクリーンポリゴン破棄
	if (m_pScreen != nullptr)
	{
		m_pScreen->Uninit();
	}

	// 自身の破棄
	delete this;
}

//*************************************************
// フェード処理更新
//*************************************************
void CFade::Update(void)
{
	// スクリーンポリゴンが無ければ処理しない
	if (m_pScreen == nullptr)
	{
		return;
	}

	// 今のポリゴンの色を取得
	D3DXCOLOR nowColor = m_pScreen->GetColor();

	if (m_fade != FADE_NONE)
	{
		// フレームカウンターカウントダウン
		m_nCntFrame--;

		// 時間倍率算出
		float fMugTime = (float)m_nCntFrame / (float)FADE_FRAME;
		fMugTime = easing::OutExpo(fMugTime);

		//フェードイン状態
		if (m_fade == FADE_IN)
		{
			// 色を設定
			nowColor.a = fMugTime;
			
			if (m_nCntFrame <= 0)
			{
				//何もしていない状態
				m_fade = FADE_NONE;
			}
		}
		//フェードアウト状態
		else if (m_fade == FADE_OUT)
		{
			// 色を設定
			nowColor.a = 1.0f - fMugTime;

			if (m_nCntFrame <= 0)
			{
				// フレームカウンターセット
				m_nCntFrame = FADE_FRAME;

				//フェードイン状態
				m_fade = FADE_IN;

				//モード設定(次の画面に移行)
				CManager::GetInstance()->SetMode(m_modeNext);

				// 処理を切り上げる
				return;
			}
		}
	}

	// 色を設定
	m_pScreen->SetColor(nowColor);
}

//*************************************************
// フェード処理描画
//*************************************************
void CFade::Draw(void)
{
	// スクリーンポリゴン描画
	if (m_pScreen != nullptr)
	{
		m_pScreen->Draw(NULL);
	}
}

//*************************************************
// フェード処理設定処理
//*************************************************
void CFade::SetFade(CScene::MODE modeNext)
{
	// 設定していなければ処理する
	if (m_fade != FADE_NONE)
	{
		return;
	}

	//フェードアウト状態に
	m_fade = FADE_OUT;
	
	//次の画面(モード)を設定
	m_modeNext = modeNext;

	// フレームカウンターセット
	m_nCntFrame = FADE_FRAME;
}

//*************************************************
// フェード生成
//*************************************************
CFade* CFade::Create(CScene::MODE modeNext)
{
	// フェードインスタンス生成
	CFade* pFade = new CFade;

	if (pFade != nullptr)
	{
		// 初期化処理
		pFade->Init(modeNext);
	}
	else
	{
		MessageBox(NULL, " CFade::Create() メモリを確保できませんでした", "Error", MB_OK);
	}

	return pFade;
}