//**************************************************
// 
// タイトルロゴ
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "object.h"

// 前方宣言
class CPolygon2D;	// ポリゴン2D

// タイトルロゴクラス
class CTitleLogo : public CObject
{
public:
	CTitleLogo();
	~CTitleLogo();

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight,const char* pTexturePass = NULL);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CTitleLogo* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass = NULL);

private:
	CPolygon2D* m_pPolygon2D;	// 2Dポリゴン
};

#endif