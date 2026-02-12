//********************************************************
// 
// 3Dポリゴンコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_POLYGON3D_H_
#define _COMPONENT_POLYGON3D_H_

#include "component.h"

// メッシュフィールドコンポーネントクラス
class CPolygon3D : public CComponent
{
public:
	static constexpr int VERTEX_NUM = 4;// 頂点数

	// 引数構造体
	typedef struct Polygon3DArgument
	{
		// コンストラクタ
		Polygon3DArgument()
		{
			col = Color::_WHITE;	// 色
			fWidth = 100.0f;		// 幅
			fHeight = 0.0f;			// 高さ
			fDips = 100.0f;			// 高さ
		}

		// 変数
		D3DXCOLOR col;				// 色
		float fWidth;				// 幅
		float fHeight;				// 高さ
		float fDips;				// 奥行き
	}Polygon3DArgument;

	CPolygon3D(Polygon3DArgument argment);
	~CPolygon3D();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXCOLOR GetColor(void){return m_argument.col;}
	void SetColor(D3DXCOLOR col);
	void SetSize(float fWidth,float fHeight,float fDips);
	void Reflection(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	Polygon3DArgument m_argument;		// 各種値
};
#endif