//**************************************
// 
// シリンダー
// Author Tetsuji Yamamoto
// 
//**************************************
#include "shapeSylinder.h"
#include "texture.h"
#include "manager.h"
#include "myMath.h"

//**************************************
// シリンダーコンストラクタ
//**************************************
CShapeSylinder::CShapeSylinder() 
{
	m_pVtxBuff = nullptr;							// 頂点バッファへのポインタ
	m_pIdxBuff = nullptr;							// インデックスバッファへのポインタ
	ZeroMemory(m_mtxWorld, sizeof(m_mtxWorld));		// ワールドマトリックス
	m_nNumPolygon = 0;						// メッシュのポリゴンの数
	m_nNumVtx = 0;									// 頂点数
	m_nNumIdx = 0;									// インデックス数
	m_nDivisionX = 0;								// 分割数X
	m_nDivisionY = 0;								// 分割数Y
	m_mtxWorld = InitValu::MTX_NULL;
}

//**************************************
// シリンダーデストラクタ
//**************************************
CShapeSylinder::~CShapeSylinder()
{
}

//**************************************
// シリンダー初期化
//**************************************
HRESULT CShapeSylinder::Init(const D3DXCOLOR col,const int nDivisionX, const  int nDivisionY, const  float fRadius, const float fHeight)
{
	// 分割数設定
	m_nDivisionX = nDivisionX;
	m_nDivisionY = nDivisionY;

	// 分割数がゼロ以下にならないようにする
	if (nDivisionX <= 0)
	{
		m_nDivisionX = 1;
	}

	if (nDivisionY <= 0)
	{
		m_nDivisionY = 1;
	}

	// フィールド部分のポリゴン数
	m_nNumPolygon = CMyMath::SeekFieldNumPolygonAll(m_nDivisionX, m_nDivisionY);

	// フィールドの頂点数
	m_nNumVtx = CMyMath::SeekFieldNumVertexAll(m_nDivisionX, m_nDivisionY);

	// フィールドのインデックス数
	m_nNumIdx = CMyMath::SeekFieldNumIndexAll(m_nNumPolygon);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

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
		MessageBox(NULL, "CCylinderBG::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
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
		MessageBox(NULL, "CCylinderBG::Init > インデックスバッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngleX = (D3DX_PI * 2.0f) / m_nDivisionX;
	int nCntVtx = 0;

	for (int nCntY = 0; nCntY <= m_nDivisionY; nCntY++)
	{
		float fPosY = -fHeight * 0.5f;
		for (int nCntX = 0; nCntX <= m_nDivisionX; nCntX++, nCntVtx++)
		{
			float fTexX = (1.0f / (float)m_nDivisionX) * (float)nCntX;	// テクスチャ座標X
			float fTexY = 1.0f / (float)m_nDivisionY * (float)nCntY;		// テクスチャ座標Y

			// 位置設定
			pVtx[nCntVtx].pos.x = sinf(fAngleX * nCntX) * fRadius;
			pVtx[nCntVtx].pos.y = fPosY + (fHeight * (float)nCntY / (float)m_nDivisionY);
			pVtx[nCntVtx].pos.z = cosf(fAngleX * nCntX) * fRadius;

			// テクスチャ座標設定
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

	int nIdxA = m_nDivisionX + 1;	// 左側インデックス
	int nIdxB = 0;				// 右側インデックス
	int nCntIdx = 0;			// インデックスカウンター

	// Zの分割数分回す
	for (int nCntA = 0; nCntA < m_nDivisionY; nCntA++)
	{
		// Xの分割数分回す
		for (int nCntB = 0; nCntB <= m_nDivisionX; nCntB++)
		{
			pIdx[nCntIdx] = nIdxA;
			pIdx[nCntIdx + 1] = nIdxB;
			nCntIdx += 2;
			nIdxA++;
			nIdxB++;
		}

		// 安全装置
		if (nCntA < m_nDivisionY - 1)
		{
			pIdx[nCntIdx] = nIdxB - 1;
			pIdx[nCntIdx + 1] = nIdxA;
			nCntIdx += 2;
		}
	}

	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//**************************************
// シリンダー終了
//**************************************
void CShapeSylinder::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 頂点インデックスの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// 破棄に失敗している
	if (m_pVtxBuff != nullptr || m_pIdxBuff != nullptr)
	{
		MessageBox(NULL, " CObject2D::Uninit() > メモリがNULLではありません", "Error", MB_OK);
	}

	// 自分のオブジェクトを破棄
	Release();
}

//**************************************
// シリンダー更新
//**************************************
void CShapeSylinder::Update(void)
{

}

//**************************************
// シリンダー描画
//**************************************
void CShapeSylinder::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
// シリンダー描画
//**************************************
void CShapeSylinder::DrawWire(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
		D3DPT_LINESTRIP,
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
CShapeSylinder* CShapeSylinder::Create(const D3DXCOLOR col,const int nDivisionX, const  int nDivisionY, const  float fRadius, const float fHeight)
{
	// シリンダーインスタンス生成
	CShapeSylinder* pSphere = new CShapeSylinder;
	pSphere->Init(col,nDivisionX, nDivisionY, fRadius, fHeight);
	return pSphere;
}

//**************************************
// シリンダー形状設定
//**************************************
void CShapeSylinder::SetMore(const D3DXCOLOR col, const float fRadius, const float fHeight)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngleX = (D3DX_PI * 2.0f) / m_nDivisionX;
	int nCntVtx = 0;

	for (int nCntY = 0; nCntY <= m_nDivisionY; nCntY++)
	{
		float fPosY = -fHeight * 0.5f;
		for (int nCntX = 0; nCntX <= m_nDivisionX; nCntX++, nCntVtx++)
		{
			float fTexX = (1.0f / (float)m_nDivisionX) * (float)nCntX;	// テクスチャ座標X
			float fTexY = 1.0f / (float)m_nDivisionY * (float)nCntY;		// テクスチャ座標Y

			// 位置設定
			pVtx[nCntVtx].pos.x = sinf(fAngleX * nCntX) * fRadius;
			pVtx[nCntVtx].pos.y = fPosY + (fHeight * (float)nCntY / (float)m_nDivisionY);
			pVtx[nCntVtx].pos.z = cosf(fAngleX * nCntX) * fRadius;

			// テクスチャ座標設定
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
}