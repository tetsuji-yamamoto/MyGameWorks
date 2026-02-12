//********************************************************
// 
// テクスチャコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compTexture.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//********************************************************
// コンストラクタ
//********************************************************
CTextures::CTextures(TexturesArgument argment)
{
	m_argument = argment;
}

//********************************************************
// デストラクタ
//********************************************************
CTextures::~CTextures()
{

}

//********************************************************
// 初期化
//********************************************************
void CTextures::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャのインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// テクスチャのレジスト
	m_argument.texturePath = pTexture->Register(m_argument.texturePath.c_str());

}

//********************************************************
// 終了
//********************************************************
void CTextures::Uninit(void)
{
	// 自身の破棄
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CTextures::Update(void)
{

}

//********************************************************
// 描画
//********************************************************
void CTextures::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャインスタンス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_argument.texturePath.c_str()));
}
