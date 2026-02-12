//**************************************
// 
// シリンダー
// Author Tetsuji Yamamoto
// 
//**************************************
#ifndef _PHAPESYLINDER_H_
#define _PHAPESYLINDER_H_

#include "main.h"
#include "object.h"

// シリンダークラス
class CShapeSylinder : public CObject // CObject3D継承
{
public:
	CShapeSylinder();
	~CShapeSylinder();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXCOLOR col, const int nDivisionX, const  int nDivisionY, const  float fRadius, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawWire(void);

	static CShapeSylinder* Create(const D3DXCOLOR col,const int nDivisionX, const  int nDivisionY, const  float fRadius, const float fHeight);

	// セッター
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	void SetMore(const D3DXCOLOR col,const float fRadius, const float fHeight);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	std::string m_mTexturePass;			// テクスチャへパス
	int m_nNumPolygon;					// メッシュのポリゴンの数
	int m_nNumVtx;						// 頂点数
	int m_nNumIdx;						// インデックス数
	int m_nDivisionX;					// 分割数X
	int m_nDivisionY;					// 分割数Y
};


#endif // !_SKY_H_
