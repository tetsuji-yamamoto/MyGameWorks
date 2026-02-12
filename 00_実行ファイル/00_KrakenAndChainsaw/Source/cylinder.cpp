//*********************************************
// 
// シリンダー
// Author Tetsuji Yamamoto
// 
//*********************************************
#include "cylinder.h"
#include "myMath.h"
#include "texture.h"
#include "manager.h"

//*********************************************
// シリンダーコンストラクタ
//*********************************************
CCylinder::CCylinder()
{
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;							// インデックスバッファへのポインタ
	ZeroMemory(m_mtxWorld,sizeof(D3DXMATRIX)) ;	// ワールドマトリックス
	m_nNumPolygon = 0;							// ポリゴン数
	m_nNumVtx = 0;								// 頂点数
	m_nNumIdx = 0;								// インデックス数
}

//*********************************************
// シリンダーデストラクタ
//*********************************************
CCylinder::~CCylinder()
{
}

//*********************************************
// シリンダー初期化
//*********************************************
HRESULT CCylinder::Init(D3DXVECTOR3 startPos, int nDivisionX, int nDivisionY, float fRadius, float fHeight,D3DXCOLOR col,const char*pTexturePass)
{
	// 分割数がゼロ以下にならないようにする
	if (nDivisionX <= 0)
	{
		nDivisionX = 1;
	}

	if (nDivisionY <= 0)
	{
		nDivisionY = 1;
	}

	// フィールド部分のポリゴン数
	m_nNumPolygon = CMyMath::SeekFieldNumPolygonAll(nDivisionX, nDivisionY);

	// フィールドの頂点数
	m_nNumVtx = CMyMath::SeekFieldNumVertexAll(nDivisionX, nDivisionY);

	// フィールドのインデックス数
	m_nNumIdx = CMyMath::SeekFieldNumIndexAll(m_nNumPolygon);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 頂点を生成
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CCylinder::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// インデックスバッファの生成
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pIdxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CCylinder::Init > インデックスバッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// テクスチャの読込
	m_mTexturePass = pTexture->Register(pTexturePass);


	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngleX = (D3DX_PI * 2.0f) / nDivisionX;
	float fAngleY = D3DX_PI / nDivisionY;
	int nCntVtx = 0;

	for (int nCntY = 0; nCntY <= nDivisionY; nCntY++)
	{
		for (int nCntX = 0; nCntX <= nDivisionX; nCntX++, nCntVtx++)
		{
			float fPosY = (fHeight / (float)nDivisionY + 1) * nCntY;	// Y座標
			float fTexX = (1.0f / (float)nDivisionX) * (float)nCntX;	// テクスチャ座標X
			float fTexY = 1.0f / (float)nDivisionY * (float)nCntY;		// テクスチャ座標Y

			pVtx[nCntVtx].pos.x = startPos.x + sinf(-fAngleX * nCntX) * fRadius;
			pVtx[nCntVtx].pos.y = fPosY;
			pVtx[nCntVtx].pos.z = startPos.z + cosf(-fAngleX * nCntX) * fRadius;
			pVtx[nCntVtx].tex.x = fTexX;
			pVtx[nCntVtx].tex.y = fTexY;

			// 各頂点の法線の設定,面に対して垂直
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[nCntVtx].col = col;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// インデック情報へのポインタ
	WORD* pIdx;

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nIdxA = nDivisionX + 1;	// 左側インデックス
	int nIdxB = 0;				// 右側インデックス
	int nCntIdx = 0;			// インデックスカウンター

	// インデックスを求める
	for (int nCntA = 0; nCntA < nDivisionY; nCntA++)
	{// Zの分割数分回す
		for (int nCntB = 0; nCntB <= nDivisionX; nCntB++)
		{// Xの分割数分回す
			pIdx[nCntIdx] = nIdxA;
			pIdx[nCntIdx + 1] = nIdxB;
			nCntIdx += 2;
			nIdxA++;
			nIdxB++;
		}

		if (nCntA < nDivisionY - 1)
		{// 安全装置
			pIdx[nCntIdx] = nIdxB - 1;
			pIdx[nCntIdx + 1] = nIdxA;
			nCntIdx += 2;
		}
	}

	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//*********************************************
// シリンダー終了
//*********************************************
void CCylinder::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 頂点インデックスの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 破棄に失敗している
	if (m_pVtxBuff != NULL || m_pIdxBuff != NULL)
	{
		MessageBox(NULL, "  CCylinder::Uninit() > メモリがNULLではありません", "Error", MB_OK);
	}

	// 自分の破棄
	delete this;
}

//*********************************************
// シリンダー更新
//*********************************************
void CCylinder::Update(void)
{

}

//*********************************************
// シリンダー描画
//*********************************************
void CCylinder::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_mTexturePass.c_str()));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumVtx,
		0,
		m_nNumPolygon
	);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//**************************************
// シリンダー生成
//**************************************
CCylinder* CCylinder::Create(D3DXVECTOR3 startPos, int nDivisionX, int nDivisionY, float fRadius, float fHeight, D3DXCOLOR col, const char* pTexturePass)
{
	// シリンダーポインター
	CCylinder* pCylinder = NULL;

	// シリンダーの生成
	pCylinder = new CCylinder;
	
	// メモリが確保できた
	if (pCylinder != NULL)
	{
		// 初期化処理
		pCylinder->Init(startPos, nDivisionX, nDivisionY, fRadius, fHeight,col,pTexturePass);
	}
	// できなかった
	else
	{
		// メッセージ
		MessageBox(NULL, " CCylinder::Create() > メモリを確保できませんでした", "Error", MB_OK);
	}

	// 結果を返す
	return pCylinder;
}