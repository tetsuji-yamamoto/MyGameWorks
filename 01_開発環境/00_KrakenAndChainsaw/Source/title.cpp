//*******************************************************
// 
// タイトルシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "title.h"
#include "myMath.h"
#include "vector.h"
#include "debugproc.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"
#include "button.h"
#include "titleLogo.h"
#include "input.h"
#include "sound.h"
#include "effectBlood.h"
#include "kraken.h"
#include "ship.h"
#include "manager.h"
#include "compMotionSkin.h"
#include "ocean.h"
#include "spherSky.h"
#include "compModel.h"
#include "player.h"
#include "compUpdateMotion.h"
#include "porygon2D.h"
#include "json.hpp"
#include "EfffectSplash.h"

using namespace std;
using namespace nlohmann;

//*******************************************************
// タイトルシーンコンストラクタ
//*******************************************************
CTitle::CTitle()
{
	m_nSelect	= SELECT_START;	// セレクトタイプ
	m_fAngle	= 0.0f;			// 角度
}

//*******************************************************
// タイトルシーンデストラクタ
//*******************************************************
CTitle::~CTitle()
{
}

//*******************************************************
// タイトルシーン初期化
//*******************************************************
void CTitle::Init(MODE mode)
{
	// 情報の読み取り
	ReadInfo();

	CreateSky();			// 空生成
	CreateOcean();			// 海生成
	CreateShip();			// 船生成
	CreatePlayer();			// プレイヤー生成
	CreateTitleLogo();		// タイトルロゴ生成
	CreateStartButton();	// スタートボタン生成
	CreateExitButton();		// 終了ボタン生成

	// プレイヤードライブ状態
	GetObj<CPlayer>()->GetComponent<CMotionSkin>()->SetMotion(CPlayer::MOTION_DRIVING,1);

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラ位置設定
	pCamera->SetDistance(m_info.CAMERA_DISTANCE);
	pCamera->SetRotation(D3DXVECTOR3(m_info.CAMERA_ANGLE_X, m_info.CAMERA_ANGLE_Y, m_info.CAMERA_ANGLE_Z));
	pCamera->SetPositionRDest(D3DXVECTOR3(m_info.CAMERA_POSR_X, m_info.CAMERA_POSR_Y, m_info.CAMERA_POSR_Z));
	
	// シーンの初期化
	CScene::Init(mode);

	// 波音を流す
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_WAVE);
}

//*******************************************************
// タイトルシーン終了
//*******************************************************
void CTitle::Uninit(void)
{
	// オブジェクトを消す
	OllDeleteObjs();

	// 自身の破棄
	CScene::Uninit();
}

//*******************************************************
// タイトルシーン更新
//*******************************************************
void CTitle::Update(void)
{
	// カメラの揺れ
	CameraVibration();

	// しぶき
	Splash();

	// フェード移行状態であればこの先処理しない
	if (CManager::GetInstance()->GetFade() != CFade::FADE_NONE)
	{
		return;
	}

	// 入力取得
	CInputKeyboard* pInputKeyBoard = CInputKeyboard::GetkeyBoard();
	CInputJoypad* pInputJoyPad = CManager::GetInstance()->GetInputJoypad();

	// ボタンポインタ取得
	CButton* pStartbutton = GetObj<CButton>(m_info.START_BUTTON_NAME);
	CButton* pExitbutton = GetObj<CButton>(m_info.EXIT_BUTTON_NAME);

	// 上入力
	if (
		pInputKeyBoard->GetTrigger(DIK_W) == true ||
		pInputKeyBoard->GetTrigger(DIK_UP) == true || 
		pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_UP))
	{
		// セレクターデクリメント
		m_nSelect--;
	}
	// 下入力
	else if (
		pInputKeyBoard->GetTrigger(DIK_S) == true ||
		pInputKeyBoard->GetTrigger(DIK_DOWN) == true ||
		pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_DOWN))
	{
		// セレクターインクリメント
		m_nSelect++;
	}

	// 繰り返す
	m_nSelect = Wrap(m_nSelect, (int)SELECT_START, (int)SELECT_EXIT);

	// マウスで触ったか否か
	bool bTouchMouse = false;

	// 触ってないことにする
	pStartbutton->SetIsTouch(false);
	pStartbutton->SetIsPress(false);
	pExitbutton->SetIsTouch(false);
	pExitbutton->SetIsPress(false);

	// ボタン更新
	if (pStartbutton->InspectionMouse() == true)bTouchMouse = true;
	if (pExitbutton->InspectionMouse() == true)bTouchMouse = true;

	// マウスで触れいていない場合
	if (bTouchMouse == false)
	{
		switch (m_nSelect)
		{
			// 開始
		case SELECT_START:
			pStartbutton->SetIsTouch(true);
			break;

			// 終了
		case SELECT_EXIT:
			pExitbutton->SetIsTouch(true);
			break;
		}
	}
	// マウスで触れいていた
	else
	{
		// 触っていたものに対応するセレクターの値にする

		if (pStartbutton->GetIsTouch() == true)m_nSelect = SELECT_START;
		if (pExitbutton->GetIsTouch() == true)m_nSelect = SELECT_EXIT;
	}

		// セレクターで切替
		switch (m_nSelect)
		{
			// 開始
		case SELECT_START:
			if (pInputKeyBoard->GetTrigger(DIK_RETURN) == true ||
				pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_A)||
				pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_B) ||
				pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_START))
			{
				pStartbutton->SetIsPress(true);
			}
			break;

			// 終了
		case SELECT_EXIT:
			if (pInputKeyBoard->GetTrigger(DIK_RETURN) == true ||
				pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_A) ||
				pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_B) ||
				pInputJoyPad->GetkeyTrigger(CInputJoypad::JOYKEY_START))
			{
				pExitbutton->SetIsPress(true);
			}
			break;
		}

		// ボタン更新
		pStartbutton->Update();
		pExitbutton->Update();

		if (pStartbutton != nullptr)
		{
			// 開始押している状況であれば画面を遷移させる
			if (pStartbutton->GetIsPress() == true)
			{
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHAINSAW_ENGIN_STARTUP);
				CManager::GetInstance()->SetFade(CScene::MODE_GAME);
			}
			// 終了ボタンを押している状況であればゲームを終了する
			else if (pExitbutton->GetIsPress() == true)
			{
				// 終了メッセージを出す
				int nID = MessageBox(NULL, "終了しますか？", "終了メッセージ", MB_YESNO);
				if (nID == IDYES)
				{
					PostQuitMessage(0);
				}
			}
		}

