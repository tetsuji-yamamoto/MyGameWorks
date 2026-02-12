//****************************************************************************
// 
// タイマー
// Author tetuji yamamoto
// 
//****************************************************************************
#include "timer.h"
#include "word.h"
#include "manager.h"

// 静的メンバー変数宣言
bool CTimer::m_bCount = true;	// タイムカウントの有無

//****************************************************************************
// タイマーコンストラクタ
//****************************************************************************
CTimer::CTimer() : CObject(CObject::PRIORITY_4)
{
	m_nTime = 0;			// 時間
	m_nCntTime = 0;			// 時間カウンター
	m_pWord = nullptr;		// 文字
	m_bTimeCount = false;	// 時間をカウントしたかの有無
	m_bTimeOut = false;		// 時間切れか否か
}

//****************************************************************************
// タイマーデストラクタ
//****************************************************************************
CTimer::~CTimer()
{
}

//****************************************************************************
// タイマー初期化
//****************************************************************************
HRESULT CTimer::Init()
{
	return S_OK;
}

//****************************************************************************
// タイマー初期化
//****************************************************************************
HRESULT CTimer::Init(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nTime)
{
	m_nCntTime = 0;
	m_nTime = nTime;

	// 文字列初期化
	if (m_pWord == nullptr)
	{
		std::string strA = "Time : ";
		strA += std::to_string(m_nTime);
		m_pWord = CWord::Create(startPos, col, fWidth, fHeight, strA.c_str());
	}

	return S_OK;
}

//****************************************************************************
// タイマー終了
//****************************************************************************
void CTimer::Uninit(void)
{
	// ワード破棄
	if (m_pWord != nullptr)
	{
		m_pWord->Uninit();
		m_pWord = nullptr;
	}

	// 自身の破棄
	Release();
}

//****************************************************************************
// タイマー更新
//****************************************************************************
void CTimer::Update(void)
{
#ifdef _DEBUG
	// カウントしない状態なら処理を止める
	if (m_bCount == false)
	{
		return;
	}
#endif

	// 時間をカウントしていない状態にする
	m_bTimeCount = false;

	if (CTimer::SecondsCnt() == true)
	{
		// タイマーを減らす
		m_nTime--;

		// タイマーがゼロより低くならないようにする
		if (m_nTime <= 0)
		{
			m_nTime = 0;
			
			// 時間切れ状態にする
			m_bTimeOut = true;		
		}

		// 数字を文字列に変換し設定
		if (m_pWord != nullptr)
		{
			std::string strA = "Time : ";
			strA += std::to_string(m_nTime);
			m_pWord->SetStringWord(strA.c_str());
		}
	}
}

//****************************************************************************
// タイマー描画
//****************************************************************************
void CTimer::Draw(void)
{
	// 文字の描画
	if (m_pWord != nullptr)
	{
		m_pWord->Draw();
	}
}

//****************************************************************************
// タイマー生成
//****************************************************************************
CTimer* CTimer::Create(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nTime)
{
	// タイマーインスタンス生成
	CTimer* pTimer = new CTimer;

	// メモリが確保できていたら初期化
	if (pTimer != nullptr)
	{
		pTimer->Init(startPos, col, fWidth, fHeight, nTime);
	}

	return pTimer;
}

//****************************************************************************
// タイマー計測処理
//****************************************************************************
bool CTimer::SecondsCnt(void)
{
	// 時間カウンター加算
	m_nCntTime++;

	// 60フレームで一秒とする
	if (m_nCntTime >= 60)
	{
		// 時間カウンターゼロ
		m_nCntTime = 0;

		// 時間をカウントしたことにする
		m_bTimeCount = true;
		
		return true;
	}

	return false;
}