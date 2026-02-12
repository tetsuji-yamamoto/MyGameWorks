//**************************************
// 
// カプセル形状
// Author Tetsuji Yamamoto
// 
//**************************************
#ifndef _SHAPECAPSULE_H_
#define _SHAPECAPSULE_H_

#include "main.h"
#include "object.h"

class CShapeSylinder;
class CShapeSpher;

// カプセル形状クラス
class CShapeCapsule : public CObject
{
public:
	CShapeCapsule();
	~CShapeCapsule();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXCOLOR col, const int nDivisionX, const float fRadius, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawWire(void);

	static CShapeCapsule* Create(const D3DXCOLOR col, const int nDivisionX, const float fRadius, const float fHeight);

	// セッター
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMatrixParent(D3DXMATRIX mtx) { m_mtxParent = mtx; }
	void SetMore(const D3DXVECTOR3 pos, const D3DXQUATERNION quat, const D3DXCOLOR col, const D3DXMATRIX mtx, const float fRadius, const float fHeight);
private:
	CShapeSylinder* m_pSylinder;
	CShapeSpher* m_pSpherA;
	CShapeSpher* m_pSpherB;
	D3DXMATRIX m_mtxWorld;
	D3DXMATRIX m_mtxParent;
	D3DXQUATERNION m_quat;
	D3DXVECTOR3 m_posA;
	D3DXVECTOR3 m_posB;
	D3DXVECTOR3 m_pos;
};


#endif // !_SKY_H_
