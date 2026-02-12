//****************************************************
// 
// 2Dのポリゴン
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _PORYGON2D_H_
#define _PORYGON2D_H_

#include "main.h"
#include "object.h"

// ゲーム2Dクラス
class CPolygon2D : public CObject
{
public:
	// 情報
	typedef struct Info
	{
		D3DXVECTOR3	pos		= InitValu::VEC3_NULL;
		D3DXVECTOR3	rot		= InitValu::VEC3_NULL;
		D3DXCOLOR	col		= Color::_FULL;
		float		fWidth	= NULL;
		float		fHeight	= NULL;
	}Info;

	CPolygon2D();
	~CPolygon2D();

	HRESULT	Init				(void)	{return S_OK;}
	void	Init				(Info info,const char*pTexturePass = nullptr);
	void	InitLeftBottom		(Info info,const char*pTexturePass = nullptr);
	void	Uninit				(void);
	void	Update				(void);
	void	UpdateLeftBottom	(void);
	void	Draw				(void);

	void ScaleLeftBottomWidth(float fPalam);

	// セッター
	void	SetPosition	(D3DXVECTOR3 pos)	{ m_info.pos		= pos; }
	void	SetRotation	(D3DXVECTOR3 rot)	{ m_info.rot		= rot; }
	void	SetWidth	(float fWidth)		{ m_info.fWidth		= fWidth; }
	void	SetHeight	(float fHeight)		{ m_info.fHeight	= fHeight; }
	void	SetColor	(D3DXCOLOR col)		{ m_info.col		= col; }

	// ゲッター
	D3DXVECTOR3	GetPosition	(void) { return m_info.pos; }
	D3DXVECTOR3	GetRotation	(void) { return m_info.rot; }
	float		GetWidth	(void) { return m_info.fWidth; }
	float		GetHeight	(void) { return m_info.fHeight; }

	static CPolygon2D*	Create				(Info info, const char* pTexturePass = nullptr);
	static CPolygon2D*	CreateLeftBottom	(Info info, const char* pTexturePass = nullptr);

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファ
	std::string				m_mTexturePass;	// テクスチャへパス
	Info					m_info;			// 情報
};

#endif // !_GAME2D_H_