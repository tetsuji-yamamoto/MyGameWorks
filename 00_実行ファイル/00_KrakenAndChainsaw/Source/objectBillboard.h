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
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CObjectBillboard* Create(void);

	// セッター
	void SetTexture(std::string textureFileName);
	void SetPosition(D3DXVECTOR3 pos);
	void SetRadius(float fRadius);
	void SetColor(D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	std::string m_mTexturePass;			// テクスチャへパス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅
	float m_fheight;					// 高さ
};

#endif // !_OBJECTBILLBOARD_H_
