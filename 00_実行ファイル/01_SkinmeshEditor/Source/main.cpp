#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "object2D.h"
#include <crtdbg.h>

//グローバル変数宣言
int g_nCountFPS = 0;
bool g_isFullscreen = false;	//ウィンドウを切り替えるためのフラグ
RECT g_windowRect;				//ウィンドウを切り替えるための変数
DEBUGMANAGER g_debugManager;	// デバッグ表示管理

//***********************************************
// メイン関数
//***********************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
	// メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);//メモリリーク検知
	//_CrtSetBreakAlloc(1094);

	WNDCLASSEX wcex =					// ウィンドウクラスの構造体
	{									   
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウィンドウのスタイル
		WindowProc,						// ウィンドウプロシージャ
		0,								// 0にする(通常は使用しない)
		0,								// 0にする(通常は使用しない)	
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		CLASS_NAME,						// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	// 画面サイズの構造体
		
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 現在時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	// ウィンドクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウ左上X座標
		CW_USEDEFAULT,				// ウィンドウ左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウのハンドル
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ生成データ

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);			// クライアント領域を更新

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	CManager* pManager = NULL;// マネージャーへのポインタ

	if (pManager == NULL)
	{// ヌルであれば
		// メモリ確保
		pManager = new CManager;

		if (pManager != NULL)
		{// 確保できていれば
			// レンダラーの初期化
			pManager->Init(hInstance,hWnd,TRUE);
		}
		else
		{
			MessageBox(NULL, " WinMain() > if (pManager != NULL) > pManagerメモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " WinMain() > if(pManager == NULL) > pManagerメモリを確保できませんでした", "Error", MB_OK);
	}

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windoesの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);	// 仮想メッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{// DilectXの処理
			dwCurrentTime = timeGetTime();	// 現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;											// FPSを測定したを保存
				dwFrameCount = 0;														// フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 60フレーム
			{
				dwExecLastTime = dwCurrentTime;
			
				CDebugprocc::Print("FPS : %d\n\n", g_nCountFPS);

				// 更新処理
				pManager->Update();

				// 描画処理
				pManager->Draw();

				dwFrameCount++;	// フレームカウントを加算
			}
		}
	}

	// マネージャーの破棄
	if (pManager != NULL)
	{// ヌルでなかったら
		// マネージャーの終了処理
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	if (pManager != NULL)
	{
		MessageBox(NULL, " WinMain() > if (pManager != NULL) > pManagerメモリがNULLではありません", "Error", MB_OK);
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//***********************************************
// ウィンドウをフルスクリーンに変える処理
//***********************************************
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}

//***********************************************
//ウィンドプロシージャ
//***********************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// 戻り値を格納
	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンド破棄メッセージ

		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:	// ESCが押されたら
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(EM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;

		case VK_F11:
			ToggleFullscreen(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}