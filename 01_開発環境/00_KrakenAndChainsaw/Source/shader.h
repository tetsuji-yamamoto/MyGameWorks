//**************************************************
// 
// シェーダークラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _SHADER_H_
#define _SHADER_H_

// シェーダークラス
class CShader
{
public:
	static constexpr int ID_INIT = -1;	// IDの初期値
	static constexpr int ID_START = 0;	// IDの始めの数

	typedef struct ParamaterHandleInfo
	{
		ParamaterHandleInfo()
		{
			nID = ID_INIT;	// ID初期化
		}
		D3DXHANDLE	ParamaterHandle;	// パラメーターハンドル
		int			nID;				// ID
	}ParamaterHandleInfo;

	CShader();
	CShader(std::string shaderFileName, std::string TechniqueName);
	~CShader();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int	AddParameterHandle(std::string ParameterName);

	// セッター
	void SetShaderFileName	(std::string ShaderFileName);
	void SetTechniqueName	(std::string TechniqueName);

	// ゲッター
	D3DXHANDLE		GetParamaterHandleHandle(int nID);
	D3DXHANDLE		GetTechniqueHandle(void)			{return m_Tech;}
	LPD3DXEFFECT	GetShader(void)						{return m_pShader;}
private:
	static int							m_nCounterID;			// IDカウンター
	LPD3DXEFFECT						m_pShader;				// シェーダデータ
	D3DXHANDLE							m_Tech;					// テクニックへのハンドル
	LPDIRECT3DVERTEXDECLARATION9		m_pVertexDecl;			// 頂点デクラレーションへのポインタ
	std::string							m_shaderFileName;		// シェーダーファイル名
	std::string							m_TechniqueName;		// テクニックハンドルネーム
	std::vector<ParamaterHandleInfo>	m_ParamaterHandleInfo;	// グローバル変数へのハンドル
};


#endif
