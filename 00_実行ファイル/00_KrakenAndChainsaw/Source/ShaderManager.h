//****************************************************
// 
// シェーダーマネージャー
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "shader.h"

// 当たり判定形状クラス
class CShaderManager
{
public:
static const int ID_INIT = -1;	// IDの初期値

	// シェーダー情報構造体
	typedef struct ShaderInfo
	{
		// コンストラクタ
		ShaderInfo() 
		{
			pShader = new CShader;	// シェーダーインスタンス生成
			nID = ID_INIT;			// ID初期値設定
		}

		CShader*	pShader;	// シェーダー
		int			nID;		// ID
	}ShaderInfo;

	// 生成処理
	static void Create(void)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CShaderManager;
			m_pInstance->Init();
		}
	}

	// 破棄処理
	static void Delete()
	{
		if (m_pInstance != nullptr)
		{
			m_pInstance->Uninit();
			m_pInstance = nullptr;
		}
	}

	// 取得処理
	static CShaderManager* GetInstance(void)
	{
		return m_pInstance;
	}

	void Init(void);
	void Uninit(void);

	int AddShader(std::string shaderFileName, std::string TechniqueName);
	CShader* GetShader(int nID);

private:
	CShaderManager() {}
	~CShaderManager() {}

	static CShaderManager*	m_pInstance;	// シェーダーマネージャーインスタンス
	static int				m_nCounterID;	// IDカウンター
	std::vector<ShaderInfo>	m_ShaderInfo;	// シェーダー情報
};

#endif