//*************************************************
// 
// XYZ軸
// Author Tetsuji Yamamoto
// 
//*************************************************
#ifndef _XYZAXIS
#define _XYZAXIS

#include "object.h"

// XYZ軸クラス
class CXYZAxis : public CObject
{
public:
	static constexpr float LINELENGTH = 20.0f;

	CXYZAxis();
	~CXYZAxis();

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CXYZAxis* Create(D3DXVECTOR3 pos);

	// セッター
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }

	void Scaling(float fPlam);

	static void SwitchIsDisp(void) { m_bDisp = !m_bDisp; }

private:
	VERTEX_LINE m_aVtx[6];	// 頂点情報
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;		// 位置
	static bool m_bDisp;	// 表示切替
};


#endif // !_XYZAXIS
