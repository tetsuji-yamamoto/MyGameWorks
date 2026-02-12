//****************************************************
// 
// ステンシルシャドウ
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "shadowS.h"
#include "object.h"
#include "manager.h"
#include "texture.h"
#include "light.h"

//****************************************************
// ステンシルシャドウコンストラクタ
//****************************************************
CShadowS::CShadowS(int nPriority) : CObjectX(nPriority)
{
	ZeroMemory(&m_mtxScale, sizeof(D3DXMATRIX));	// スケーリングマトリックス
	m_pos = InitValu::VEC3_NULL;						// 位置
	m_fRadius = 0.0f;								// 半径
}

//****************************************************
// ステンシルシャドウデストラクタ
//****************************************************
CShadowS::~CShadowS()
{
}

//****************************************************
// ステンシルシャドウ初期化
//****************************************************
HRESULT CShadowS::Init(float fRadius)
{
	// オブジェクトXの初期化
	CObjectX::Init("data\\model\\StencilShadowModel.x");

	// 半径設定
	m_fRadius = fRadius;

	// マトリックスの初期化
	D3DXMatrixIsIdentity(&m_mtxScale);

	// スケールマトリックスを求める
	D3DXMatrixScaling(&m_mtxScale, m_fRadius * 2.0f, 50.0f, m_fRadius * 2.0f);

	return S_OK;
}

//****************************************************
// ステンシルシャドウ終了
//****************************************************
void CShadowS::Uninit(void)
{
	// オブジェクトXの終了
	CObjectX::Uninit();
}

//****************************************************
// ステンシルシャドウ更新
//****************************************************
void CShadowS::Update(void)
{

}

//****************************************************
// ステンシルシャドウ描画
//****************************************************
void CShadowS::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// ステンシルシャドウ用のポリゴン取得
	CScreen* pScreenShadowS = CManager::GetInstance()->GetScreenShadwS();

	// 計算用マトリックス
	D3DXMATRIX mtxWorld, mtxTrans;

	// 画面クリア(バッファバッファ&のクリア）
	pDevice->Clear(0, NULL,
		(D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	
	// スケーリングマトリックスを掛け合わせる
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld ,& m_mtxScale);

	// 位置を決める
	mtxWorld._41 = m_pos.x;
	mtxWorld._42 = m_pos.y;
	mtxWorld._43 = m_pos.z;

	// ステンシルを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	
	// Zバッファへの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	// カラーバッファへの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// ステンシルバッファの比較パラメータを設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);		// ステ : 合　Z : 合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);	// ステ : 合　Z : 不
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);		// ステ : 不

	// 裏面カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// オブジェクトXの描画
	CObjectX::Draw(&mtxWorld);

	// ステンシルバッファの参照値を設定
	pDevice->SetRenderState(D3DRS_STENCILREF,0x01);

	// ステンシルバッファの比較パラメータを設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);		// ステ : 合　Z : 合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ステ : 合　Z : 不
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ステ : 不

	// 裏面カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// オブジェクトXの描画
	CObjectX::Draw(&mtxWorld);

	// カラーバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// ステンシルバッファの参照値を設定(2にする)
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの比較パラメータを設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);		// ステ : 合　Z : 合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ステ : 合　Z : 不
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ステ : 不

	// 画面全体のポリゴン描画
	pScreenShadowS->Draw(NULL);

	// すべて元に戻す
	// 裏面カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// カラーバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// ステンシルを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// Zバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//****************************************************
// ステンシルシャドウモデルの描画
//****************************************************
void CShadowS::Draw(int* m_apIdxTexture, LPD3DXMESH m_pMesh, LPD3DXBUFFER m_pBuffMat, DWORD m_dwNumMat, D3DXMATRIX m_mtxWorld)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	D3DXVECTOR3 vecScale;
	vecScale.x = 1.0f;
	vecScale.y = 1.0f;
	vecScale.z = 1.0f;

	D3DXMATRIX mtxScale;
	D3DXMATRIX mtxResult;
	D3DXMatrixIdentity(&mtxScale);
	D3DXMatrixScaling(&mtxScale, vecScale.x, vecScale.y, vecScale.z);
	D3DXMatrixMultiply(&mtxResult, &m_mtxWorld, &mtxScale);
	mtxResult._41 = m_mtxWorld._41;
	mtxResult._42 = m_mtxWorld._42;
	mtxResult._43 = m_mtxWorld._43;

	// ステンシルを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// Zバッファへの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// カラーバッファへの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// ステンシルバッファの比較パラメータを設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);		// ステ : 合　Z : 合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);	// ステ : 合　Z : 不
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);		// ステ : 不

	// 裏面カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// モデルの描画
	CShadowS::DrawModel(m_apIdxTexture, m_pMesh, m_pBuffMat, m_dwNumMat, mtxResult);

	// ステンシルバッファの参照値を設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの比較パラメータを設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);		// ステ : 合　Z : 合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ステ : 合　Z : 不
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ステ : 不

	// 裏面カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// モデルの描画
	CShadowS::DrawModel(m_apIdxTexture, m_pMesh, m_pBuffMat, m_dwNumMat, mtxResult);

	// カラーバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// ステンシルバッファの参照値を設定(2にする)
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの比較パラメータを設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);		// ステ : 合　Z : 合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ステ : 合　Z : 不
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ステ : 不

	// すべて元に戻す
	// 裏面カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// カラーバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// ステンシルを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// Zバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//****************************************************
// ステンシルシャドウモデルの描画
//****************************************************
void CShadowS::DrawModel(int* m_apIdxTexture, LPD3DXMESH m_pMesh, LPD3DXBUFFER m_pBuffMat, DWORD m_dwNumMat, D3DXMATRIX m_mtxWorld)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//****************************************************
// ステンシルシャドウの位置更新
//****************************************************
void CShadowS::UpdatePosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_pos.y += 0.1f;
}

//****************************************************
// ステンシルシャドウの生成
//****************************************************
CShadowS* CShadowS::Create(float fRadius)
{
	// 影インスタンス生成
	CShadowS* pShadowS = new CShadowS(CObject::PRIORITY_4);
	
	// 確保できていれば
	if (pShadowS != nullptr)
	{
		// 初期化処理
		pShadowS->Init(fRadius);
	}
	// いなければ
	else
	{
		MessageBox(NULL, " CShadowS::Create() > メモリを確保できませんでした", "Error", MB_OK);
	}

	// 結果を返す
	return  pShadowS;
}