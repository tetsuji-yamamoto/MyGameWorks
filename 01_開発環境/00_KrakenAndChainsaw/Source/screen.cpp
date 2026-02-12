//**********************************************
// 
// スクリーン処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "screen.h"
#include "renderer.h"
#include "manager.h"

//**********************************************
// スクリーンコンストラクタ
//**********************************************
CScreen::CScreen()
{
	m_pVtxBuff = nullptr;							// 頂点バッファ
	m_fWidth = 0.0f;							// 幅
	m_fHeight = 0.0f;							// 高さ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_fSize = 0.0f;								// 大きさ

	m_bScale = false;		// 拡大の有無
	m_fScaleValue = 0.0f;	// 拡大率
	m_fScaleValueNow = 0.0f;// 今の拡大率
	m_nFrameScale = 0;		// 拡大する時間
	m_nCntScale = 0;		// 拡大カウンター
}

//**********************************************
// スクリーンデストラクタ
//**********************************************
CScreen::~CScreen()
{
}

//**********************************************
// スクリーン初期化処理
//**********************************************
void CScreen::Init(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	HRESULT hr;

	// 頂点バッファの生成
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CScreen::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	float fWCenter = SCREEN_WIDTH * 0.5f;
	float fHCenter = SCREEN_HEIGHT * 0.5f;

	m_fWidth = fWCenter;
	m_fHeight = fHCenter;
	m_pos = D3DXVECTOR3(fWCenter, fHCenter, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = col;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = fWCenter - fWCenter;
	pVtx[0].pos.y = fHCenter - fHCenter;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = fWCenter + fWCenter;
	pVtx[1].pos.y = fHCenter - fHCenter;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = fWCenter - fWCenter;
	pVtx[2].pos.y = fHCenter + fHCenter;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = fWCenter + fWCenter;
	pVtx[3].pos.y = fHCenter + fHCenter;
	pVtx[3].pos.z = 0.0f;

	float fShiftX = (0.5f / SCREEN_WIDTH);
	float fShiftY = (0.5f / SCREEN_HEIGHT);

	pVtx[0].tex = D3DXVECTOR2(0.0f + fShiftX, 0.0f + fShiftY);
	pVtx[1].tex = D3DXVECTOR2(1.0f + fShiftX, 0.0f + fShiftY);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fShiftX, 1.0f + fShiftY);
	pVtx[3].tex = D3DXVECTOR2(1.0f + fShiftX, 1.0f + fShiftY);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = m_col;
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// スクリーン終了処理
//**********************************************
void CScreen::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	delete this;
}

//**********************************************
// スクリーン更新処理
//**********************************************
void CScreen::Update()
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 拡大の更新
	UpdateScale();

	// 拡大率を掛けた幅と高さを求める
	float fWidth = m_fWidth * (m_fScaleValueNow);
	float fHeight = m_fHeight * (m_fScaleValueNow);

	float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight));
	float fAngleA = atan2f(fWidth, fHeight);
	float fAngleB = atan2f(fWidth, -fHeight);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x + fLength * sinf(m_rot.z - fAngleB));
	pVtx[0].pos.y = (m_pos.y + fLength * cosf(m_rot.z - fAngleB));
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_pos.x + fLength * sinf(m_rot.z + fAngleB));
	pVtx[1].pos.y = (m_pos.y + fLength * cosf(m_rot.z + fAngleB));
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_pos.x + fLength * sinf(m_rot.z - fAngleA));
	pVtx[2].pos.y = (m_pos.y + fLength * cosf(m_rot.z - fAngleA));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_pos.x + fLength * sinf(m_rot.z + fAngleA));
	pVtx[3].pos.y = (m_pos.y + fLength * cosf(m_rot.z + fAngleA));
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// スクリーン描画処理
//**********************************************
void CScreen::Draw(LPDIRECT3DTEXTURE9 pTexture)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//**********************************************
// スクリーン描画処理
//**********************************************
void CScreen::DrawAlpherTest(LPDIRECT3DTEXTURE9 pTexture)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//**********************************************
// スクリーン生成処理
//**********************************************
CScreen* CScreen::Create(D3DXCOLOR col)
{
	CScreen* pScreen = nullptr;

	// スクリーンの生成
	if (pScreen == nullptr)
	{
		pScreen = new CScreen;
		if (pScreen != nullptr)
		{
			// 初期化処理
			pScreen->Init(col);
		}
		else
		{
			MessageBox(NULL, " CScreen* CScreen::Create() >  メモリを確保できませんでした", "Error", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " CScreen* CScreen::Create() >  メモリを確保できませんでした", "Error", MB_OK);
	}

	return pScreen;
}

//**********************************************
// スクリーン拡大設定処理
//**********************************************
void CScreen::SetScaleSize(float fScaleValue, int nTimeScale)
{
	m_bScale = true;				// 拡大の有無
	m_fScaleValue = fScaleValue;	// 拡大率
	m_nFrameScale = nTimeScale;		// 拡大する時間
	m_nCntScale = 0;				// 拡大カウンター
	m_fScaleValueNow = 1.0f;		// 今の拡大率
}

//**********************************************
// スクリーン拡大更新処理
//**********************************************
void CScreen::UpdateScale(void)
{
	// 拡大する状態で無ければ更新しない
	if (m_bScale == false)
	{
		return;
	}

	// 拡大カウンタカウントアップ
	m_nCntScale++;

	// 時間倍率を計算
	float fMugTime = (float)m_nCntScale / (float)m_nFrameScale;

	// 目的の拡大率に時間倍率を掛けて時間とともに拡大する
	float fAddScaleValue = m_fScaleValue * fMugTime;

	// 拡大率を追加
	m_fScaleValueNow += fAddScaleValue;

	// カウンターが拡大する時間以上の数値になったら拡大を止める
	if (m_nCntScale >= m_nFrameScale)
	{
		m_fScaleValueNow = 1.0f;
		m_bScale = false;
	}
}

//**********************************************
// スクリーン色の設定
//**********************************************
void CScreen::SetColor(D3DXCOLOR col)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 色を設定
	m_col = col;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーを設定
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}