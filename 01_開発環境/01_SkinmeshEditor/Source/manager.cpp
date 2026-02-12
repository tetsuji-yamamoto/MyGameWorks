//**********************************************
// 
// マネージャー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "manager.h"
#include "object2D.h"
#include "object3D.h"
#include "line.h"
#include "edit.h"
#include "myMath.h"

// 静的メンバー変数宣言
CRenderer* CManager::m_pRenderer = nullptr;				// レンダラーへのポインタ
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;	// キーボードへのポインタ
CInputMouse* CManager::m_pInputMouse = nullptr;			// マウスへのポインタ
CInputJoypad* CManager::m_pInputJoypad = nullptr;		// ジョイパッドへのポインタ
CSound* CManager::m_pSound = nullptr;					// サウドへのポインタ
CDebugprocc* CManager::m_pDebugprocc = nullptr;			// デバッグのポインタ
CPause* CManager::m_pPause = nullptr;					// ポーズのポインタ
CTexture* CManager::m_pTexture = nullptr;				// テクスチャのポインタ
CCamera* CManager::m_pCamera = nullptr;					// カメラのポインタ
CLight* CManager::m_pLight = nullptr;					// ライトのポインタ
CObject3D* CManager::m_obj3Dtest = nullptr;				// フォントマネージャーのポインタ
CFontManager* CManager::m_pFontMane = nullptr;			// フォントマネージャー
CEdit* CManager::m_pEdit = nullptr;						// 編集クラス
std::string CManager::m_sFilePathModel;					// 読み込むモデルパス
std::string CManager::m_sFilePathReadSkinMesh;			// 読み込むスキン情報パス
std::string CManager::m_sFilePathReadSkinMeshBinary;	// 読み込むスキン情報パスバイナリ
std::string CManager::m_sFilePathWriteSkinMesh;			// 書き込むスキン情報パス
std::string CManager::m_sFilePathWriteSkinMeshBinary;	// 書き込むスキン情報パスバイナリ

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
	m_pTexture->Load();

	m_pPause = CPause::Create();
	
	float fWidth = 10.0f;
	float fLength = 2000.0f;
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,0.4f };

	for (int nCnt = 0; nCnt < 201; nCnt++)
	{
		CLine::Create(D3DXVECTOR3(fWidth * nCnt, 0.0f, -fLength), D3DXVECTOR3(fWidth * nCnt, 0.0f, fLength), col);
	}

	for (int nCnt = 0; nCnt < 200; nCnt++)
	{
		CLine::Create(D3DXVECTOR3(-fWidth - fWidth * nCnt, 0.0f, -fLength), D3DXVECTOR3(-fWidth - fWidth * nCnt, 0.0f, fLength), col);
	}

	for (int nCnt = 0; nCnt < 201; nCnt++)
	{
		CLine::Create(D3DXVECTOR3(-fLength, 0.0f, fWidth * nCnt), D3DXVECTOR3(fLength, 0.0f, fWidth * nCnt), col);
	}

	for (int nCnt = 0; nCnt < 200; nCnt++)
	{
		CLine::Create(D3DXVECTOR3( - fLength, 0.0f, -fWidth - fWidth * nCnt), D3DXVECTOR3(fLength, 0.0f, -fWidth - fWidth * nCnt ), col);
	}

	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	
	// システムファイル
	ReadJsonSystemFile();

	// 編集クラス生成
	if (m_pEdit == nullptr)
	{
		m_pEdit = CEdit::Create("data\\model\\tako_test_001.x");
		m_pEdit->SkinInfoRead();
	}

	return S_OK;
}

//**********************************************
// マネージャー終了処理
//**********************************************
void CManager::Uninit(void)
{
	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{// ヌルでなかったら
		// レンダラーの終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pRenderer != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pRenderer != NULL) > m_pRendererメモリがNULLではありません", "Error", MB_OK);
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{// ヌルでなかったら
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pInputKeyboard != NULL) > m_pInputKeyboardメモリがNULLではありません", "Error", MB_OK);
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{// ヌルでなかったら
		// マウスの終了処理
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (m_pInputMouse != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pInputMouse != NULL) > m_pInputMouseメモリがNULLではありません", "Error", MB_OK);
	}

	// ジョイパッドの破棄
	if (m_pInputJoypad != NULL)
	{// ヌルでなかったら
		// ジョイパッドの終了処理
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	if (m_pInputJoypad != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pInputJoypad != NULL) > m_pInputJoypadメモリがNULLではありません", "Error", MB_OK);
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{// ヌルでなかったら
		// サウンドの終了処理
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}

	if (m_pSound != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pSound != NULL) > m_pSoundメモリがNULLではありません", "Error", MB_OK);
	}

	// デバッグの破棄
	if (m_pDebugprocc != NULL)
	{// ヌルでなかったら
		// デバッグの終了処理
		m_pDebugprocc->Uninit();
		delete m_pDebugprocc;
		m_pDebugprocc = NULL;
	}

	if (m_pDebugprocc != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pDebugprocc != NULL) > m_pDebugproccメモリがNULLではありません", "Error", MB_OK);
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{// ヌルでなかったら
		// デバッグの終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pCamera != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pDebugprocc != NULL) > m_pDebugproccメモリがNULLではありません", "Error", MB_OK);
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{// ヌルでなかったら
		// デバッグの終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pLight != NULL)
	{
		MessageBox(NULL, " void CManager::Uninit() > if (m_pDebugprocc != NULL) > m_pDebugproccメモリがNULLではありません", "Error", MB_OK);
	}

	if (m_pFontMane != NULL)
	{
		m_pFontMane->Uninit();
		delete m_pFontMane;
		m_pFontMane = NULL;
	}

	m_pPause = NULL;	// ポーズのメモリを忘れる

	if (m_pTexture != nullptr)
	{
		m_pTexture->Uninit();
		m_pTexture = nullptr;
	}

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
	m_pRenderer->Draw();		// レンダラーの描画処理
}

//**********************************************
// マネージャーシステムファイル読込
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
	m_sFilePathModel = json["FILEPATH_MODEL"];
	m_sFilePathWriteSkinMesh = json["FILEPATH_WRITE_SKINMESH"];
	m_sFilePathWriteSkinMeshBinary = json["FILEPATH_WRITE_SKINMESH_BINARY"];
	m_sFilePathReadSkinMesh = json["FILEPATH_READ_SKINMESH"];
	m_sFilePathReadSkinMeshBinary = json["FILEPATH_READ_SKINMESH_BINARY"];

	ifs.close();
}