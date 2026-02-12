//**********************************************************************
// 
// 壁
// Author Tetsuji Yamamoto
// 
//**********************************************************************
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "object3D.h"

// 壁クラス
class CWall : public CObject3D // CObject3D継承
{
public:
	CWall();
	~CWall();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, const char* pTexturePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CWall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, const char* pTexturePass);

	bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

private:
	D3DXVECTOR3 m_posA;	// ポリゴンの頂点
	D3DXVECTOR3 m_posB;	// ポリゴンの頂点
	D3DXVECTOR3 m_posC;	// ポリゴンの頂点
};

#endif // !_WALL_H_
