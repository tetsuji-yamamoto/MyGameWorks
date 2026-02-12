//**********************************************
// 
// レンダラー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "renderer.h"
#include "object.h"
#include "object3D.h"
#include "manager.h"
#include "myMath.h"
#include "shadowS.h"
#include "fade.h"
#include "screen.h"
#include "camera.h"
#include "debugproc.h"
#include "debugPoint.h"
#include "debugCapsule.h"

// 静的メンバー変数宣言
LPDIRECT3DDEVICE9 CRenderer::m_pD3DDevice = nullptr;

//**********************************************
// レンダラーコンストラクタ
//**********************************************
CRenderer::CRenderer()
{
	m_pD3D = nullptr;										// Direct3Dオブジェクトへのポインタ
	ZeroMemory(m_apTextureMT,sizeof(m_apTextureMT));	// レンダリングターゲット用テクスチャ
	ZeroMemory(m_apRenderMT, sizeof(m_apRenderMT));		// テクスチャレンダリング用インターーフェース
	m_pZBuffMT = nullptr;									// テクスチャレンダリング用Zバッファ
	ZeroMemory(&m_viewPortMT,sizeof(D3DVIEWPORT9)) ;	// テクスチャレンダリング用ビューポート
	ZeroMemory(m_apScreen, sizeof(m_apScreen));			// 画面用ポリゴン
	m_bBlur = false;									// ぼかしの有無
	m_bBlurUp = false;									// 拡大ぼかしの有無
	m_nBlurTime = 0;									// ぼかす時間
	m_nCntBulur = 0;									// ぼかしカウンター
}

//**********************************************
// レンダラーデストラクタ
//**********************************************
CRenderer::~CRenderer()
{
}

//**********************************************
// レンダラー初期化処理
//**********************************************
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	// Direc3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ高さ
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									//
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//
	d3dpp.EnableAutoDepthStencil = TRUE;						//
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// Zバッファ24bit、ステンシルバッファ8bit
	d3dpp.Windowed = bWindow;									//
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理をハードウェア,頂点処理はCPUで行う)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// プレイヤーの中に透明度を加える
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンブラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// 配列分回す
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// レンダリングターゲット用テクスチャの生成
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,			// テクスチャ幅
			SCREEN_HEIGHT,			// テクスチャ高さ
			1,						// ミップマップレヴェル
			D3DUSAGE_RENDERTARGET,	// 
			D3DFMT_X8R8G8B8,		// ピクセルフォーマット
			D3DPOOL_DEFAULT,		// メモリ管理フラグ
			&m_apTextureMT[nCnt],	// 
			NULL					// 
		);

		// テクスチャレンダリング用インターフェースの生成
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);
	}

	// テクスチャレンダリング用のZバッファの生成
	m_pD3DDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pZBuffMT,
		NULL
	);

	// 配列分回す
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// 現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファを取得
		m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		// レンダリングターゲットを生成したテクスチャに設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Zバッファを生成したZバッファに生成
		m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

		// レンダリングターゲット用テクスチャのクリア
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// レンダリングターゲットをもとに戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファをもとに戻す
		m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
	}

	// テクスチャレンダリング用ビューポートの設定
	m_viewPortMT.X = 0;
	m_viewPortMT.Y = 0;
	m_viewPortMT.Width = SCREEN_WIDTH;
	m_viewPortMT.Height = SCREEN_HEIGHT;
	m_viewPortMT.MinZ = 0.0f;
	m_viewPortMT.MaxZ = 1.0f;

	// 画面用ポリゴンの生成
	m_apScreen[0] = CScreen::Create(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apScreen[1] = CScreen::Create(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//**********************************************
// レンダラー終了処理
//**********************************************
void CRenderer::Uninit(void)
{
	// オブジェクトの破棄
	CObject::ReleaseAll();

	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	//Direct3Dオブジェクト
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// 配列分回す
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// レンダリングターゲット用テクスチャの破棄
		if (m_apTextureMT[nCnt] != nullptr)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = nullptr;
		}

		// テクスチャレンダリング用インターフェースの破棄
		if (m_apRenderMT[nCnt] != nullptr)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = nullptr;
		}
	}

	// テクスチャレンダリング用Zバッファの破棄
	if (m_pZBuffMT != nullptr)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = nullptr;
	}

	// 配列分回す
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// 画面ポリゴンの破棄
		m_apScreen[nCnt]->Uninit();
		m_apScreen[nCnt] = nullptr;
	}

	// 自身の破棄
	delete this;
}

//**********************************************
// レンダラー更新処理
//**********************************************
void CRenderer::Update(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// 全オブジェクトの更新
	CObject::UpdateAll();
	

	//// ブラーのオンオフ
	//if (pInputKeyboard->GetTrigger(DIK_Z) == true)
	//{
	//	SetBlur(D3DXCOLOR(1.0f,1.0f,1.0f,0.99f));
	//}

	//// 拡大ブラー
	//if (pInputKeyboard->GetTrigger(DIK_X) == true)
	//{
	//	// 拡大ブラー設定
	//	SetBlurUp(180,1.5f,D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f));
	//}

	//// 拡大ブラーの更新
	//UpdateBulrUp();

	// 画面ポリゴンの描画
	//m_apScreen[0]->Update();
	/*m_apScreen[1]->Update();*/
}

