//**********************************************
// 
// マネージャー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "manager.h"
#include "object2D.h"
#include "object3D.h"
#include "player.h"
#include "explosion.h"
#include "bgManager.h"
#include "effect.h"
#include "number.h"
#include "timer.h"
#include "line.h"
#include "edit.h"

// 静的メンバー変数宣言
CRenderer* CManager::m_pRenderer = NULL ;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CPlayer* CManager::m_pPlayer = NULL;
CSound* CManager::m_pSound = NULL;
CDebugprocc* CManager::m_pDebugprocc = NULL;
CPause* CManager::m_pPause = NULL;
CTexture* CManager::m_pTexture = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CObject3D* CManager::m_obj3Dtest = NULL;
CFontManager* CManager::m_pFontMane = NULL;
std::string CManager::m_sFilePathMotionSkinRead;		// モーション読込用ファイルパス
std::string CManager::m_sFilePathMotionSkinReadBinary;	// モーション読込用ファイルパスバイナリ
std::string CManager::m_sFilePathMotionSkinWrite;		// モーション書き込用ファイルパス
std::string CManager::m_sFilePathMotionSkinWriteBinary;	// モーション書き込用ファイルパスバイナリ

//**********************************************
// マネージャーコンストラクタ
//**********************************************
CManager::CManager()
{
}

//**********************************************
// マネージャーデストラクタ
//**********************************************
CManager::~CManager()
{
}

