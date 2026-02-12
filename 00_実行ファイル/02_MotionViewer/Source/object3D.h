//*************************************
// 
// オブジェクト3D
// Author Tetsuji Yamamoto
// 
//*************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

// オブジェクト3Dクラス
class CObject3D : public CObject // CObject継承
{
public:
	CObject3D();
	~CObject3D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject3D* Create(void);
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nIdxTexture;					// テクスチャへのインデックス
};

#endif // !_OBJECT3D_H_
