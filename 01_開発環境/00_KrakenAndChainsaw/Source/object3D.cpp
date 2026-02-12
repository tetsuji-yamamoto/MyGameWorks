//*************************************
//
// オブジェクト3D
// Author Tetsuji Yamamoto
//
//*************************************
#include "object3D.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"

//*************************************
// オブジェクトコンストラクタ
//*************************************
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = NULL;
	ZeroMemory(m_mtxWorld,sizeof(m_mtxWorld));
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//*************************************
// オブジェクトデストラクタ
//*************************************
CObject3D::~CObject3D()
{

}

//*************************************
// オブジェクト初期化処理
//*************************************
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,float fWidth,float fHeight,float fDepth,const char *pTexturePass)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 各種変数の設定
	m_pos = pos;
	m_rot = rot;
	m_col = col;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;

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
		MessageBox(NULL, "CObject3D::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// テクスチャの読込
	m_mTexturePass = pTexture->Register(pTexturePass);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -fWidth;
	pVtx[0].pos.y = fHeight;
	pVtx[0].pos.z = fDepth;

	pVtx[1].pos.x = fWidth;
	pVtx[1].pos.y = fHeight;
	pVtx[1].pos.z = fDepth;

	pVtx[2].pos.x = -fWidth;
	pVtx[2].pos.y = -fHeight;
	pVtx[2].pos.z = -fDepth;

	pVtx[3].pos.x = fWidth;
	pVtx[3].pos.y = -fHeight;
	pVtx[3].pos.z = -fDepth;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ポリゴンの縁を求めて
	D3DXVECTOR3 nor,edge0, edge1;
	edge0 = pVtx[1].pos - pVtx[0].pos;
	edge1 = pVtx[2].pos - pVtx[0].pos;

	// 外積で法線を求める
	D3DXVec3Cross(&nor, &edge0, &edge1);

	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
	
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*************************************
// オブジェクト終了処理
//*************************************
void CObject3D::Uninit(void)
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
// オブジェクト更新処理
//*************************************
void CObject3D::Update(void)
{

}

//*************************************
// オブジェクト描画処理
//*************************************
void CObject3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャを設定
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
// オブジェクト描画処理
//*************************************
void CObject3D::DrawNoTexture(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
// オブジェクト生成処理
//*************************************
void CObject3D::SetColor(D3DXCOLOR col)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//*************************************
// オブジェクト頂点取得処理
//*************************************
D3DXVECTOR3 CObject3D::GetVtxPos(int nNumVtx)
{
	// 戻り値用変数
	D3DXVECTOR3 posReturn = InitValu::VEC3_NULL;

	// 頂点番号が範囲外であれば処理しない
	if (nNumVtx < 0 || nNumVtx >= 4)
	{
		return posReturn;
	}

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posVtx = pVtx[nNumVtx].pos;

	// 位置設定
	posReturn.x = (posVtx.x * m_mtxWorld._11) + (posVtx.y * m_mtxWorld._21) + (posVtx.z * m_mtxWorld._31) + (1.0f * m_mtxWorld._41);
	posReturn.y = (posVtx.x * m_mtxWorld._12) + (posVtx.y * m_mtxWorld._22) + (posVtx.z * m_mtxWorld._32) + (1.0f * m_mtxWorld._42);
	posReturn.z = (posVtx.x * m_mtxWorld._13) + (posVtx.y * m_mtxWorld._23) + (posVtx.z * m_mtxWorld._33) + (1.0f * m_mtxWorld._43);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return posReturn;
}

//*************************************
// オブジェクト生成処理
//*************************************
CObject3D* CObject3D::Create(void)
{
	// オブジェクト3Dインスタンス生成
	CObject3D* pObject3D = new CObject3D;
	
	if (pObject3D != NULL)
	{
		// 初期化処理
		pObject3D->Init();
	}
	else
	{
		MessageBox(NULL, " CObject3D::Create() > if ( pObject3D != NULL) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pObject3D;
}