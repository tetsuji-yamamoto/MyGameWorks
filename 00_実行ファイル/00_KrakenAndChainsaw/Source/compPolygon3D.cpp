//********************************************************
// 
// 3Dポリゴンコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compPolygon3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//********************************************************
// コンストラクタ
//********************************************************
CPolygon3D::CPolygon3D(Polygon3DArgument argment)
{
	m_pVtxBuff = nullptr;	// 頂点バッファへのポインタ
	m_argument = argment;	// 各種値
}

//********************************************************
// デストラクタ
//********************************************************
CPolygon3D::~CPolygon3D()
{

}

//********************************************************
// 初期化
//********************************************************
void CPolygon3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 頂点を生成
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CMeshField::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 幅と奥行き設定
	float fWidth = m_argument.fWidth * Parameters::HALF;
	float fDips = m_argument.fDips * Parameters::HALF;
	float fHeight = m_argument.fHeight * Parameters::HALF;

	// 位置設定
	pVtx[0].pos.x = -fWidth;
	pVtx[0].pos.y = fHeight;
	pVtx[0].pos.z = fDips;
	pVtx[1].pos.x = fWidth;
	pVtx[1].pos.y = fHeight;
	pVtx[1].pos.z = fDips;
	pVtx[2].pos.x = -fWidth;
	pVtx[2].pos.y = -fHeight;
	pVtx[2].pos.z = -fDips;
	pVtx[3].pos.x = fWidth;
	pVtx[3].pos.y = -fHeight;
	pVtx[3].pos.z = -fDips;

	// テクスチャ座標設定
	pVtx[0].tex = InitValu::VEC2_LEFTTOP;
	pVtx[1].tex = InitValu::VEC2_RIGHTTOP;
	pVtx[2].tex = InitValu::VEC2_LEFTBOTTOM;
	pVtx[3].tex = InitValu::VEC2_RIGHTBOTTOM;
	
	// 頂点の数分回す
	for (int nCntVtx = 0; nCntVtx < VERTEX_NUM; nCntVtx++)
	{
		// 法線設定
		pVtx[nCntVtx].nor = InitValu::VEC3_Y;

		// 色設定
		pVtx[nCntVtx].col = m_argument.col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//********************************************************
// 終了
//********************************************************
void CPolygon3D::Uninit(void)
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

//********************************************************
// 更新
//********************************************************
void CPolygon3D::Update(void)
{

}

//********************************************************
// 描画
//********************************************************
void CPolygon3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);
}

//********************************************************
// 色設定
//********************************************************
void CPolygon3D::SetColor(D3DXCOLOR col)
{
	// 色情報保存
	m_argument.col = col;
}

//********************************************************
// 色設定
//********************************************************
void CPolygon3D::SetSize(float fWidth, float fHeight, float fDips)
{
	m_argument.fWidth	= fWidth;
	m_argument.fHeight	= fHeight;
	m_argument.fDips	= fDips;
}

//********************************************************
// 反映
//********************************************************
void CPolygon3D::Reflection(void)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 幅と奥行き設定
	float fWidth = m_argument.fWidth;
	float fDips = m_argument.fDips;
	float fHeight = m_argument.fHeight;

	// 位置設定
	pVtx[0].pos.x = -fWidth;
	pVtx[0].pos.y = fHeight;
	pVtx[0].pos.z = fDips;
	pVtx[1].pos.x = fWidth;
	pVtx[1].pos.y = fHeight;
	pVtx[1].pos.z = fDips;
	pVtx[2].pos.x = -fWidth;
	pVtx[2].pos.y = -fHeight;
	pVtx[2].pos.z = -fDips;
	pVtx[3].pos.x = fWidth;
	pVtx[3].pos.y = -fHeight;
	pVtx[3].pos.z = -fDips;

	// 頂点の数分回す
	for (int nCntVtx = 0; nCntVtx < VERTEX_NUM; nCntVtx++)
	{
		// 色設定
		pVtx[nCntVtx].col = m_argument.col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}