//**********************************************
// レンダラー描画処理
//**********************************************
void CRenderer::Draw(void)
{
	// デバッグプロック取得
	CDebugprocc* pDebugproc = CManager::GetInstance()->GetDebugprocc();
	
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	
	// ステンシルシャドウ用ポリゴン取得
	CScreen* pScreenShadowS = CManager::GetInstance()->GetScreenShadwS();

	// キャンバス取得
	CCanvas* pCanvasu = CManager::GetInstance()->GetCanvas();

	/*LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	LPDIRECT3DSURFACE9 pRenderWK;
	LPDIRECT3DTEXTURE9 pTextureWK;*/

	// 画面クリア(バッファバッファ&のクリア）
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		//// 現在のレンダリングターゲットを取得(保存)
		//m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//// 現在のZバッファを取得
		//m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		//// レンダーターゲットをテクスチャ[0]変更する
		//CManager::GetInstance()->GetRenderer()->ChangeTarget(0,D3DXVECTOR3(0.0f, 50.0f, -450.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		//// テクスチャのクリア
		//m_pD3DDevice->Clear(0, NULL,
		//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		//	D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// カメラのセット
		pCamera->SetCamera();

		// オブジェクトの描画
		CObject::DrawAll();

		// デバッグ位置描画
		CDebugPoint::GetInstance()->Draw();

		// デバッグ用カプセル描画
		CDebugCapsule::GetInstance()->Draw();

		//// フィードバック用ポリゴンにテクスチャ[1]を張り付けて描画
		//m_apScreen[1]->DrawAlpherTest(m_apTextureMT[1]);

		//// レンダリングターゲットをもとに戻す
		//m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//// Zバッファをもとに戻す
		//m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

		//// ポリゴンにテクスチャ[0]をを張りつけて描画
		//m_apScreen[0]->Draw(m_apTextureMT[0]);

		// フェードの描画
		CManager::GetInstance()->DrawFade();

		// デバッグ表示の描画処理
		pDebugproc->Draw();

		//// テクスチャ[0]とテクスチャ[1]を入れ替える
		//pTextureWK = m_apTextureMT[0];
		//m_apTextureMT[0] = m_apTextureMT[1];
		//m_apTextureMT[1] = pTextureWK;

		//pRenderWK = m_apRenderMT[0];
		//m_apRenderMT[0] = m_apRenderMT[1];
		//m_apRenderMT[1] = pRenderWK;

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//**********************************************
// レンダラーレンダリングターゲットの変更
//**********************************************
void CRenderer::ChangeTarget(int nIdxTexture,D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	D3DXMATRIX mtxView, mtxProjection;
	float fAspect;

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nIdxTexture]);

	// Zバッファを生成したZバッファに生成
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// テクスチャレンダリング用ビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewPortMT);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &vecU);

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクトマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	fAspect = (float)m_viewPortMT.Width / (float)m_viewPortMT.Height;
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		fAspect,
		10.0f,
		10000.0f);

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//**********************************************
// レンダラーテクスチャーMTのクリア処理
//**********************************************
void CRenderer::ClearTextureMT(void)
{
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// 配列分回す
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{

		// 現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファを取得
		m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		// レンダリングターゲットを生成したテクスチャに設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Zバッファを生成したZバッファに生成
		m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

		// レンダリングターゲット用テクスチャのクリア
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(255, 100, 100, 100), 1.0f, 0);

		// レンダリングターゲットをもとに戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファをもとに戻す
		m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
	}
}

//**********************************************
// レンダラーぼかしの設定
//**********************************************
void CRenderer::SetBlur(D3DXCOLOR bulrColor)
{
	// 今の状態と逆の状態にする
	m_bBlur = !m_bBlur;

	// ぼかす状態であれば
	if (m_bBlur == true)
	{
		// 色を設定
		m_apScreen[1]->SetColor(bulrColor);	
	}
	// でなければ
	else
	{
		// 色を設定
		m_apScreen[1]->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));	
	}
}

//**********************************************
// レンダラーぼかしの設定
//**********************************************
void CRenderer::SetBlurUp(int nBlurTime, float fScaleValue,D3DXCOLOR bulrColor)
{
	// 拡大ぼかし状態オン
	m_bBlurUp = true;

	// 時間設定
	m_nBlurTime = nBlurTime;

	// カウンターリセット
	m_nCntBulur = 0;

	// ポリゴンの拡大設定
	m_apScreen[1]->SetScaleSize(fScaleValue,nBlurTime);
	
	// ポリゴンの色を設定
	m_apScreen[1]->SetColor(bulrColor);
}

//**********************************************
// レンダラーぼかしの更新
//**********************************************
void CRenderer::UpdateBulrUp(void)
{
	// ぼかさない状態出れば処理しない
	if (m_bBlurUp == false)
	{
		return;
	}

	// ぼかしカウンタカウントアップ
	m_nCntBulur++;

	// 拡大率取得
	float fScaleValue = m_apScreen[1]->GetScaleValue();

	// ポリゴンの拡大設定
	m_apScreen[1]->SetScaleSize(fScaleValue, m_nBlurTime);

	// カウンターが最大値までカウントされたら終了
	if (Clamp(m_nCntBulur, 0, m_nBlurTime) == m_nBlurTime)
	{
		// ぼかさない状態にする
		m_bBlurUp = false;

		// 色を設定
		m_apScreen[1]->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));	
	}
}