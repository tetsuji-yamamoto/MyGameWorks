//******************************************
// 
// テクスチャ
// Author Tetsuji Yamamoto
// 
//******************************************
#include "texture.h"
#include "renderer.h"
#include "debugproc.h"

using namespace std;

// 静的メンバー変数宣言
int CTexture::m_nNumAll = 0;

//******************************************
// テクスチャ
//******************************************
CTexture::CTexture()
{

}

//******************************************
// テクスチャ
//******************************************
CTexture::~CTexture()
{

}

//******************************************
// テクスチャ
//******************************************
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//******************************************
// テクスチャ破棄
//******************************************
void CTexture::Uninit(void)
{
	// すべてのテクスチャの破棄
	for (auto& iterTexInfo : m_mTextureInfo)
	{
		// テクスチャがあれば破棄
		if (iterTexInfo.second != nullptr)
		{
			iterTexInfo.second->Release();
			iterTexInfo.second = nullptr;
		}
	}
	m_mTextureInfo.clear();

	// 自身の破棄
	delete this;
}

//******************************************
// テクスチャ全破棄
//******************************************
void CTexture::UnloadOll(void)
{
	// すべてのテクスチャの破棄
	for (auto& iterTexInfo : m_mTextureInfo)
	{
		// テクスチャがあれば破棄
		if (iterTexInfo.second != nullptr)
		{
			iterTexInfo.second->Release();
			iterTexInfo.second = nullptr;
		}
	}
	m_mTextureInfo.clear();
}

//******************************************
// テクスチャ破棄
//******************************************
void CTexture::Unload(const char* pTexturePass)
{
	// 同じファイル名があればそれを削除
	auto iterTexInfo = m_mTextureInfo.find(pTexturePass);
	if (iterTexInfo != m_mTextureInfo.end())
	{
		// テクスチャがあれば破棄
		if ((*iterTexInfo).second != nullptr)
		{
			(*iterTexInfo).second->Release();
			(*iterTexInfo).second = nullptr;
		}

		// 要素削除
		m_mTextureInfo.erase(iterTexInfo);
	}
}

//******************************************
// テクスチャ
//******************************************
string CTexture::Register(const char* pFileName)
{
	// ファイル名が無ければ処理しない
	if (pFileName == NULL)
	{
		return "";
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 同じファイル名があればそれを返す
	if (m_mTextureInfo.find(pFileName) != m_mTextureInfo.end())
	{
		return pFileName;
	}

	// テクスチャを読み込む
	LPDIRECT3DTEXTURE9 pTexture;
	HRESULT hr = D3DXCreateTextureFromFile(pDevice,
		pFileName,
		&pTexture);

	// 読込失敗
	if (FAILED(hr))
	{
		MessageBox(NULL, "CTexture::Register() > テクスチャを読み込めませんでした", "Error", MB_OK);
		return "";
	}

	// 要素追加
	m_mTextureInfo.insert(std::make_pair(pFileName, pTexture));

	// テクスチャの総数カウントアップ
	m_nNumAll++;

	return pFileName;
}

//******************************************
// テクスチャ
//******************************************
LPDIRECT3DTEXTURE9 CTexture::GetAddress(const char* pTexturePass)
{
	// 指定したインデックスと同じテクスチャを返す
	for (auto& iterTexInfo : m_mTextureInfo)
	{
		// 同じインデックスであれば
		if (iterTexInfo.first == pTexturePass)
		{
			return iterTexInfo.second;
		}
	}

	return nullptr;
}

//******************************************
// テクスチャ
//******************************************
CTexture* CTexture::Create()
{
	CTexture* pTexture = nullptr;

	// オブジェクト2Dの生成
	if (pTexture == nullptr)
	{
		pTexture = new CTexture;
	}

	// 結果を返す
	return  pTexture;
}

//******************************************
// テクスチャ
//******************************************
void CTexture::DebugTexture()
{
	CDebugprocc::Print("[ CTexture ]\n");
	CDebugprocc::Print("m_nNumAll : %d\n", m_nNumAll);
	CDebugprocc::Print("\n");
}