#ifdef _DEBUG
		CDebugprocc::Print("SCene : [ TITLE ]\n");
#endif
}

//*******************************************************
// タイトルシーン描画
//*******************************************************
void CTitle::Draw(void)
{
}

//************************************************************************
// 情報の読込
//************************************************************************
void CTitle::ReadInfo(void)
{
	// ファイルを読込
	ifstream ifs("data\\json\\title.json");

	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CTitle::ReadInfo > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// ジェイソンオブジェクト生成
	json json;

	// jsonの情報を読み込む
	ifs >> json;

	// 情報を移し替える
	m_info.START_BUTTON_NAME		= json["START_BUTTON_NAME"];		// スタートボタン名
	m_info.EXIT_BUTTON_NAME			= json["EXIT_BUTTON_NAME"];			// 終了ボタン名
	m_info.TITLE_LOGO_NAME			= json["TITLE_LOGO_NAME"];			// タイトルロゴ名
	m_info.CSVDIVISIONX				= json["CSVDIVISIONX"];				// CSVデータの分割数
	m_info.CSVDIVISIONY				= json["CSVDIVISIONY"];				// CSVデータの分割数
	m_info.CAMERA_DISTANCE			= json["CAMERA_DISTANCE"];			// カメラ距離
	m_info.CAMERA_ANGLE_X			= json["CAMERA_ANGLE_X"];			// カメラ角度X
	m_info.CAMERA_ANGLE_Y			= json["CAMERA_ANGLE_Y"];			// カメラ角度Y
	m_info.CAMERA_ANGLE_Z			= json["CAMERA_ANGLE_Z"];			// カメラ角度Z
	m_info.CAMERA_POSR_X			= json["CAMERA_POSR_X"];			// カメラ注視点X 
	m_info.CAMERA_POSR_Y			= json["CAMERA_POSR_Y"];			// カメラ注視点Y
	m_info.CAMERA_POSR_Z			= json["CAMERA_POSR_Z"];			// カメラ注視点Z
	m_info.CAMERA_POSV_Y			= json["CAMERA_POSV_Y"];			// カメラ注視点Y
	m_info.LOTATION_SPEED			= json["LOTATION_SPEED"];			// カメラ注視点Z
	m_info.RADIUS_VIBRATION			= json["RADIUS_VIBRATION"];			// 揺れ半径
	m_info.START_BUTTON_STR			= json["START_BUTTON_STR"];			// スタートボタン文字
	m_info.EXIT_BUTTON_STR			= json["EXIT_BUTTON_STR"];			// 終了ボタン文字
	m_info.START_BUTTON_POS_X		= json["START_BUTTON_POS_X"];		// スタートボタン位置X
	m_info.START_BUTTON_POS_Y		= json["START_BUTTON_POS_Y"];		// スタートボタン位置X
	m_info.START_BUTTON_POS_Z		= json["START_BUTTON_POS_Z"];		// スタートボタン位置X
	m_info.EXIT_BUTTON_POS_X		= json["EXIT_BUTTON_POS_X"];		// 終了ボタン位置X
	m_info.EXIT_BUTTON_POS_Y		= json["EXIT_BUTTON_POS_Y"];		// 終了ボタン位置X
	m_info.EXIT_BUTTON_POS_Z		= json["EXIT_BUTTON_POS_Z"];		// 終了ボタン位置X
	m_info.START_BUTTON_WIDTH		= json["START_BUTTON_WIDTH"];		// スタートボタン幅
	m_info.START_BUTTON_HEGHT		= json["START_BUTTON_HEGHT"];		// スタートボタン高さ
	m_info.EXIT_BUTTON_WIDTH		= json["EXIT_BUTTON_WIDTH"];		// スタートボタン幅
	m_info.EXIT_BUTTON_HEGHT		= json["EXIT_BUTTON_HEGHT"];		// スタートボタン高さ
	m_info.BUTTON_COLOR_ALPHER		= json["BUTTON_COLOR_ALPHER"];		// ボタンのアルファ値
	m_info.TITLE_LOGO_POS_X			= json["TITLE_LOGO_POS_X"];			// タイトルロゴ位置X
	m_info.TITLE_LOGO_POS_Y			= json["TITLE_LOGO_POS_Y"];			// タイトルロゴ位置Y
	m_info.TITLE_LOGO_POS_Z			= json["TITLE_LOGO_POS_Z"];			// タイトルロゴ位置Z
	m_info.TITLE_LOGO_WIDTH			= json["TITLE_LOGO_WIDTH"];			// タイトルロゴ幅
	m_info.TITLE_LOGO_HEGHT			= json["TITLE_LOGO_HEGHT"];			// タイトルロゴ高さ
	m_info.TITLE_LOGO_TEXTURE_PATH	= json["TITLE_LOGO_TEXTURE_PATH"];	// タイトルロゴテクスチャパス
	m_info.SPLASH_POS_X				= json["SPLASH_POS_X"];				// しぶき位置X
	m_info.SPLASH_POS_Y				= json["SPLASH_POS_Y"];				// しぶき位置Y
	m_info.SPLASH_POS_Z				= json["SPLASH_POS_Z"];				// しぶき位置Z
	m_info.SPLASH_MOVE_X			= json["SPLASH_MOVE_X"];			// しぶき向きX
	m_info.SPLASH_MOVE_Y			= json["SPLASH_MOVE_Y"];			// しぶき向きY
	m_info.SPLASH_MOVE_Z			= json["SPLASH_MOVE_Z"];			// しぶき向きZ
	m_info.SPLASH_RADIUS			= json["SPLASH_RADIUS"];			// しぶき半径
	m_info.SPLASH_GRAVITY			= json["SPLASH_GRAVITY"];			// しぶき重力
	m_info.SPLASH_LIFE				= json["SPLASH_LIFE"];				// しぶき寿命
	m_info.SPLASH_AMOUNT			= json["SPLASH_AMOUNT"];			// しぶき量

	// ファイルを閉じる
	ifs.close();
}

