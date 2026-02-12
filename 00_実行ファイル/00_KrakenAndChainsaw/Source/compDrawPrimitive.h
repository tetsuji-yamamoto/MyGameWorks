//********************************************************
// 
// 描画コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_DRAWPRIMITIVE_H_
#define _COMPONENT_DRAWPRIMITIVE_H_

#include "component.h"

// メッシュフィールドコンポーネントクラス
class CDrawPrimitive : public CComponent
{
public:

	// 引数構造体
	typedef struct DrawArgument
	{
		// コンストラクタ
		DrawArgument()
		{
			primitiveType = D3DPT_TRIANGLESTRIP;	// 描画タイプ
			nNumVertex = 0;							// 頂点数
			nNumPrimitive = 0;						// 描画数
		}

		// 変数
		D3DPRIMITIVETYPE primitiveType;	// 描画タイプ
		int nNumVertex;					// 頂点数
		int nNumPrimitive;				// 描画数
	}DrawArgument;

	CDrawPrimitive(DrawArgument argment);
	~CDrawPrimitive();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	DrawArgument m_argument;	// 各種値
};
#endif