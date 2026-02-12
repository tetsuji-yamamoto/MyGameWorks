//***********************************************
// 
// 丸影
// Author Tetsuji Yamamoto
// 
//***********************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

// 影クラス
class CShadow : public CObject	// CObject継承
{
public:
	CShadow();
	~CShadow();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CShadow* Create(float fRadius);

	void SetPosition(D3DXVECTOR3 pos);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXMATRIX m_mtxTrans;				// 拡大縮小行列
	D3DXMATRIX m_mtxRot;				// 回転行列
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	float m_fRadius;					// 半径
};

#endif // !_SHADOW_H_
