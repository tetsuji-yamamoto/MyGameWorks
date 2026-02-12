//**********************************************
// 
// マネージャー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "sound.h"
#include "debugproc.h"
#include "pause.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "word.h"
#include "string.h"
#include "fontManager.h"

// マネージャークラス
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer* GetRenrere() { return m_pRenderer; };
	static CInputKeyboard* GetInputKeyboard() { return m_pInputKeyboard; };
	static CInputMouse* GetInputMouse() { return m_pInputMouse; };
	static CInputJoypad* GetInputJoypad() { return m_pInputJoypad; };
	static CPlayer* GetPlayer() { return m_pPlayer; };
	static CSound* GetSound() { return m_pSound; };
	static CDebugprocc* GetDebugprocc() { return m_pDebugprocc; };
	static CPause* GetPause() { return m_pPause; };
	static CTexture* GetTexture() { return m_pTexture; };
	static CCamera* GetCamera() { return m_pCamera; };
	static CLight* GetLight() { return m_pLight; };
	static CObject3D* GetObj() { return m_obj3Dtest; };
	static CFontManager* GetFOntMane() { return m_pFontMane; };
	static void ReadJsonSystemFile(void);
	static std::string GetFilePathMotionSkinRead(void) { return m_sFilePathMotionSkinRead; }
	static std::string GetFilePathMotionSkinReadBinary(void) { return m_sFilePathMotionSkinReadBinary; }
	static std::string GetFilePathMotionSkinWrite(void) { return m_sFilePathMotionSkinWrite; }
	static std::string GetFilePathMotionSkinWriteBinary(void) { return m_sFilePathMotionSkinWriteBinary; }

private:
	static CRenderer* m_pRenderer;				// レンダラーへのポインタ
	static CInputKeyboard* m_pInputKeyboard;	// キーボードへのポインタ
	static CInputMouse* m_pInputMouse;			// マウスへのポインタ
	static CInputJoypad* m_pInputJoypad;		// ジョイパッドへのポインタ
	static CPlayer* m_pPlayer;					// プレイヤーへのポインタ
	static CSound* m_pSound;					// サウドへのポインタ
	static CDebugprocc* m_pDebugprocc;			// デバッグのポインタ
	static CPause* m_pPause;					// ポーズのポインタ
	static CTexture* m_pTexture;				// テクスチャのポインタ
	static CCamera* m_pCamera;					// カメラのポインタ
	static CLight* m_pLight;					// ライトのポインタ
	static CFontManager* m_pFontMane;			// フォントマネージャーのポインタ
	static CObject3D* m_obj3Dtest;
	CObjectX* m_ObjectX;					// オブジェクトX
	CObjectBillboard* m_ObjectBillboard;	// オブジェクトビルボード

	static std::string m_sFilePathMotionSkinRead;			// モーション読込用ファイルパス
	static std::string m_sFilePathMotionSkinReadBinary;		// モーション読込用ファイルパスバイナリ
	static std::string m_sFilePathMotionSkinWrite;			// モーション書き込用ファイルパス
	static std::string m_sFilePathMotionSkinWriteBinary;	// モーション書き込用ファイルパスバイナリ
};

#endif // !_MANAGER_H_