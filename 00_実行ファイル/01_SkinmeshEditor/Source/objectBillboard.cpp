//*************************************
//
// オブジェクトビルボードビルボード
// Author Tetsuji Yamamoto
//
//*************************************
#include "objectBillboard.h"
#include "manager.h"
#include "camera.h"

// 静的メンバー変数宣言
bool CObjectBillboard::m_bDisp = true;	// 映すか否か

//*************************************
// オブジェクトビルボードコンストラクタ
//*************************************
CObjectBillboard::CObjectBillboard() : CObject(PRIORITY_6)
{
	m_pVtxBuff = NULL;							// 頂点バッファの生成
	ZeroMemory(m_mtxWorld, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pos = InitValu::VEC3_NULL;					// 位置
	m_rot = InitValu::VEC3_NULL;					// 向き
	m_col = InitValu::COLOR_NULL;						// 色
	m_fWidth = 0.0f;							// 幅
	m_fHeight = 0.0f;							// 高さ
}

//*************************************
// オブジェクトビルボードデストラクタ
//*************************************
CObjectBillboard::~CObjectBillboard()
{

}

//*************************************
// オブジェクトビルボード初期化処理
//*************************************
HRESULT CObjectBillboard::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CObjectBillboard::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}


	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -50.0f;
	pVtx[0].pos.y = 50.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 50.0f;
	pVtx[1].pos.y = 50.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -50.0f;
	pVtx[2].pos.y = -50.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 50.0f;
	pVtx[3].pos.y = -50.0f;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*************************************
// オブジェクトビルボード初期化処理
//*************************************
HRESULT CObjectBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 各種変数の初期化
	m_pos = pos;
	m_rot = rot;
	m_col = col;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// 頂点バッファの生成
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CObjectBillboard::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -fWidth;
	pVtx[0].pos.y = fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = fWidth;
	pVtx[1].pos.y = fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -fWidth;
	pVtx[2].pos.y = -fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = fWidth;
	pVtx[3].pos.y = -fHeight;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col =  col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*************************************
// オブジェクトビルボード終了処理
//*************************************
void CObjectBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pVtxBuff != NULL)
	{
		MessageBox(NULL, " CObject2D::Uninit() > if (m_pVtxBuff != NULL) メモリがNULLではありません", "Error", MB_OK);
	}

	Release();
}

//*************************************
// オブジェクトビルボード更新処理
//*************************************
void CObjectBillboard::Update(void)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -m_fWidth;
	pVtx[0].pos.y = m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_fWidth;
	pVtx[1].pos.y = m_fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -m_fWidth;
	pVtx[2].pos.y = -m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_fWidth;
	pVtx[3].pos.y = -m_fHeight;
	pVtx[3].pos.z = 0.0f;

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//*************************************
// オブジェクトビルボード描画処理
//*************************************
void CObjectBillboard::Draw(void)
{
	// 映すか否か
	if (m_bDisp == false)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetTexture();

	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックス取得
	D3DXMATRIX mtxview;
	pDevice->GetTransform(D3DTS_VIEW, &mtxview);

	// カメラの逆行列を設定
	m_mtxWorld._11 = mtxview._11;
	m_mtxWorld._12 = mtxview._21;
	m_mtxWorld._13 = mtxview._31;
	m_mtxWorld._21 = mtxview._12;
	m_mtxWorld._22 = mtxview._22;
	m_mtxWorld._23 = mtxview._32;
	m_mtxWorld._31 = mtxview._13;
	m_mtxWorld._32 = mtxview._23;
	m_mtxWorld._33 = mtxview._33;

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetTexture(0, pTexture->GetAddress(m_mTexturePass.c_str()));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//*************************************
// オブジェクトビルボード生成処理
//*************************************
CObjectBillboard* CObjectBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	CObjectBillboard* pObjectBill = new CObjectBillboard;
	if (pObjectBill != NULL)
	{
		// 初期化処理
		pObjectBill->Init(pos, rot, col, fWidth, fHeight, pTexturePass);
	}
	else
	{
		MessageBox(NULL, " CObjectBillboard::Create() > if ( pObjectBill != NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pObjectBill;
}

//*************************************
// オブジェクトビルボード幅設定
//*************************************
void CObjectBillboard::SetWidth(float fWidth)
{
	// 幅設定
	m_fWidth = fWidth;
	m_fHeight = fWidth;
}