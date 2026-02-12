//**************************************************
// 
// シェーダークラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "shader.h"
#include "renderer.h"

// 静的メンバー変数宣言
int	CShader::m_nCounterID = ID_START;	// IDカウンター

//********************************************************
// コンストラクタ
//********************************************************
CShader::CShader()
{
	m_pShader = nullptr;
	m_pVertexDecl = nullptr;
}

//********************************************************
// コンストラクタ
//********************************************************
CShader::CShader(std::string shaderFileName, std::string TechniqueName):
	m_shaderFileName(shaderFileName),
	m_TechniqueName(TechniqueName)
{
	m_pShader = nullptr;
	m_pVertexDecl = nullptr;
}

//********************************************************
// デストラクタ
//********************************************************
CShader::~CShader()
{

}

//********************************************************
// 初期化
//********************************************************
void CShader::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ファイルネーム移し替え
	const char* pFIleName = m_shaderFileName.c_str();

	// シェーダの読み込み
	LPD3DXBUFFER error = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(
		pDevice,
		m_shaderFileName.c_str(),
		0,
		0,
		D3DXSHADER_DEBUG,
		0,
		&m_pShader,
		&error);

	// 失敗した
	if (FAILED(hr))
	{
		if (error) {
			// エラーメッセージを表示
			MessageBoxA(NULL, (char*)error->GetBufferPointer(), "Shader Compile Error", MB_OK);
			error->Release();
		}
		else {
			MessageBoxA(NULL, "Unknown error. Check the file path or device initialization.", "Error", MB_OK);
		}
	}

	// テクニックのハンドル取得
	m_Tech = m_pShader->GetTechniqueByName(m_TechniqueName.c_str());
}

//********************************************************
// 終了
//********************************************************
void CShader::Uninit(void)
{
	// シェーダーの破棄
	if (m_pShader != NULL)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	// IDカウンタースタート値にセット
	m_nCounterID = ID_START;

	// 自身の削除
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CShader::Update(void)
{

}

//********************************************************
// 描画
//********************************************************
void CShader::Draw(void)
{
	
}

//********************************************************
// パラメータハンドル追加
//********************************************************
int CShader::AddParameterHandle(std::string ParameterName)
{
	// return用ID
	int nIDReturn = m_nCounterID;

	// グローバル変数ハンドル取得
	ParamaterHandleInfo ParamHandleInfo;
	ParamHandleInfo.ParamaterHandle = m_pShader->GetParameterByName(NULL, ParameterName.c_str());

	// ID設定
	ParamHandleInfo.nID = m_nCounterID;

	// 情報追加
	m_ParamaterHandleInfo.push_back(ParamHandleInfo);

	// IDカウンターインクリメント
	m_nCounterID++;

	// IDを返す
	return nIDReturn;
}

//********************************************************
// パラメータハンドル取得
//********************************************************
D3DXHANDLE CShader::GetParamaterHandleHandle(int nID)
{
	// 探す
	for (auto iter : m_ParamaterHandleInfo)
	{
		// 同じIDであれば
		if (iter.nID == nID)
		{
			// このハンドルを返す
			return iter.ParamaterHandle;
		}
	}

	// 無かった
	return NULL;
}

//********************************************************
// シェーダーファイル名設定
//********************************************************
void CShader::SetShaderFileName(std::string ShaderFileName)
{
	m_shaderFileName = ShaderFileName;
}

//********************************************************
// テクニック名設定
//********************************************************
void CShader::SetTechniqueName(std::string TechniqueName)
{
	m_TechniqueName = TechniqueName;
}