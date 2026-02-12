//****************************************************************************
// 
// 背景マネージャー
// Author tetuji yamamoto
// 
//****************************************************************************
#include "bgManager.h"

//****************************************************************************
// 背景マネージャー
//****************************************************************************
CBGManager::CBGManager()
{
	memset(m_apBG,NULL,sizeof(m_apBG));
}

//****************************************************************************
// 背景マネージャー
//****************************************************************************
CBGManager::~CBGManager()
{
}

//****************************************************************************
// 背景マネージャー
//****************************************************************************
HRESULT CBGManager::Init(int nBGNum, float fFeedRate, float fFeedAngle)
{
	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = { fWidth,fHeight,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	for (int nCnt = 0; nCnt < MAX_BG_NUM && nCnt < nBGNum; nCnt++)
	{
		float fFR = fFeedRate + fFeedRate * nCnt;
		m_apBG[nCnt] =  CBG::Create(fFR, fFeedAngle, (CBG::TYPE)nCnt);
	}

	return S_OK;
}

//****************************************************************************
// 背景マネージャー
//****************************************************************************
void CBGManager::Uninit(void)
{
	memset(m_apBG, NULL, sizeof(m_apBG));
	Release();
}

//****************************************************************************
// 背景マネージャー
//****************************************************************************
void CBGManager::Update(void)
{

}

//****************************************************************************
// 背景マネージャー
//****************************************************************************
void CBGManager::Draw(void)
{

}

//****************************************************************************
// 背景マネージャー
//****************************************************************************
CBGManager* CBGManager::Create(int nBGNum, float fFeedRate, float fFeedAngle)
{
	CBGManager* pManager = NULL;
	int nNumAll = GetObujectNumAll();

	// オブジェクト2Dの生成
	if (pManager == NULL)
	{
		
		pManager = new CBGManager;

		if (pManager != NULL)
		{
			// 初期化処理
			pManager->Init(nBGNum, fFeedRate, fFeedAngle);
		}
		else
		{
			MessageBox(NULL, " CBGManager::Create() > if ( pManager != NULL) メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CBGManager::Create() > if ( pManager == NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return pManager;
}