//************************************************************************
// 空の生成
//************************************************************************
void CTitle::CreateSky(void)
{
	// 空生成
	CSphereSky* pSky = CSphereSky::Create(20, 20, 20000.0f);

	// ヴェクターに追加
	AddObj(pSky);
}

//************************************************************************
// 海の生成
//************************************************************************
void CTitle::CreateOcean(void)
{
	// 海生成
	COcean* pOcean = COcean::Create();

	// ヴェクターに追加
	AddObj(pOcean);
}

//************************************************************************
// 船の生成
//************************************************************************
void CTitle::CreateShip(void)
{
	// 船生成
	CShip* pShip = CShip::Create(InitValu::VEC3_NULL);

	// コンポーネントの設定
	pShip->AddComponent	<CModel>	(string("ship"),		"data\\model\\ship\\ship_001.x"						);	// 船モデル
	pShip->AddComponent	<CModel>	(string("collison"),	"data\\model\\ship\\ship_collision_001.x",	false	);	// 船当たり判定モデル

	// ヴェクターに追加
	AddObj(pShip);
}

//************************************************************************
// プレイヤーの生成
//************************************************************************
void CTitle::CreatePlayer(void)
{
	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create(InitValu::VEC3_NULL);

	pPlayer->AddComponent	<CInfo>			();																		// 情報
	pPlayer->AddComponent	<CMotionSkin>	("data\\motion\\krakenAndChainsaw\\MosionSkinInfo_orangeJaket.txt");	// モーションスキン
	pPlayer->AddComponent	<CUpdateMotion>	();																		// モーション更新

	// ヴェクターに追加
	AddObj(pPlayer);
}

