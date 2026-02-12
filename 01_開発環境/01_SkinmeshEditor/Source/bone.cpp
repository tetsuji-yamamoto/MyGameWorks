//*********************************************
// 
// ボーン
// Author Tetsuji Yamamoto
// 
//*********************************************
#include "bone.h"

// 静的メンバー変数宣言
int CBone::m_nNumOll = 0;	// 全体数
bool CBone::m_bDisp = true;	// 映すか否か

//**************************************************
// ボーンコンストラク
//**************************************************
CBone::CBone()
{
	m_pVtxBuff = NULL;								// 頂点バッファ
	m_pIdxBuff = NULL;								// インデックスバッファ
	ZeroMemory(&m_mtxWorld,sizeof(D3DXMATRIX));		// ワールドマトリックス
	m_pos = InitValu::VEC3_NULL;						// 位置
	m_col = InitValu::COLOR_NULL;							// 色

	// ID設定
	m_nID = m_nNumOll;

	// 全体数カウントアップ
	m_nNumOll++;
}

//**************************************************
// ボーンデストラクタ
//**************************************************
CBone::~CBone()
{
	// 全体数カウントダウン
	m_nNumOll--;
}

//**************************************************
// ボーン初期化
//**************************************************
HRESULT CBone::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	m_col = InitValu::COLOR_BONE;	// 色

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 20,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pIdxBuff,
		NULL);

	// 各種変数の初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&m_mtxRotate);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fRadius = BONE_MODEL_RADIUS;
	float fHeight = BONE_MODEL_HEIGHT;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, fRadius);
	pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, -fRadius);
	pVtx[3].pos = D3DXVECTOR3(-fRadius, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pVtx[5].pos = D3DXVECTOR3(0.0f, fRadius,0.0f );
	pVtx[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, -fRadius, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
	{
		pVtx[nCntVtx].tex = { 0.0f,0.0f };
		pVtx[nCntVtx].col = m_col;
		pVtx[nCntVtx].nor = { 0.0f,1.0f,0.0f };
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// インデック情報へのポインタ
	WORD* pIdx;

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 1;
	pIdx[3] = 2;
	pIdx[4] = 2;
	pIdx[5] = 3;
	pIdx[6] = 3;
	pIdx[7] = 0;

	pIdx[8] = 0;
	pIdx[9] = 4;
	pIdx[10] = 1;
	pIdx[11] = 4;
	pIdx[12] = 2;
	pIdx[13] = 4;
	pIdx[14] = 3;
	pIdx[15] = 4;

	pIdx[16] = 5;
	pIdx[17] = 6;

	pIdx[18] = 6;
	pIdx[19] = 7;

	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	// 結果を返す
	return S_OK;
}

//**************************************************
// ボーン終了
//**************************************************
void CBone::Uninit(void)
{
	// 頂点バッファの解放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 自身の破棄
	Release();
}

//**************************************************
// ボーン更新
//**************************************************
void CBone::Update(void)
{
}

//**************************************************
// ボーン描画
//**************************************************
void CBone::Draw(void)
{
	// 映すか否か
	if (m_bDisp == false)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転行列を掛ける
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRotate);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST,
		0,
		0,
		8,
		0,
		10);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//**************************************************
// ボーン生成
//**************************************************
CBone* CBone::Create(void)
{
	CBone* pBone = new CBone;

	if (pBone != NULL)
	{
		// 初期化処理
		pBone->Init();
	}
	else
	{
		MessageBox(NULL, " CBone::Create() メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pBone;
}

//**************************************************
// ボーン高さ設定
//**************************************************
void CBone::SetHeight(float fHeight)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[4].pos.y = fHeight;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************************
// ボーン高さ設定
//**************************************************
void CBone::SetBack(D3DXVECTOR3 pos)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定

	pVtx[7].pos = pos;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************************
// ボーン幅設定
//**************************************************
void CBone::SetRadius(float fRadius)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.z = fRadius;
	pVtx[1].pos.x = fRadius;
	pVtx[2].pos.z = -fRadius;
	pVtx[3].pos.x = -fRadius;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************************
// ボーンの形をリセット
//**************************************************
void CBone::ResetVertex(void)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, BONE_MODEL_RADIUS);
	pVtx[1].pos = D3DXVECTOR3(BONE_MODEL_RADIUS, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, -BONE_MODEL_RADIUS);
	pVtx[3].pos = D3DXVECTOR3(-BONE_MODEL_RADIUS, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, BONE_MODEL_HEIGHT, 0.0f);

	pVtx[5].pos = D3DXVECTOR3(0.0f, BONE_MODEL_RADIUS, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************************
// ボーンの色設定
//**************************************************
void CBone::SetColor(D3DXCOLOR col)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 色を設定
	m_col = col;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色を設定
	for (int nCntVtx = 0; nCntVtx < 7; nCntVtx++)
	{
		pVtx[nCntVtx].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}