//****************************************************
// 
// モデルコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_MODEL_H_
#define _COMPONENT_MODEL_H_

#include "component.h"

// パーツクラス
class CModel : public CComponent
{
public:
	CModel();
	CModel(const std::string& XFilePath,const bool bDisp = true);
	~CModel();

	void Init(void)override;
	void Uninit(void)override;
	void Update(void) override;
	void Draw(void)override;

	// ゲッター
	LPD3DXMESH GetMesh(void){return m_pMesh;}

	// セッター
	void SetXFilePath(std::string XFilePath){ m_XFilePath = XFilePath;}

private:
	LPD3DXMESH					m_pMesh;			// メッシュ
	LPD3DXBUFFER				m_pBuffMat;			// マテリアル
	DWORD						m_dwNumMat;			// マテリアルの数
	D3DXMATRIX*					m_pMtxParent;		// 親マトリックス
	std::vector<std::string>	m_vsTexturePath;	// テクスチャへのインデックスポインター
	std::string					m_XFilePath;		// ｘファイルパス
	bool						m_bDisp;			// 映すか否か
};

#endif