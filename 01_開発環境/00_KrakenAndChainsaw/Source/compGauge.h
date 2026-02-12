//******************************************************************
// 
// ゲージコンポーネント
// Author Tetsuji Yamamoto
// 
//******************************************************************
#ifndef _COMPONENT_GAUGE_H_
#define _COMPONENT_GAUGE_H_

#include "component.h"

// 前方宣言
class CPolygon2D;

// ゲージコンポーネントクラス
class CGauge : public CComponent
{
public:
	CGauge();
	CGauge(const int nMax,const D3DXVECTOR3 pos, const  D3DXVECTOR3 rot, const  D3DXCOLOR col, const  float fWidth, const  float fHeight, const char* pTexturePass = nullptr);
	~CGauge();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// セッター
	void SetNow(int nNow) { m_nNow = nNow;}

private:
	CPolygon2D* m_ppolygon;	// ポリゴン
	int m_nMax;				// 最大値
	int m_nNow;				// 現在の値
};

#endif