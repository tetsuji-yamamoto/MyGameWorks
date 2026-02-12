//*********************************************
// 
// シリンダー
// Author Tetsuji Yamamoto
// 
//*********************************************
#ifndef _SYLINDER_H_
#define _SYLINDER_H_

#include "main.h"
#include "object.h"

// シリンダークラス
class CCylinder
{
public:
	CCylinder();
	~CCylinder();

	HRESULT Init(D3DXVECTOR3 startPos, int nDivisionX, int nDivisionY, float fRadius, float fHeight, D3DXCOLOR col,const char* pTexturePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCylinder* Create(D3DXVECTOR3 startPos, int nDivisionX, int nDivisionY, float fRadius, float fHeight, D3DXCOLOR col,const char* pTexturePass);

	// セッター
	void SetMatrix(D3DXMATRIX mtx) {m_mtxWorld = mtx;}

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	std::string m_mTexturePass;			// テクスチャへパス
	int m_nNumPolygon;					// ポリゴン数
	int m_nNumVtx;						// 頂点数
	int m_nNumIdx;						// インデックス数
};

#endif // !_SYLINDER_H_
