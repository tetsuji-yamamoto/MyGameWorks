//****************************************************
// 
// 2Dポリゴンコンポーネント
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_PORYGON2D_H_
#define _COMPONENT_PORYGON2D_H_

#include "main.h"
#include "component.h"

// ゲーム2Dクラス
class CCompPolygon2D : public CComponent
{
public:

	// 生成タイプ
	typedef enum
	{
		TYPE_CENTER = 0,	// 中央
		TYPE_LEFTBUTTOM,	// 左下
		TYPE_MAX
	}TYPE;

	// 情報
	typedef struct Argment
	{
		TYPE		type		= TYPE_CENTER;			// 生成タイプ
		D3DXVECTOR3	pos			= InitValu::VEC3_NULL;	// 位置
		D3DXVECTOR3	rot			= InitValu::VEC3_NULL;	// 向き
		D3DXCOLOR	col			= Color::_FULL;			// 色
		float		fWidth		= NULL;					// 幅
		float		fHeight		= NULL;					// 高さ
		std::string	TexturePath;						// テクスチャパス
	}Argment;

	CCompPolygon2D(Argment info);
	~CCompPolygon2D();

	void	Init				(void);
	void	InitCenter			(void);
	void	InitLeftBottom		(void);
	void	Uninit				(void);
	void	Update				(void);
	void	UpdateLeftBottom	(void);
	void	Draw				(void);

	void ScaleLeftBottomWidth(float fPalam);

	// セッター
	void	SetPosition	(D3DXVECTOR3 pos)	{ m_info.pos = pos; }
	void	SetRotation	(D3DXVECTOR3 rot)	{ m_info.rot = rot; }
	void	SetWidth	(float fWidth)		{ m_info.fWidth = fWidth; }
	void	SetHeight	(float fHeight)		{ m_info.fHeight = fHeight; }
	void	SetColor	(D3DXCOLOR col)		{ m_info.col = col; }

	// ゲッター
	D3DXVECTOR3	GetPosition	(void) { return m_info.pos; }
	D3DXVECTOR3	GetRotation	(void) { return m_info.rot; }
	D3DXCOLOR	GetColor	(void) { return m_info.col;}
	float		GetWidth	(void) { return m_info.fWidth; }
	float		GetHeight	(void) { return m_info.fHeight; }

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファ
	std::string				m_mTexturePass;	// テクスチャへパス
	Argment					m_info;			// 情報
};

#endif // !_GAME2D_H_