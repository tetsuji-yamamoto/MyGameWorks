//********************************************************
// 
// テクスチャコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_TEXTURE_H_
#define _COMPONENT_TEXTURE_H_

#include "component.h"

// テクスチャコンポーネントクラス
class CTextures : public CComponent
{
public:
	static constexpr int VERTEX_NUM = 4;// 頂点数

	// 引数構造体
	typedef struct TexturesArgument
	{
		// コンストラクタ
		TexturesArgument()
		{
		}

		// 変数
		std::string texturePath;// テクスチャパス
	}TexturesArgument;

	CTextures(TexturesArgument argment);
	~CTextures();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	TexturesArgument m_argument;		// 各種値
};
#endif