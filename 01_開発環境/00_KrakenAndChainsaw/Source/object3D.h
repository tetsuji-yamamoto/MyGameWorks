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
	CObject3D(int nPriority = CObject::PRIORITY_3);
	~CObject3D();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, const char* pTexturePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void DrawNoTexture(void);
	static CObject3D* Create(void);

	// セッター
	void SetColor(D3DXCOLOR col);

	// ゲッター
	D3DXVECTOR3 GetVtxPos(int nNumVtx);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotasion(void) { return m_rot; }
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	float m_fDepth;						// 奥行き
	std::string m_mTexturePass;			// テクスチャへパス
};

#endif // !_OBJECT3D_H_
