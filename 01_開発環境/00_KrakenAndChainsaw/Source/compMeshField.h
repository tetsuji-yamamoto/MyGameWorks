//********************************************************
// 
// メッシュフィールドコンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_MESHFIELD_H_
#define _COMPONENT_MESHFIELD_H_

#include "component.h"

// メッシュフィールドコンポーネントクラス
class CMeshField : public CComponent
{
public:
	// 引数構造体
	typedef struct MeshFIeldArgument
	{
		// コンストラクタ
		MeshFIeldArgument()
		{
			posStart = InitValu::VEC3_NULL;		// 始点
			col = Color::_WHITE;				// 色
			fWidth = 100.0f;					// 幅
			fHeight = 100.0f;					// 高さ
			nDivisionX = 1;						// 横の分割数
			nDivisionY = 1;						// 縦の分割数
		}

		// 変数
		std::string texturePass;	// テクスチャパス
		D3DXVECTOR3 posStart;		// 始点
		D3DXCOLOR col;				// 色
		float fWidth;				// 幅
		float fHeight;				// 高さ
		int nDivisionX;				// 横の分割数
		int nDivisionY;				// 縦の分割数
	}MeshFIeldArgument;

	CMeshField(MeshFIeldArgument argment);
	~CMeshField();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ResetNormal(void);
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* pNor = NULL);
	void EditHeight(D3DXVECTOR3 pos, float fHeight, float fRadius);
	void Save(const char* pFilePass);
	void Load(const char* pFilePass);

	// セッター
	void SetColor(D3DXCOLOR col);
	void SetColor(float fHeight, D3DXCOLOR colTop, D3DXCOLOR colBottom);
	void SetPosition(D3DXVECTOR3 pos, float fWidth, float fHeight);

	// セッター
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	MeshFIeldArgument m_argument;		// 各種値
	int m_nNumPolygon;					// ポリゴン数
	int m_nNumVtx;						// 頂点数
	int m_nNumIdx;						// インデックス数
};
#endif