//**************************************
// 
// 球形状
// Author Tetsuji Yamamoto
// 
//**************************************
#ifndef _SHAPESPHER_H_
#define _SHAPESPHER_H_

#include "main.h"
#include "object.h"

// 球形状クラス
class CShapeSpher : public CObject
{
public:
	CShapeSpher();
	~CShapeSpher();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXCOLOR col, const int nDivisionX, const  int nDivisionY, const  float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawWire(void);

	static CShapeSpher* Create(const D3DXCOLOR col, const int nDivisionX, const  int nDivisionY, const  float fRadius);

	// セッター
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	void SetMore(const D3DXCOLOR col, const float fRadius);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	std::string m_mTexturePass;			// テクスチャへパス
	int m_nNumPolygonMeshField;			// メッシュのポリゴンの数
	int m_nNumVtx;						// 頂点数
	int m_nNumVtxFan;					// 扇状の頂点の数
	int m_nNumVtxMeshField;				//メッシュの頂点の数
	int m_nNumIdx;						// インデックス数
	int m_nNumIdxFan;					// ファンのインデック数
	int m_nNumIdxMeshField;				// メッシュフィールドのインデックス数
	int m_nDivisionX;					// 分割数X
	int m_nDivisionY;					// 分割数Y
};

#endif // !_SKY_H_
