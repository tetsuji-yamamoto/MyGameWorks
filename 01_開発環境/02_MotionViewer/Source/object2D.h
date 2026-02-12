//**********************************************
// 
// オブジェクト2D処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"

#define STATE_COUNT_NOMAL (60)

// オブジェクト2Dクラス
class CObject2D : public CObject // CObjectを継承
{
public:

	CObject2D(int nPriority = PRIORITY_NOMAL);
	~CObject2D();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass = NULL);
	void Uninit(void);
	void Update(void);
	void Update(float fPatternWidth, float fPatternHeight, int nNowPattern, int nTexLine);
	void Update(float fFeedRate, float fFeedAngle);
	void Draw(void);

	// セッター
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }

	// ゲッター
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXCOLOR GetColor(void) {return m_col; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }

	static CObject2D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass = NULL);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	int m_nIdxTexture;					// テクスチャのインデックス
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
};

#endif
