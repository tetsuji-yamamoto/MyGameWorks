//**************************************************
// 
// 範囲ポリゴン
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _RANGE2D_H_
#define _RANGE2D_H_

#include "object.h"

// 範囲ポリゴンクラス
class CRange2D : public CObject
{
public:
	CRange2D();
	~CRange2D();

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass = NULL);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRange2D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass = NULL);
	
	// セッター
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// ゲッター
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
private:
	std::string  m_texturePass;	// テクスチャへパス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
};

#endif