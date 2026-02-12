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

// 前方宣言
class CEdit;	// 編集クラス

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
	static CSound* GetSound() { return m_pSound; };
	static CDebugprocc* GetDebugprocc() { return m_pDebugprocc; };
	static CPause* GetPause() { return m_pPause; };
	static CTexture* GetTexture() { return m_pTexture; };
	static CCamera* GetCamera() { return m_pCamera; };
	static CLight* GetLight() { return m_pLight; };
	static CObject3D* GetObj() { return m_obj3Dtest; };
	static CFontManager* GetFOntMane() { return m_pFontMane; };
	static CEdit* GetEdit() { return m_pEdit; };
	static std::string GetFilePathModel(void) { return m_sFilePathModel; }
	static std::string GetFilePathReadSkinMesh(void) { return m_sFilePathReadSkinMesh; }
	static std::string GetFilePathReadSkinMeshBinary(void) { return m_sFilePathReadSkinMeshBinary; }
	static std::string GetFilePathWriteSkinMesh(void) { return m_sFilePathWriteSkinMesh; }
	static std::string GetFilePathWriteSkinMeshBinary(void) { return m_sFilePathWriteSkinMeshBinary; }
	static void ReadJsonSystemFile(void);

private:
	static CRenderer* m_pRenderer;				// レンダラーへのポインタ
	static CInputKeyboard* m_pInputKeyboard;	// キーボードへのポインタ
	static CInputMouse* m_pInputMouse;			// マウスへのポインタ
	static CInputJoypad* m_pInputJoypad;		// ジョイパッドへのポインタ
	static CSound* m_pSound;					// サウドへのポインタ
	static CDebugprocc* m_pDebugprocc;			// デバッグのポインタ
	static CPause* m_pPause;					// ポーズのポインタ
	static CTexture* m_pTexture;				// テクスチャのポインタ
	static CCamera* m_pCamera;					// カメラのポインタ
	static CLight* m_pLight;					// ライトのポインタ
	static CFontManager* m_pFontMane;			// フォントマネージャーのポインタ
	static CObject3D* m_obj3Dtest;
	CObjectX* m_ObjectX;							// オブジェクトX
	CObjectBillboard* m_ObjectBillboard;			// オブジェクトビルボード
	static std::string m_sFilePathModel;				// 読み込むモデルパス
	static std::string m_sFilePathReadSkinMesh;			// 読み込むスキン情報パス
	static std::string m_sFilePathReadSkinMeshBinary;	// 読み込むスキン情報パスバイナリ
	static std::string m_sFilePathWriteSkinMesh;		// 書き込むスキン情報パス
	static std::string m_sFilePathWriteSkinMeshBinary;	// 書き込むスキン情報パスバイナリ
	static CEdit* m_pEdit;	// 編集クラス
};

#endif // !_MANAGER_H_