//**********************************************
// マネージャー初期化処理
//**********************************************
HRESULT CManager::Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow)
{
	// メモリ確保
	m_pRenderer = new CRenderer;

	if (m_pRenderer != NULL)
	{// 確保できていれば
		// レンダラーの初期化
		if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
		{
			MessageBox(NULL, " CManager::Init() > if(FAILED(m_pRenderer->Init(hWnd, TRUE))) > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pRenderer != NULL) > m_pRendererメモリを確保できませんでした", "Error", MB_OK);
	}

	// メモリ確保
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{// 確保できていれば
		// キーボードの初期化
		if (FAILED(m_pInputKeyboard->Init(hinstance, hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > if (FAILED(m_pInputKeyboard->Init(hinstance,hWnd))) > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pInputKeyboard != NULL) > m_pInputKeyboardメモリを確保できませんでした", "Error", MB_OK);
	}

	// メモリ確保
	m_pInputMouse = new CInputMouse;

	if (m_pInputMouse != NULL)
	{// 確保できていれば
		// マウスの初期化
		if (FAILED(m_pInputMouse->Init(hinstance, hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > if (FAILED(m_pInputMouse->Init(hinstance,hWnd))) > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pInputMouse != NULL) > m_pInputKeyboardメモリを確保できませんでした", "Error", MB_OK);
	}

	// メモリ確保
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != NULL)
	{// 確保できていれば
		// ジョイパッドの初期化
		if (FAILED(m_pInputJoypad->Init(hinstance, hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > if (FAILED(m_pInputJoypad->Init(hinstance,hWnd))) > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pInputJoypad != NULL) > m_pInputJoypadメモリを確保できませんでした", "Error", MB_OK);
	}

	// メモリ確保
	//m_pSound = new CSound;

	//if (m_pSound != NULL)
	//{// 確保できていれば
	//	// サウンドの初期化
	//	if (FAILED(m_pSound->InitSound(hWnd)))
	//	{
	//		MessageBox(NULL, " CManager::Init() > if (FAILED(m_pSound->Init(hinstance,hWnd))) > 初期化できませんでした", "Error", MB_OK);
	//		return -1;
	//	}
	//}
	//else
	//{
	//	MessageBox(NULL, " CManager::Init() > if (m_pSound != NULL) > m_pSoundメモリを確保できませんでした", "Error", MB_OK);
	//}

	// メモリ確保
	m_pDebugprocc = new CDebugprocc;

	if (m_pDebugprocc != NULL)
	{// 確保できていれば
		// デバッグの初期化
		m_pDebugprocc->Init();
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pDebugprocc != NULL) > m_pDebugproccメモリを確保できませんでした", "Error", MB_OK);
	}

	// メモリ確保
	m_pCamera = new CCamera;
	if (m_pCamera != NULL)
	{// 確保できていれば
		// デバッグの初期化
		m_pCamera->Init();
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pCamera != NULL) > m_pCameraメモリを確保できませんでした", "Error", MB_OK);
	}

	// メモリ確保
	m_pLight = new CLight;
	if (m_pLight != NULL)
	{// 確保できていれば
		// デバッグの初期化
		m_pLight->Init();
	}
	else
	{
		MessageBox(NULL, " CManager::Init() > if (m_pLight != NULL) > m_pLightメモリを確保できませんでした", "Error", MB_OK);
	}

	m_pTexture = CTexture::Create();

	m_pPause = CPause::Create();
	
	float fWidth = 10.0f;
	float fLength = 500.0f;
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,0.4f };

	for (int nCnt = 0; nCnt < 51; nCnt++)
	{
		CLine::Create(D3DXVECTOR3(fWidth * nCnt, 0.0f, -fLength), D3DXVECTOR3(fWidth * nCnt, 0.0f, fLength), col);
	}

	for (int nCnt = 0; nCnt < 50; nCnt++)
	{
		CLine::Create(D3DXVECTOR3(-fWidth - fWidth * nCnt, 0.0f, -fLength), D3DXVECTOR3(-fWidth - fWidth * nCnt, 0.0f, fLength), col);
	}

	for (int nCnt = 0; nCnt < 51; nCnt++)
	{
		CLine::Create(D3DXVECTOR3(-fLength, 0.0f, fWidth * nCnt), D3DXVECTOR3(fLength, 0.0f, fWidth * nCnt), col);
	}

	for (int nCnt = 0; nCnt < 50; nCnt++)
	{
		CLine::Create(D3DXVECTOR3( - fLength, 0.0f, -fWidth - fWidth * nCnt), D3DXVECTOR3(fLength, 0.0f, -fWidth - fWidth * nCnt ), col);
	}

	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	
	// システムファイルの読込
	ReadJsonSystemFile();

	CEdit::Create(GetFilePathMotionSkinRead().c_str());

	return S_OK;
}

//**********************************************
// マネージャー終了処理
//**********************************************
void CManager::Uninit(void)
{
	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{// ヌルでなかったら
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{// ヌルでなかったら
		// マウスの終了処理
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// ジョイパッドの破棄
	if (m_pInputJoypad != NULL)
	{// ヌルでなかったら
		// ジョイパッドの終了処理
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{// ヌルでなかったら
		// サウンドの終了処理
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}

	// デバッグの破棄
	if (m_pDebugprocc != NULL)
	{// ヌルでなかったら
		// デバッグの終了処理
		m_pDebugprocc->Uninit();
		delete m_pDebugprocc;
		m_pDebugprocc = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{// ヌルでなかったら
		// デバッグの終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{// ヌルでなかったら
		// デバッグの終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pFontMane != NULL)
	{
		m_pFontMane->Uninit();
		delete m_pFontMane;
		m_pFontMane = NULL;
	}

	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{// ヌルでなかったら
		// レンダラーの終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	m_pPause = NULL;	// ポーズのメモリを忘れる
	m_pPlayer = NULL;	// ポーズのメモリを忘れる

	m_pTexture->Unload();	// テクスチャとメモリ破棄
	m_pTexture = NULL;		// 忘れる

	m_obj3Dtest = NULL;
	m_ObjectX = NULL;
	m_ObjectBillboard = NULL;
}

//**********************************************
// マネージャー更新処理
//**********************************************
void CManager::Update(void)
{
	m_pInputKeyboard->Update();	// キーボード
	m_pInputMouse->Update();	// マウス
	m_pInputJoypad->Update();	// ジョイパッド
	m_pCamera->Update();		// カメラ

	bool bNowPause = m_pPause->GetPause();	// 現在のポーズの有無を取得
	if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
	{// Pキーを押したら
		m_pPause->SetPause(!bNowPause);			// 現在とは逆のポーズにする
	}

	if (m_pPause->GetPause() == false)
	{
		m_pRenderer->Update();		// レンダラーの更新処理
	}
}

//**********************************************
// マネージャー描画処理
//**********************************************
void CManager::Draw(void)
{
	//CObject::DrawDebug();		// オブジェクトのデバッグ表示
	//CTexture::DebugTexture();	// テクスチャンのデバッグ表示
	
	m_pRenderer->Draw();		// レンダラーの描画処理
}

//**********************************************
// マネージャーシステムファイル読込処理
//**********************************************
void CManager::ReadJsonSystemFile(void)
{
	using namespace nlohmann;
	using namespace std;

	// ファイルを読込
	ifstream ifs("data\\system\\system.json");

	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CManager::Read() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// ジェイソンオブジェクト生成
	json json;

	// jsonの情報を読み込む
	ifs >> json;

	// ファイルパスを移し替える
	m_sFilePathMotionSkinRead = json["FILEPATH_READ_MOTIONSKINMESH"];
	m_sFilePathMotionSkinReadBinary = json["FILEPATH_READ_MOTIONSKINMESH_BINARY"];
	m_sFilePathMotionSkinWrite = json["FILEPATH_WRITE_MOTIONSKINMESH"];
	m_sFilePathMotionSkinWriteBinary = json["FILEPATH_WRITE_MOTIONSKINMESH_BINARY"];

	// ファイルを閉じる
	ifs.close();
}