//****************************************************************************
// 
// ダイレクトX
// Author tetuji yamamoto
// 
//****************************************************************************

#ifndef _MAIN_H_ //このマクロ定義がされてなかったら
#define _MAIN_H_ //二重インクルード帽子のマクロ定義

// マクロ定義
#define CLASS_NAME "WindowClass"		//ウィンドウクラスの名前
#define WINDOW_NAME "SkinMeshEditor"	//ウィンドウの名前(キャプションに表示)

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif