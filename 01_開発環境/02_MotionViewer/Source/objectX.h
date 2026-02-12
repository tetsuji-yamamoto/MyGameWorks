//*************************************
// 
// オブジェクトX
// Author Tetsuji Yamamoto
// 
//*************************************
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "main.h"
#include "object.h"

// オブジェクト3Dクラス
class CObjectX : public CObject // CObject継承
{
public:
	CObjectX();
	~CObjectX();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObjectX* Create(void);

private:
	int *m_apIdxTexture;		// テクスチャへのインデックスポインター
	LPD3DXMESH m_pMesh;			// メッシュ
	LPD3DXBUFFER m_pBuffMat;	// マテリアル
	DWORD m_dwNumMat;			// マテリアルの数
	D3DXMATRIX m_mtxWorld;		// マトリックス
};

#endif // !_OBJECTX_H_
