//**********************************************
// 
// マネージャー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"
#include "light.h"
#include "camera.h"
#include "fontManager.h"
#include "screen.h"
#include "scene.h"
#include "sceneTest.h"
#include "fade.h"
#include "pause.h"
#include "pause.h"
#include "line.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "managerCollision.h"
#include "managerComponent.h"
#include "debugPoint.h"
#include "debugCapsule.h"
#include "resultGameOver.h"
#include "resultGameClear.h"
#include "ShaderManager.h"

using namespace nlohmann;
using namespace std;

// 静的メンバー変数宣言
CManager* CManager::m_pInstance = nullptr;	// インスタンス

//**********************************************
// マネージャーコンストラクタ
//**********************************************
CManager::CManager()
{
	m_pRenderer					= nullptr;					// レンダラー
	m_pInputKeyboard			= nullptr;					// キーボード
	m_pInputMouse				= nullptr;					// マウス
	m_pInputJoypad				= nullptr;					// ジョイパッド
	m_pSound					= nullptr;					// サウンド
	m_pDebugprocc				= nullptr;					// デバッグ
	m_pTexture					= nullptr;					// テクスチャ
	m_pLight					= nullptr;					// ライト
	m_pCamera					= nullptr;					// カメラ
	m_pFontMane					= nullptr;					// フォント
	m_pScreenShadowS			= nullptr;					// ステンシルシャドウ用のポリゴンポインタ
	m_pScene					= nullptr;					// シーンポインタ
	m_pFade						= nullptr;					// フェードポインタ
	m_pCanvas					= nullptr;					// キャンバスポインタ
	m_pPause					= nullptr;					// ポーズ
	m_fMovementPlayer			= 0.0f;						// プレイヤーの移動量
	m_fFrictionPlayer			= 0.0f;						// プレイヤーの摩擦係数
	m_fGravityPlayer			= 0.0f;						// プレイヤーの重力
	m_fLowLimit					= 0.0f;						// 限界低度
	m_bPause					= false;					// ポーズの有無
	m_nIDShader					= CShaderManager::ID_INIT;	// シェーダーID
	m_nIDShader					= CShader::ID_INIT;			// シェーダーID
	m_nIDPramHandlMtxWorld		= CShader::ID_INIT;			// パラメータハンドルID
	m_nIDPramHandlMtxProj		= CShader::ID_INIT;			// パラメータハンドルID
	m_nIDPramHandlMtxView		= CShader::ID_INIT;			// パラメータハンドルID
	m_nIDPramHandlVecLight		= CShader::ID_INIT;			// パラメータハンドルID
	m_nIDPramHandlDeffuse		= CShader::ID_INIT;			// パラメータハンドルID
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
	// レンダラー生成
	m_pRenderer = new CRenderer;
	if (m_pRenderer != nullptr)
	{
		// レンダラーの初期化
		if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
		{
			MessageBox(NULL, " CManager::Init() > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}

	// キーボード生成
	m_pInputKeyboard = new CInputKeyboard;
	if (m_pInputKeyboard != nullptr)
	{
		// キーボードの初期化
		if (FAILED(m_pInputKeyboard->Init(hinstance, hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}

	// マウス生成
	m_pInputMouse = new CInputMouse;
	if (m_pInputMouse != nullptr)
	{
		// マウスの初期化
		if (FAILED(m_pInputMouse->Init(hinstance, hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}

	// ジョイパッド生成
	m_pInputJoypad = new CInputJoypad;
	if (m_pInputJoypad != nullptr)
	{
		// ジョイパッドの初期化
		if (FAILED(m_pInputJoypad->Init(hinstance, hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}

	// サウンド生成
	m_pSound = new CSound;
	if (m_pSound != nullptr)
	{
		// サウンドの初期化
		if (FAILED(m_pSound->InitSound(hWnd)))
		{
			MessageBox(NULL, " CManager::Init() > 初期化できませんでした", "Error", MB_OK);
			return -1;
		}
	}

	// デバッグプロシージャの生成
	m_pDebugprocc = new CDebugprocc;
	if (m_pDebugprocc != nullptr)
	{
		// デバッグの初期化
		m_pDebugprocc->Init();
	}

	// カメラの生成
	m_pCamera = new CCamera;
	if (m_pCamera != nullptr)
	{
		// カメラの初期化
		m_pCamera->Init();
	}

	// ライトの生成
	m_pLight = new CLight;
	if (m_pLight != nullptr)
	{
		// ライトの初期化
		m_pLight->Init();
	}

	// ステンシルシャドウ用ポリゴン生成
	if (m_pScreenShadowS == nullptr)
	{
		m_pScreenShadowS = CScreen::Create(D3DXCOLOR(0.0f,0.0f,0.0f,0.7f));
	}

	// フェード生成
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(CScene::MODE_TITLE);
	}

	// フォントマネージャー生成
	m_pFontMane = CFontManager::Create();
	
	// テクスチャ生成
	m_pTexture = CTexture::Create();

	// デバッグ用点生成
	CDebugPoint::Create();

	// デバッグ用カプセル生成
	CDebugCapsule::Create();
	
	// コンポーネントマネージャ生成
	CManagerComponent::Create();

	// 当たり判定マネージャ生成
	CManagerCollision::Create();

	// シェーダーマネージャー生成
	CShaderManager::Create();

#ifdef _DEBUG
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
		CLine::Create(D3DXVECTOR3(-fLength, 0.0f, -fWidth - fWidth * nCnt), D3DXVECTOR3(fLength, 0.0f, -fWidth - fWidth * nCnt), col);
	}

	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	CLine::Create(InitValu::VEC3_NULL, D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif

	// システムファイルを読み込む
	ReadJsonSystemFile();

	// シェーダーの追加
	m_nIDShader = CShaderManager::GetInstance()->AddShader(std::string("toon.hlsl"), std::string("StandardDraw"));
	CShader* pShader = CShaderManager::GetInstance()->GetShader(m_nIDShader);

	// パラメータハンドルの追加
	m_nIDPramHandlMtxWorld = pShader->AddParameterHandle("g_mtxWorld");
	m_nIDPramHandlMtxProj= pShader->AddParameterHandle("g_Proj");
	m_nIDPramHandlMtxView = pShader->AddParameterHandle("g_View");
	m_nIDPramHandlVecLight = pShader->AddParameterHandle("g_vecLight");
	m_nIDPramHandlDeffuse = pShader->AddParameterHandle("g_Deffuse");

	// モード切替
	SetMode(CScene::MODE_TITLE);

	return S_OK;
}

//**********************************************
// マネージャー終了処理
//**********************************************
void CManager::Uninit(void)
{
	// キーボードの破棄
	if (m_pInputKeyboard != nullptr)
	{
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = nullptr;
	}

	// マウスの破棄
	if (m_pInputMouse != nullptr)
	{
		// マウスの終了処理
		m_pInputMouse->Uninit();
		m_pInputMouse = nullptr;
	}

	// ジョイパッドの破棄
	if (m_pInputJoypad != nullptr)
	{
		// ジョイパッドの終了処理
		m_pInputJoypad->Uninit();
		m_pInputJoypad = nullptr;
	}

	// デバッグの破棄
	if (m_pDebugprocc != nullptr)
	{
		// デバッグの終了処理
		m_pDebugprocc->Uninit();
		m_pDebugprocc = nullptr;
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		// デバッグの終了処理
		m_pCamera->Uninit();
		m_pCamera = nullptr;
	}

	// ライトの破棄
	if (m_pLight != nullptr)
	{
		// デバッグの終了処理
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	// フォントの破棄
	if (m_pFontMane != nullptr)
	{
		m_pFontMane->Uninit();
		m_pFontMane = nullptr;
	}

	//	テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Uninit();
		m_pTexture = nullptr;
	}

	// ステンシルシャドウ用ポリゴン生成
	if (m_pScreenShadowS != nullptr)
	{
		m_pScreenShadowS->Uninit();
		m_pScreenShadowS = nullptr;
	}

	// フェードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// ポーズの破棄
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		// レンダラーの終了処理
		m_pRenderer->Uninit();
		m_pRenderer = nullptr;
	}

	// サウンドの破棄
	if (m_pSound != nullptr)
	{
		// サウンドの終了処理
		m_pSound->UninitSound();
		m_pSound = nullptr;
	}

	// デバッグ用点破棄
	CDebugPoint::Delete();

	// デバッグ用カプセル破棄
	CDebugCapsule::Delete();

	// コンポーネントマネージャ破棄
	CManagerComponent::Delete();

	// 当たり判定マネージャ削除
	CManagerCollision::Delete();

	// シェーダーマネージャー削除
	CShaderManager::Delete();
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
	m_pFade->Update();			// フェード

	// ポーズしているか否か
	bool bPause = false;

	// ポーズ
	if (m_pPause != nullptr)
	{
		// ポーズの更新
		m_pPause->Update();

		// ポーズ状況取得
		bPause = m_pPause->GetPause();
	}

	// ポーズをしていなければ処理する
	if (m_bPause == false)
	{
		// レンダラーの更新処理
		m_pRenderer->Update();

		// コンポーネント更新
		CManagerComponent::GetInstance()->Update();

		// コンポーネント当たり判定前
		CManagerComponent::GetInstance()->HitBefore();

		// コンポーネント当たり判定
		CManagerComponent::GetInstance()->Collision();

		// コンポーネント修正
		CManagerComponent::GetInstance()->Correction();
	}

#ifdef _DEBUG
	// デバッグキー

	// フェードの更新
	if (m_pFade != nullptr)
	{
		// Lシフトを押しながら
		if (m_pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			// タイトル
			if (m_pInputKeyboard->GetTrigger(DIK_1) == true)
			{
				m_pFade->SetFade(CScene::MODE_TITLE);
			}
			// ゲーム
			else if (m_pInputKeyboard->GetTrigger(DIK_2) == true)
			{
				// 現在とは逆のポーズにする
				m_pFade->SetFade(CScene::MODE_GAME);
			}
			// リザルト
			else if (m_pInputKeyboard->GetTrigger(DIK_3) == true)
			{
				// 現在とは逆のポーズにする
				m_pFade->SetFade(CScene::MODE_RESULT);
			}
			// リザルト
			else if (m_pInputKeyboard->GetTrigger(DIK_4) == true)
			{
				// 現在とは逆のポーズにする
				m_pFade->SetFade(CScene::MODE_RESULT_GAMEOVER);
			}
			// リザルト
			else if (m_pInputKeyboard->GetTrigger(DIK_5) == true)
			{
				// 現在とは逆のポーズにする
				m_pFade->SetFade(CScene::MODE_RESULT_GAMECLEAR);
			}
		}

		if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
		{
			CManager::GetInstance()->SeitchIsPause();
		}

		// デバッグ球Zテスト切替
		if (m_pInputKeyboard->GetTrigger(DIK_F1) == true)
		{
			CDebugPoint::GetInstance()->SwitchIsZtest();
			CDebugCapsule::GetInstance()->SwitchIsZtest();
		}

		// デバッグ球表示切替
		if (m_pInputKeyboard->GetTrigger(DIK_F2) == true)
		{
			CDebugPoint::GetInstance()->SwitchIsDisp();
			CDebugCapsule::GetInstance()->SwitchIsDisp();
		}

		// デバッグ球表示切替
		if (m_pInputKeyboard->GetTrigger(DIK_F3) == true)
		{
			CDebugCapsule::GetInstance()->SwitchIsWire();
		}
	}

	// 右クリック中注視点操作
	if (m_pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_LEFT) == true)
	{
		m_pCamera->OperateMousePosV();
	}
	// 左クリック中注視点操作
	else if (m_pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		m_pCamera->OperateMousePosR();
	}
	// 中ボタンスライド操作
	else if (m_pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		m_pCamera->OperateMouseSlide();
	}

	// マウスで距離を変える処理
	m_pCamera->OperateMouseDistance();
#endif
}

//**********************************************
// マネージャー描画処理
//**********************************************
void CManager::Draw(void)
{
#ifdef _DEBUG
	m_pCamera->Debug();			// カメラのデバッグ表示
	CObject::DrawDebug();		// オブジェクトのデバッグ表示
	CTexture::DebugTexture();	// テクスチャンのデバッグ表示
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 設定用変数
	D3DXMATRIX projectionMtx, viewMtx;
	D3DVIEWPORT9 viewport;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR4 color;

	// ライト取得
	CLight* pLight = CManager::GetInstance()->GetLight();

	pDevice->GetViewport(&viewport);								// ビューポート取得
	pDevice->GetTransform(D3DTS_PROJECTION, &projectionMtx);		// プロジェクションマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &viewMtx);					// ビューマトリックス取得
	vecLight = D3DXVECTOR4(pLight->GetLight(0)->Direction, 0.0f);	// ライト向き取得

	// シェーダーの取得
	int nIDShader = CManager::GetInstance()->GetIDShader();
	CShader* pShaderClass = CShaderManager::GetInstance()->GetShader(nIDShader);
	LPD3DXEFFECT pShader = pShaderClass->GetShader();

	// テクニックの設定
	D3DXHANDLE tech = pShaderClass->GetTechniqueHandle();
	pShader->SetTechnique(tech);

	// パラメータ(グローバル変数の設定)
	int nIDMtxWorld = GetIDPramHandlMtxWorld();
	int nIDMtxView = GetIDPramHandlMtxView();
	int nIDMtxProj = GetIDPramHandlMtxProj();
	int nIDVecLight = GetIDPramHandlVecLight();
	int nIDDeffuse = GetIDPramHandlDeffuse();
	pShader->SetMatrix(pShaderClass->GetParamaterHandleHandle(nIDMtxView), &viewMtx);
	pShader->SetMatrix(pShaderClass->GetParamaterHandleHandle(nIDMtxProj), &projectionMtx);
	pShader->SetVector(pShaderClass->GetParamaterHandleHandle(nIDVecLight), &vecLight);

	// レンダラーの描画処理
	m_pRenderer->Draw();
}

//**********************************************
// マネージャーシーンのモード設定
//**********************************************
void CManager::SetMode(CScene::MODE mode)
{
	// 現在のモード破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// ポーズの破棄
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	// オ死亡フラグが立っているブジェクトの全削除
	CObject::DeleteAll();

	// 新しいモード生成
	CangeScene(mode);
}

//***********************************************
// 画面遷移処理
//***********************************************
void CManager::CangeScene(CScene::MODE mode)
{
	// シーン削除
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// 当たり判定マネージャ削除
	CManagerCollision::Delete();

	// 当たり判定マネージャ削除
	CManagerCollision::Create();

	// 種類によってインスタンス生成
	switch (mode)
	{
		// タイトル
	case CScene::MODE_TITLE:
		m_pScene = new CTitle;
		break;

		// チュートリアル
	case CScene::MODE_TUTORIAL:
		m_pScene = new CTutorial;
		break;

		// ゲーム
	case CScene::MODE_GAME:
		m_pScene = new CGame;
		break;

		// リザルト
	case CScene::MODE_RESULT:
		m_pScene = new CResult;
		break;

		// リザルトゲームオーバー
	case CScene::MODE_RESULT_GAMEOVER:
		m_pScene = new CResultGameOver;
		break;

		// リザルトゲームクリア
	case CScene::MODE_RESULT_GAMECLEAR:
		m_pScene = new CResultGameClear;
		break;

		// シーンテスト
	case CScene::MODE_TEST:
		m_pScene = new CTestScene;
		break;

	default:
		break;
	}

	// 初期化
	m_pScene->Init(mode);
}

//**********************************************
// マネージャーシステムファイル読込
//**********************************************
void CManager::ReadJsonSystemFile(void)
{
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

	// 情報を移し替える
	m_sFilePathMotionPlayer = json["FILEPATH_MOTION_PLAYER"];
	m_sFilePathMotionPlayerBinary = json["FILEPATH_MOTION_PLAYER_BINARY"];
	m_sFilePathMotionShip = json["FILEPATH_MOTION_SHIP"];
	m_sFilePathMotionShipBinary = json["FILEPATH_MOTION_SHIP_BINARY"];
	m_sFilePathMotionKrakenBinary = json["FILEPATH_MOTION_KRAKEN_BINARY"];
	m_sFilePathMotionKraken = json["FILEPATH_MOTION_KRAKEN"];
	m_fMovementPlayer = json["FLOAT_MOVEMENT_PLAYER"];
	m_fFrictionPlayer = json["FLOAT_FRICTION_PLAYER"];
	m_fGravityPlayer = json["FLOAT_GRAVITY_PLAYER"];
	m_fLowLimit = json["FLOAT_LOWLIMIT"];

	ifs.close();
}