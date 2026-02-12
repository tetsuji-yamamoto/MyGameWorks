//****************************************************
// 
// モデルコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "compModel.h"
#include "compInfo.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "shader.h"
#include "ShaderManager.h"

using namespace std;

//********************************************************
// コンストラクタ
//********************************************************
CModel::CModel()
{
	m_pMesh			= nullptr;	// メッシュ
	m_pBuffMat		= nullptr;	// マテリアル
	m_dwNumMat		= 0;		// マテリアルの数
	m_pMtxParent	= nullptr;	// 親マトリックス
	m_bDisp			= true;		// 映すか否か
}

//********************************************************
// コンストラクタ
//********************************************************
CModel::CModel(const std::string& XFilePath, const bool bDisp):
	m_XFilePath(XFilePath),
	m_bDisp(bDisp)
{
	m_pMesh			= nullptr;	// メッシュ
	m_pBuffMat		= nullptr;	// マテリアル
	m_dwNumMat		= 0;		// マテリアルの数
	m_pMtxParent	= nullptr;	// 親マトリックス
}

//********************************************************
// デストラクタ
//********************************************************
CModel::~CModel()
{

}

//********************************************************
// 初期化
//********************************************************
void CModel::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// Xファイル読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		m_XFilePath.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh
	);

	if (FAILED(hr))
	{// 失敗したら
		MessageBox(NULL, "CMotionSkin::Init() > モデルを読み込めませんでした", "Error", MB_OK);

		// 結果を返す
		return;
	}

	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		string texturePath = pTexture->Register((const char*)pMat[nCntMat].pTextureFilename);
		m_vsTexturePath.push_back(texturePath);
	}

	// 情報取得
	CInfo* pInfo = GetOwner()->GetComponent<CInfo>();

	// 親マトリックス取得
	if (pInfo != nullptr)
	{
		m_pMtxParent = &pInfo->mtxWorld;
	}
}

//********************************************************
// 終了
//********************************************************
void CModel::Uninit(void)
{
	// マテリアルバッファの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// 自身の破棄
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CModel::Update(void)
{

}

//********************************************************
// 描画
//********************************************************
void CModel::Draw(void)
{
	// メッシュが無ければ処理しない
	if(m_pMesh == nullptr)return;

	// 映さなければ処理しない
	if(m_bDisp == false)return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// マネージャーの取得
	CManager* pMana = CManager::GetInstance();

	// シェーダーの取得
	int nIDShader = CManager::GetInstance()->GetIDShader();
	CShader* pShaderClass = CShaderManager::GetInstance()->GetShader(nIDShader);
	LPD3DXEFFECT pShader = pShaderClass->GetShader();

	// パラメータ(グローバル変数の設定)
	int nIDMtxWorld = pMana->GetIDPramHandlMtxWorld();

	if(m_pMtxParent != nullptr)pShader->SetMatrix(pShaderClass->GetParamaterHandleHandle(nIDMtxWorld), m_pMtxParent);
	else pShader->SetMatrix(pShaderClass->GetParamaterHandleHandle(nIDMtxWorld), &InitValu::MTX_NULL);

	// シェーダー起動
	pShader->Begin(0, 0);

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの設定
	if(m_pMtxParent != nullptr)pDevice->SetTransform(D3DTS_WORLD, m_pMtxParent);
	else pDevice->SetTransform(D3DTS_WORLD, &InitValu::MTX_NULL);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// 表示用マテリアル作成
		D3DXMATERIAL mat = pMat[nCntMat];

		// 色取得
		D3DXVECTOR4 defuuse = {
		mat.MatD3D.Diffuse.r,
		mat.MatD3D.Diffuse.g,
		mat.MatD3D.Diffuse.b,
		mat.MatD3D.Diffuse.a
		};

		// 透明であれば
		if (defuuse.w < 1.0f)
		{
			// カリングオフ
			//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// Zテストオフ
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			////αブレンディングを加算合成に設定
			//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// パラメータ(グローバル変数の設定)
		int nIDDeffuse = pMana->GetIDPramHandlDeffuse();
		pShader->SetVector(pShaderClass->GetParamaterHandleHandle(nIDDeffuse), &defuuse);

		// マテリアルの設定
		pDevice->SetMaterial(&mat.MatD3D);

		// テクスチャの取得
		LPDIRECT3DTEXTURE9 pTex = pTexture->GetAddress(m_vsTexturePath[nCntMat].c_str());

		// テクスチャの設定
		pDevice->SetTexture(0, pTex);

		// パス起動,テクスチャの有無によりパスを変更
		if (pTex != nullptr)pShader->BeginPass(3);
		else pShader->BeginPass(2);

		// 変更をGPUに反映
		pShader->CommitChanges();

		// モデル(パーツの描画)
		m_pMesh->DrawSubset(nCntMat);

		// パス終了
		pShader->EndPass();

		// カリングの設定
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// Zテストオン
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// シェーダー終了
	pShader->End();
}