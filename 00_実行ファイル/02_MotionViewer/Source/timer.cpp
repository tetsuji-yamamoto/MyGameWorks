//****************************************************************************
// 
// タイマー
// Author tetuji yamamoto
// 
//****************************************************************************
#include "timer.h"
#include "number.h"

// 静的メンバー変数宣言
int CTimer::m_nTime = 0;
int CTimer::m_nCntTime = 0;

//****************************************************************************
// タイマー
//****************************************************************************
CTimer::CTimer()
{
	memset(m_apNumber, NULL, sizeof(m_apNumber));
	m_nDigit = 0;
}

//****************************************************************************
// タイマー
//****************************************************************************
CTimer::~CTimer()
{
}

//****************************************************************************
// タイマー
//****************************************************************************
HRESULT CTimer::Init()
{
	return S_OK;
}

//****************************************************************************
// タイマー
//****************************************************************************
HRESULT CTimer::Init(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nDigits, int nTime)
{
	m_nCntTime = 0;
	m_nDigit = nDigits;
	m_nTime = nTime;

	float fDigitWidth = fWidth / nDigits;
	float fDigitWidthHerf = fDigitWidth * 0.5f;

	for (int nCntDigit = 0; nCntDigit < nDigits; nCntDigit++)
	{
		D3DXVECTOR3 pos = InitValu::VEC3_NULL;
		pos.x = startPos.x + fDigitWidthHerf + fDigitWidth * nCntDigit;
		pos.y = startPos.y + fHeight * 0.5f;

		m_apNumber[nCntDigit] = CNumber::Create(pos, col, fDigitWidthHerf, fHeight * 0.5f, 0);

		if (m_apNumber[nCntDigit] == NULL)
		{
			MessageBox(NULL, " CTimer::Init() > if ( m_apNumber[nCntDigit] != NULL) クリエイトに失敗しました", "Error", MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}

//****************************************************************************
// タイマー
//****************************************************************************
void CTimer::Uninit(void)
{
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_DIGIT; nCntDigit++)
	{
		m_apNumber[nCntDigit]->Uninit();
		m_apNumber[nCntDigit] = NULL;
	}

	Release();
}

//****************************************************************************
// タイマー
//****************************************************************************
void CTimer::Update(void)
{
	if (CTimer::SecondsCnt() == true)
	{
		m_nTime--;

		if (m_nTime <= 0)
		{
			m_nTime = 0;
		}

		int nData = (int)pow(10, (double)m_nDigit - 1);

		for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
		{
			int nNum = m_nTime / nData;
			m_apNumber[nCntDigit]->SetNumber(nNum);

			nData = (int)((float)nData * 0.1f);
		}
	}
}

//****************************************************************************
// タイマー
//****************************************************************************
void CTimer::Draw(void)
{
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_DIGIT; nCntDigit++)
	{
		m_apNumber[nCntDigit]->Draw();
	}
}

//****************************************************************************
// タイマー
//****************************************************************************
CTimer* CTimer::Create(D3DXVECTOR3 startPos, D3DXCOLOR col, float fWidth, float fHeight, int nDigits, int nTime)
{
	CTimer* pTimer = NULL;
	int nNumAll = GetObujectNumAll();

	// オブジェクト2Dの生成
	if (pTimer == NULL)
	{
		pTimer = new CTimer;

		if (pTimer != NULL)
		{
			// 初期化処理
			pTimer->Init(startPos, col, fWidth, fHeight, nDigits);
		}
		else
		{
			MessageBox(NULL, " CEnemy* CEnemy::Create() > if ( pEnemy != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CEnemy* CEnemy::Create() > if ( pEnemy == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return pTimer;

}

//****************************************************************************
// タイマー
//****************************************************************************
bool CTimer::SecondsCnt(void)
{
	m_nCntTime++;

	if (m_nCntTime >= 60)
	{
		m_nCntTime = 0;
		return true;
	}

	return false;
}