//****************************************************************************
// 
// 入力処理
// Author tetuji yamamoto
// 
//****************************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "manager.h"

// マクロ定義
#define NUM_KEY_MAX (256)												//キーの最大数
#define MAX_JOYPAD_STICK_ANGLE (32768)									// ジョイパッドのアングル
#define JOYPAD_STICK_DEADZONE (32768 * ((D3DX_PI * 0.2f) / D3DX_PI))	// ジョイパッドのデッドゾーン

// 入力クラス
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void);

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

// キーボードクラス
class CInputKeyboard : public CInput // CInput継承
{
public:
	static constexpr int LIMIT_PRESS = 30;	// トリガーリピートトリガー後の有余フレーム
	static constexpr int LIMIT_RAPID = 1;	// トリガーリピートの連続入力感覚フレーム

	CInputKeyboard();
	~CInputKeyboard();

	HRESULT		Init			(HINSTANCE hInstance, HWND hWnd);
	void		Uninit			(void);
	void		Update			(void);
	bool		GetPressAny		(void)		{ return m_bAnypress; }
	int			GetPressCnt		(int nKey)	{ return m_aInputCnt[nKey]; }
	bool		GetPress		(int nKey)	{ return (m_aKeyState[nKey] & 0x80) ? true : false; }
	bool		GetTrigger		(int nKey)	{ return m_aKeyState[nKey] & 0x80 && !(m_aOldState[nKey] & 0x80) ? true : false; }
	bool		GetRelease		(int nKey)	{ return m_aOldState[nKey] & 0x80 && !(m_aKeyState[nKey] & 0x80) ? true : false; }
	bool		GetTriggerPress	(int nKey);

	static CInputKeyboard* GetkeyBoard(void) { return CManager::GetInstance()->GetInputKeyboard(); }

private:
	BYTE m_aKeyState[NUM_KEY_MAX];	//キーボードのプレス情報
	BYTE m_aOldState[NUM_KEY_MAX];	//キーボードの古いプレス情報
	int m_aInputCnt[NUM_KEY_MAX];	//キーボードの押した情報
	int m_nCntPress;				// 押下カウンター
	int m_nCntRapid;				// 連射カウンター
	bool m_bAnypress;				// 何か押したか否か
};

// マウスクラス
class CInputMouse : public CInput // CInput継承
{
public:
	// マウスボタンの種類列挙
	typedef enum
	{
		MOUSEBUTTON_LEFT = 0,	// 左クリック
		MOUSEBUTTON_RIGHT,		// 模擬クリック
		MOUSEBUTTON_CENTER,		// ホイールボタン
		MOUSEBUTTON_MAX
	}MOUSEBUTTON;

	CInputMouse();
	~CInputMouse();

	HRESULT			Init		(HINSTANCE hInstance, HWND hWnd);
	void			Uninit		(void);
	void			Update		(void);
	bool			GetRepeat	(MOUSEBUTTON mouseButtton)	{ return m_mouse.rgbButtons[mouseButtton] & (0x80) ? true : false; }
	bool			GetTrigger	(MOUSEBUTTON mouseButtton)	{ return m_mouseTrigger.rgbButtons[mouseButtton] & (0x80) ? true : false; }
	bool			GetRelease	(MOUSEBUTTON mouseButtton)	{ return m_mouseRelease.rgbButtons[mouseButtton] & (0x80) ? true : false; }
	D3DXVECTOR3		GetVelocity	(void)						{ return D3DXVECTOR3((float)m_mouseMove.lX, (float)m_mouseMove.lY, (float)m_mouseMove.lZ); }
	D3DXVECTOR3		GetPosition	(void)						{ return D3DXVECTOR3((float)m_mouse.lX, (float)m_mouse.lY, (float)m_mouse.lZ); }

private:
	DIMOUSESTATE m_mouse;			// マウス情報
	DIMOUSESTATE m_mouseMove;		// マウスの移動量情報
	DIMOUSESTATE m_mouseOld;		// 古いマウス情報
	DIMOUSESTATE m_mouseTrigger;	// マウストリガー情報
	DIMOUSESTATE m_mouseRelease;	// マウスリリース情報
};

// ジョイパッドクラス
class CInputJoypad : public CInput // CInput継承
{
public:
	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,			//十字キー上
		JOYKEY_DOWN,			//十字キー下
		JOYKEY_LEFT,			//十字キー左
		JOYKEY_RIGHT,			//十字キー右
		JOYKEY_START,			//スタートボタン
		JOYKEY_BACK,			//バックボタン
		JOYKEY_LSTICK_BUTTON,	//Lスティックボタン
		JOYKEY_RSTICK_BUTTON,	//Rスティックボタン
		JOYKEY_L1,				//L1ボタン
		JOYKEY_R1,				//R1ボタン
		JOYKEY_L2,				//L2トリガー
		JOYKEY_R2,				//R2トリガー
		JOYKEY_A,				//Aボタン
		JOYKEY_B,				//Bボタン
		JOYKEY_X,				//Xボタン
		JOYKEY_Y,				//Yボタン
		JOYKEY_MAX
	}JOYKEY;

	CInputJoypad();
	~CInputJoypad();

	HRESULT			Init			(HINSTANCE hInstance, HWND hWnd);
	void			Uninit			(void);
	void			Update			(void);
	bool			GetkeyPress		(JOYKEY key)	{ return (m_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false; }
	bool			GetkeyTrigger	(JOYKEY key)	{ return (m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false; }
	bool			GetkeyRelease	(JOYKEY key)	{ return (m_joyKeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false; }
	bool			GetkeyRepeat	(JOYKEY key)	{ return (m_joyKeyStateRepeat.Gamepad.wButtons & (0x01 << key)) ? true : false; }
	bool			GetLTTrigger	(void)			{ return (m_joyKeyStateTrigger.Gamepad.bLeftTrigger ? true : false); }
	bool			GetLTRelease	(void)			{ return (m_joyKeyStateRelease.Gamepad.bLeftTrigger ? true : false); }
	bool			GetLTRepeat		(void)			{ return (m_joyKeyStateRepeat.Gamepad.bLeftTrigger ? true : false); }
	bool			GetRTTrigger	(void)			{ return (m_joyKeyStateTrigger.Gamepad.bRightTrigger ? true : false); }
	bool			GetRTRelease	(void)			{ return (m_joyKeyStateRelease.Gamepad.bRightTrigger ? true : false); }
	bool			GetRTRepeat		(void)			{ return (m_joyKeyStateRepeat.Gamepad.bRightTrigger ? true : false); }
	bool			GetLStick		(void);
	bool			GetRStick		(void);
	XINPUT_STATE	GetXInputState	(void)			{ return m_joyKeyState; }

	static CInputJoypad* GetJoypad(void) { return CManager::GetInstance()->GetInputJoypad(); }

private:
	XINPUT_STATE m_joyKeyState;			// ジョイパッドの情報
	XINPUT_STATE m_JoyKeyStateOld;		// ジョイパッドの古い情報
	XINPUT_STATE m_joyKeyStateTrigger;	// ジョイパッドのトリガー情報
	XINPUT_STATE m_joyKeyStateRepeat;	// ジョイパッドのリピート情報
	XINPUT_STATE m_joyKeyStateRelease;	// ジョイパッドのリリース情報
};

#endif
