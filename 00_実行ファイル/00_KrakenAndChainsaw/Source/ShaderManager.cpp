//****************************************************
// 
// シェーダーマネージャー
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "ShaderManager.h"

// 静的メンバー変数宣言
CShaderManager*	CShaderManager::m_pInstance		= nullptr;	// シェーダーマネージャーインスタンス
int				CShaderManager::m_nCounterID	= 0;		// IDカウンター

//****************************************************
// 初期化
//****************************************************
void CShaderManager::Init(void)
{
	// カウンターリセット
	m_nCounterID = 0;
}

//****************************************************
// 終了
//****************************************************
void CShaderManager::Uninit(void)
{
	// シェーダーの破棄
	for (auto iter : m_ShaderInfo)
	{
		iter.pShader->Uninit();
	}
	m_ShaderInfo.clear();

	// 自身の破棄
	delete this;
}

//****************************************************
// シェーダーの追加
//****************************************************
int CShaderManager::AddShader(std::string ShaderFileName, std::string TechniqueName)
{
	// リターン用ID
	int nID = m_nCounterID;

	// シェーダー情報
	ShaderInfo ShaderInfo;

	// シェーダーの読込に必要な名前設定
	ShaderInfo.pShader->SetShaderFileName(ShaderFileName);
	ShaderInfo.pShader->SetTechniqueName(TechniqueName);

	// シェーダー初期化
	ShaderInfo.pShader->Init();

	// ID設定
	ShaderInfo.nID = m_nCounterID;

	// 情報追加
	m_ShaderInfo.push_back(ShaderInfo);

	// IDカウンター追加
	m_nCounterID++;

	// IDを返す
	return nID;
}

//****************************************************
// シェーダーの取得
//****************************************************
CShader* CShaderManager::GetShader(int nID)
{
	// シェーダーのを検索
	for (auto iter : m_ShaderInfo)
	{
		// 同じIDであれば
		if (iter.nID == nID)
		{
			// このポインターを返す
			return iter.pShader;
		}
	}

	return nullptr;
}