//************************************************************************
// スタートボタンの生成
//************************************************************************
void CTitle::CreateStartButton(void)
{
	// 生成用変数
	CButton* pButton;
	CButton::INFO info;

	// 情報設定
	info.colPolygon = Color::_BLACK;
	info.colPolygon.a = m_info.BUTTON_COLOR_ALPHER;
	info.colPolygonEmp = Color::_WHITE;
	info.colWord = Color::_GRAY;
	info.colWordEmp = Color::_YELLOW;
	info.fHeight = m_info.START_BUTTON_HEGHT;
	info.fWidth = m_info.START_BUTTON_WIDTH;
	info.pos.x = m_info.START_BUTTON_POS_X;
	info.pos.y = m_info.START_BUTTON_POS_Y;
	info.pos.z = m_info.START_BUTTON_POS_Z;
	info.rot = InitValu::VEC3_NULL;

	// ボタン生成
	pButton = CButton::Create(info, m_info.START_BUTTON_STR.c_str());

	// 名前設定
	pButton->SetName(m_info.START_BUTTON_NAME);

	// ヴェクターに追加
	AddObj(pButton);
}

//************************************************************************
// 終了ボタンの生成
//************************************************************************
void CTitle::CreateExitButton(void)
{
	// 生成用変数
	CButton* pButton;
	CButton::INFO info;

	// 情報設定
	info.colPolygon = Color::_BLACK;
	info.colPolygon.a = m_info.BUTTON_COLOR_ALPHER;
	info.colPolygonEmp = Color::_WHITE;
	info.colWord = Color::_GRAY;
	info.colWordEmp = Color::_YELLOW;
	info.fHeight = m_info.EXIT_BUTTON_HEGHT;
	info.fWidth = m_info.EXIT_BUTTON_WIDTH;
	info.pos.x = m_info.EXIT_BUTTON_POS_X;
	info.pos.y = m_info.EXIT_BUTTON_POS_Y;
	info.pos.z = m_info.EXIT_BUTTON_POS_Z;
	info.rot = InitValu::VEC3_NULL;

	// ボタン生成
	pButton = CButton::Create(info, m_info.EXIT_BUTTON_STR.c_str());

	// 名前設定
	pButton->SetName(m_info.EXIT_BUTTON_NAME);

	// ヴェクターに追加
	AddObj(pButton);
}

//************************************************************************
// タイトルロゴの生成
//************************************************************************
void CTitle::CreateTitleLogo(void)
{
	// 情報設定
	CPolygon2D::Info info;
	info.col = Color::_WHITE;
	info.fHeight = m_info.TITLE_LOGO_HEGHT;
	info.fWidth = m_info.TITLE_LOGO_WIDTH;
	info.pos.x = m_info.TITLE_LOGO_POS_X;
	info.pos.y = m_info.TITLE_LOGO_POS_Y;
	info.pos.z = m_info.TITLE_LOGO_POS_X;
	info.rot = InitValu::VEC3_NULL;

	// 生成
	CPolygon2D* pPorygon2D = CPolygon2D::Create(info,m_info.TITLE_LOGO_TEXTURE_PATH.c_str());

	// 名前設定
	pPorygon2D->SetName(m_info.TITLE_LOGO_NAME);

	// ヴェクターに追加
	AddObj(pPorygon2D);
}

//************************************************************************
// カメラの揺れ
//************************************************************************
void CTitle::CameraVibration(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 視点の位置取得
	D3DXVECTOR3 posV = pCamera->GetPositionVDest();
	D3DXVECTOR3 posR = pCamera->GetPositionRDest();

	// 角度を足す
	m_fAngle += m_info.LOTATION_SPEED;

	// 角度の正規化
	m_fAngle = my_vector::AngleNormalize(m_fAngle);

	// 揺れを追加
	posV.y = m_info.CAMERA_POSV_Y + sinf(m_fAngle) * sinf(m_fAngle) * m_info.RADIUS_VIBRATION;
	posR.y = m_info.CAMERA_POSR_Y + sinf(m_fAngle) * sinf(m_fAngle) * m_info.RADIUS_VIBRATION;

	// カメラ位置設定
	pCamera->SetPositionVDest(posV);
	pCamera->SetPositionRDest(posR);

	// 視点計算
	pCamera->ArrangeVDest();
}

//************************************************************************
// 水しぶき
//************************************************************************
void CTitle::Splash(void)
{
	// 情報の設定
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	pos.x = m_info.SPLASH_POS_X;
	pos.y = m_info.SPLASH_POS_Y;
	pos.z = m_info.SPLASH_POS_Z;
	move.x = m_info.SPLASH_MOVE_X;
	move.y = m_info.SPLASH_MOVE_Y;
	move.z = m_info.SPLASH_MOVE_Z;
	
	// パーティクル生成
	CEffectSplash::Particle(
		pos,
		move,
		m_info.SPLASH_RADIUS,
		m_info.SPLASH_GRAVITY,
		m_info.SPLASH_LIFE,
		m_info.SPLASH_AMOUNT
	);
}