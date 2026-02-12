//**********************************************
// 
// ナンバー
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "object2D.h"


// ナンバークラス
class CNumber
{
public:
	CNumber();
	~CNumber();

	void Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,int nNumber = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CNumber* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber = 0);

	// ゲッター

	// セッター
	void SetNumber(int nNumber);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	int m_nNowNumber;						// 今の番号
	int m_nIdxTexture;						// テクスチャのインデックス
};

#endif // !_NUMBER_H_
