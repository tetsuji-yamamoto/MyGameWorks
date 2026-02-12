//*********************************************
// 
// シリンダーの背景
// Author Tetsuji Yamamoto
// 
//*********************************************
#ifndef _SYLINDERBG_H_
#define _SYLINDERBG_H_

#include "main.h"
#include "object.h"
#include "cylinder.h"

// シリンダークラス
class CCylinderBG : public CObject	// CObject継承
{
public:
	CCylinderBG();
	~CCylinderBG();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(D3DXVECTOR3 startPos, int nDivisionX, int nDivisionY, float fRadius, float fheight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CCylinderBG* Create(D3DXVECTOR3 startPos, int nDivisionX, int nDivisionY, float fRadius, float fheight);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	std::string m_mTexturePass;			// テクスチャへパス
	int m_nNumPolygon;					// ポリゴン数
	int m_nNumVtx;						// 頂点数
	int m_nNumIdx;						// インデックス数
};

#endif // !_SYLINDER_H_
