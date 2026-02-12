//*******************************************************
// 
// タイトルシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "object2D.h"
#include "scene.h"
#include <vector>

// 前方宣言
class CButton;

// ゲームシーンクラス
class CTitle : public CScene	// CScene継承
{
public:

	typedef struct Read_Info
	{
		std::string	START_BUTTON_NAME;					// スタートボタン名
		std::string	EXIT_BUTTON_NAME;					// 終了ボタン名
		std::string	TITLE_LOGO_NAME;					// タイトルロゴ名
		int			CSVDIVISIONX				= 0;	// CSVデータの分割数
		int			CSVDIVISIONY				= 0;	// CSVデータの分割数
		float		CAMERA_DISTANCE				= 0;	// カメラ距離
		float		CAMERA_ANGLE_X				= 0.0f;	// カメラ角度X
		float		CAMERA_ANGLE_Y				= 0.0f;	// カメラ角度Y
		float		CAMERA_ANGLE_Z				= 0.0f;	// カメラ角度Z
		float		CAMERA_POSR_X				= 0.0f;	// カメラ注視点X
		float		CAMERA_POSR_Y				= 0.0f;	// カメラ注視点Y
		float		CAMERA_POSR_Z				= 0.0f;	// カメラ注視点Z
		float		CAMERA_POSV_Y				= 0.0f;	// カメラ注視点Y
		float		LOTATION_SPEED				= 0.0f;	// カメラ注視点Z
		float		RADIUS_VIBRATION			= 0.0f;	// 揺れ半径
		std::string	START_BUTTON_STR;					// スタートボタン文字
		std::string	EXIT_BUTTON_STR;					// 終了ボタン文字
		float		START_BUTTON_POS_X			= 0.0f;	// スタートボタン位置X
		float		START_BUTTON_POS_Y			= 0.0f;	// スタートボタン位置Y
		float		START_BUTTON_POS_Z			= 0.0f;	// スタートボタン位置Z
		float		EXIT_BUTTON_POS_X			= 0.0f;	// 終了ボタン位置X
		float		EXIT_BUTTON_POS_Y			= 0.0f;	// 終了ボタン位置Y
		float		EXIT_BUTTON_POS_Z			= 0.0f;	// 終了ボタン位置Z
		float		START_BUTTON_WIDTH			= 0.0f;	// スタートボタン幅
		float		START_BUTTON_HEGHT			= 0.0f;	// スタートボタン高さ
		float		EXIT_BUTTON_WIDTH			= 0.0f;	// 終了ボタン幅
		float		EXIT_BUTTON_HEGHT			= 0.0f;	// 終了ボタン高さ
		float		BUTTON_COLOR_ALPHER			= 0.0f;	// ボタンのアルファ値
		float		TITLE_LOGO_POS_X			= 0.0f;	// タイトルロゴ位置X
		float		TITLE_LOGO_POS_Y			= 0.0f;	// タイトルロゴ位置Y
		float		TITLE_LOGO_POS_Z			= 0.0f;	// タイトルロゴ位置Z
		float		TITLE_LOGO_WIDTH			= 0.0f;	// タイトルロゴ幅
		float		TITLE_LOGO_HEGHT			= 0.0f;	// タイトルロゴ高さ
		std::string	TITLE_LOGO_TEXTURE_PATH;			// タイトルロゴテクスチャパス
		float		SPLASH_POS_X				= 0.0f;	// しぶき位置X
		float		SPLASH_POS_Y				= 0.0;	// しぶき位置Y
		float		SPLASH_POS_Z				= 0.0;	// しぶき位置Z
		float		SPLASH_MOVE_X				= 0.0;	// しぶき向きX
		float		SPLASH_MOVE_Y				= 0.0;	// しぶき向きY
		float		SPLASH_MOVE_Z				= 0.0;	// しぶき向きZ
		float		SPLASH_RADIUS				= 0.0;	// しぶき半径
		float		SPLASH_GRAVITY				= 0.0;	// しぶき重力
		int			SPLASH_LIFE					= 0;	// しぶき寿命
		int			SPLASH_AMOUNT				= 0;	// しぶき量
	}Read_Info;

	// 選択列挙
	enum
	{
		SELECT_START = 0,	// スタート
		SELECT_EXIT,		// 終了
		SELECT_MAX
	}SELECT;

	CTitle();
	~CTitle();

	HRESULT Init(void) { return S_OK; }
	void Init(MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void ReadInfo			(void);
	void CreateSky			(void);
	void CreateOcean		(void);
	void CreateShip			(void);
	void CreatePlayer		(void);
	void CreateStartButton	(void);
	void CreateExitButton	(void);
	void CreateTitleLogo	(void);
	void CameraVibration	(void);
	void Splash				(void);

	Read_Info	m_info;			// 情報
	int			m_nSelect;		// セレクトタイプ
	float		m_fAngle;		// 角度
};

#endif