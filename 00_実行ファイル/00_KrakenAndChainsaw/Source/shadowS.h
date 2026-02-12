//****************************************************
// 
// ステンシルシャドウ
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _SHADOWS_H
#define _SHADOWS_H

#include "main.h"
#include "objectX.h"
#include "object.h"
#include "screen.h"

// ステンシルシャドウクラス
class CShadowS : public CObjectX	// CObjectX継承
{
public:
	CShadowS(int nPriority = PRIORITY_NOMAL);
	~CShadowS();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CShadowS* Create(float fRadius);

	void UpdatePosition(D3DXVECTOR3 pos);


	static void Draw(int* m_apIdxTexture, LPD3DXMESH m_pMesh, LPD3DXBUFFER m_pBuffMat, DWORD m_dwNumMat, D3DXMATRIX m_mtxWorld);
	static void DrawModel(int* m_apIdxTexture, LPD3DXMESH m_pMesh, LPD3DXBUFFER m_pBuffMat, DWORD m_dwNumMat, D3DXMATRIX m_mtxWorld);

private:
	D3DXMATRIX m_mtxScale;		// スケーリングマトリックス
	D3DXVECTOR3 m_pos;			// 位置
	float m_fRadius;			// 半径
};

#endif // !_SHADOWS_H

