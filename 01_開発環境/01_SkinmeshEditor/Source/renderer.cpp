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

// 静的メンバー変数宣言
LPDIRECT3DDEVICE9 CRenderer::m_pD3DDevice = NULL;

//**********************************************
// レンダラーコンストラクタ
//**********************************************
CRenderer::CRenderer()
{
	m_pD3D = NULL;
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
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// ゲーム画面サイズ幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// ゲーム画面サイズ高さ
	d3dpp.BackBufferFormat = d3ddm.Format;		// バックバッファの形式
	d3dpp.BackBufferCount = 1;					//
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//
	d3dpp.EnableAutoDepthStencil = TRUE;		//
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//
	d3dpp.Windowed = bWindow;					//
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
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクト
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//**********************************************
// レンダラー更新処理
//**********************************************
void CRenderer::Update(void)
{
	CObject::UpdateAll();
}

//**********************************************
// レンダラー描画処理
//**********************************************
void CRenderer::Draw(void)
{
	CDebugprocc* pDebugproc = CManager::GetDebugprocc();
	CCamera* pCamera = CManager::GetCamera();

	// 画面クリア(バッファバッファ&のクリア）
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		pCamera->SetCamera();

		// オブジェクトの描画
		CObject::DrawAll();

		// デバッグ表示の描画処理
		pDebugproc->Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}