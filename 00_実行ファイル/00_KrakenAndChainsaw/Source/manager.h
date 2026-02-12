//**********************************************
// 
// マネージャー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "scene.h"
#include "fade.h"

// 前方宣言
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;
class CSound;
class CDebugprocc;
class CTexture;
class CCamera;
class CLight;
class CFontManager;
class CScreen;
class CCanvas;
class CLoadMachine;
class CPause;

// マネージャークラス
class CManager
{
public:

	// 生成処理
	static void Create(void) 
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CManager;
		}
	}

	// 破棄処理
	static void Delete(void)
	{
		if (m_pInstance != nullptr)
		{
			m_pInstance->Uninit();
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	// 取得処理
	static CManager* GetInstance(void)
	{
		return m_pInstance;
	}

	// シーンの取得処理
	template<class T>
		T* GetScene(void)
	{
		// キャスト出来たら
		if (auto* t = dynamic_cast<T*>(m_pScene))
		{
			return t;
		}

		// 無かった
		return nullptr;
	}

	HRESULT		Init				(HINSTANCE hinstance, HWND hWnd, BOOL bWindow);
	void		Uninit				(void);
	void		Update				(void);
	void		Draw				(void);
	void		DrawFade			(void)	{ if (m_pFade != nullptr) { m_pFade->Draw(); } }
	void		ReadJsonSystemFile	(void);
	void		CangeScene			(CScene::MODE mode);

	CRenderer*		GetRenderer(void)		{ return m_pRenderer; };
	CInputKeyboard*	GetInputKeyboard(void)	{ return m_pInputKeyboard; };
	CInputMouse*	GetInputMouse(void)		{ return m_pInputMouse; };
	CInputJoypad*	GetInputJoypad(void)	{ return m_pInputJoypad; };
	CSound*			GetSound(void)			{ return m_pSound; };
	CDebugprocc*	GetDebugprocc(void)		{ return m_pDebugprocc; };
	CTexture*		GetTexture(void)		{ return m_pTexture; };
	CCamera*		GetCamera(void)			{ return m_pCamera; };
	CLight*			GetLight(void)			{ return m_pLight; };
	CFontManager*	GetFontMane(void)		{ return m_pFontMane; };
	CScreen*		GetScreenShadwS(void)	{ return m_pScreenShadowS; }
	CScene*			GetScene(void)			{ return m_pScene; }
	CScene::MODE	GetMode(void)			{ return m_pScene->GetMode(); }
	CFade::FADE		GetFade(void)			{ return m_pFade->GetFade(); }
	CPause*			GetPause(void)			{ return m_pPause; }
	CCanvas*		GetCanvas(void)			{ return m_pCanvas; }
	CLoadMachine*	Getload(void)			{ return m_pLoad; }

	// セッター
	void SetFade(CScene::MODE mode) { m_pFade->SetFade(mode); }
	void SetMode(CScene::MODE mode);

	// ゲッター
	std::string	GetFilePathMotionPlayer(void)		{ return m_sFilePathMotionPlayer; }
	std::string	GetFilePathMotionPlayerBinary(void)	{ return m_sFilePathMotionPlayerBinary; }
	std::string	GetFilePathMotionShip(void)			{ return m_sFilePathMotionShip; }
	std::string	GetFilePathMotionShipBinary(void)	{ return m_sFilePathMotionShipBinary; }
	std::string	GetFilePathMotionKrakenBinary(void)	{ return m_sFilePathMotionKrakenBinary; }
	std::string	GetFilePathMotionKraken(void)		{ return m_sFilePathMotionKraken; }
	float		GetMovementPlayer(void)				{ return m_fMovementPlayer; }
	float		GetFrictionPlayer(void)				{ return m_fFrictionPlayer; }
	float		GetGravityPlayer(void)				{ return m_fGravityPlayer; }
	float		GetLowLimit(void)					{ return m_fLowLimit; }
	int			GetIDShader(void)					{ return m_nIDShader;}
	int			GetIDPramHandlMtxWorld(void)		{ return m_nIDPramHandlMtxWorld;}
	int			GetIDPramHandlMtxProj(void)			{ return m_nIDPramHandlMtxProj; }
	int			GetIDPramHandlMtxView(void)			{ return m_nIDPramHandlMtxView;}
	int			GetIDPramHandlVecLight(void)		{ return m_nIDPramHandlVecLight; }
	int			GetIDPramHandlDeffuse(void)			{ return m_nIDPramHandlDeffuse; };

	void SeitchIsPause(void) { m_bPause = !m_bPause; }
private:
	CManager();
	~CManager();

	static CManager* m_pInstance;	// インスタンス

	CRenderer*		m_pRenderer;			// レンダラーへのポインタ
	CInputKeyboard*	m_pInputKeyboard;		// キーボードへのポインタ
	CInputMouse*	m_pInputMouse;			// マウスへのポインタ
	CInputJoypad*	m_pInputJoypad;			// ジョイパッドへのポインタ
	CSound*			m_pSound;				// サウドへのポインタ
	CDebugprocc*	m_pDebugprocc;			// デバッグのポインタ
	CTexture*		m_pTexture;				// テクスチャのポインタ
	CCamera*		m_pCamera;				// カメラのポインタ
	CLight*			m_pLight;				// ライトのポインタ
	CFontManager*	m_pFontMane;			// フォントマネージャーのポインタ
	CScreen*		m_pScreenShadowS;		// ステンシルシャドウに使うポリゴン
	CScene*			m_pScene;				// シーンポインタ
	CFade*			m_pFade;				// フェードポインタ
	CCanvas*		m_pCanvas;				// キャンバスポインタ
	CLoadMachine*	m_pLoad;				// 読込ポインタ
	CPause*			m_pPause;				// ポーズ

	std::string	m_sFilePathMotionPlayer;		// 読み込むプレイヤーのモーションパス
	std::string	m_sFilePathMotionPlayerBinary;	// 読み込むプレイヤーのモーションパス
	std::string	m_sFilePathMotionShip;			// 読み込む船のモーションパス
	std::string	m_sFilePathMotionShipBinary;	// 読み込む船のモーションパス
	std::string	m_sFilePathMotionKraken;		// 読み込む船のモーションパス
	std::string	m_sFilePathMotionKrakenBinary;	// 読み込む船のモーションパス
	
	float m_fMovementPlayer;					// プレイヤーの移動量
	float m_fFrictionPlayer;					// プレイヤーの摩擦係数
	float m_fGravityPlayer;						// プレイヤーの重力
	float m_fLowLimit;							// 限界低度
	
	bool m_bPause;	// ポーズの有無
	
	int m_nIDShader;				// シェーダーID
	int m_nIDPramHandlMtxWorld;	// パラメータハンドルID
	int m_nIDPramHandlMtxProj;	// パラメータハンドルID
	int m_nIDPramHandlMtxView;	// パラメータハンドルID
	int m_nIDPramHandlVecLight;	// パラメータハンドルID
	int m_nIDPramHandlDeffuse;	// パラメータハンドルID
};

#endif // !_MANAGER_H_