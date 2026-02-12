//*****************************************
// 
// ビルボード
// Author Tetsuji Yamamoto
// 
//*****************************************
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

#include "main.h"
#include "object.h"

// オブジェクトビルボードクラス
class CObjectBillboard : public CObject
{
public:
	CObjectBillboard();
	~CObjectBillboard();
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight,const char* pTexturePass = NULL);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObjectBillboard* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass = NULL);

	// セッター
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetWidth(float fWidth);

	D3DXCOLOR GetColor(void) { return m_col; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	int m_nIdxTexture;					// テクスチャへのインデックス
};

#endif // !_OBJECTBILLBOARD_H_
