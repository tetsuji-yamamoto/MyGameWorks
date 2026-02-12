//**************************************
// 
// 球
// Author Tetsuji Yamamoto
// 
//**************************************
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "main.h"
#include "object.h"

// 球クラス
class CSphere
{
public:
	CSphere();
	~CSphere();

	HRESULT Init(int nDivisionX, int nDivisionY, float fRadius, D3DXCOLOR col, const char* texturePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawWire(void);
	static CSphere* Create(int nDivisionX, int nDivisionY, float fRadius, D3DXCOLOR col, const char* texturePass);

	void SetMore(D3DXVECTOR3 pos,D3DXCOLOR col, float fRadius);

	// セッター
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	std::string m_mTexturePass;			// テクスチャへパス
	int m_nNumPolygonMeshField;			// メッシュのポリゴンの数
	int m_nNumVtx;						// 頂点数
	int m_nNumVtxFan;					// 扇状の頂点の数
	int m_nNumVtxMeshField;				//メッシュの頂点の数
	int m_nNumIdx;						// インデックス数
	int m_nNumIdxFan;					// ファンのインデック数
	int m_nNumIdxMeshField;				// メッシュフィールドのインデックス数
	int m_nDivisionX;					// 分割数X
	int m_nDivisionY;					// 分割数Y
	bool m_bWire;						// ワイヤー表示の有無
	float m_fRadius;					// 半径
};

#endif // !SPHERE_H_