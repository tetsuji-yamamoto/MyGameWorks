//**********************************************
// 
// エフェクト処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "object2D.h"

// エフェクトクラス
class CEffect2D : public CObject2D // CObject2Dを継承
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_NOMAL,		// 通常
		TYPE_SHAKING,	// 揺れるshaking
		TYPE_MAX
	}TYPE;

	CEffect2D(int nPriority = PRIORITY_NOMAL);
	~CEffect2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEffectType(TYPE type);
	void SetLife(int nLife) { m_nLife = nLife; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	int GetLife(void) { return m_nLife; }
	int GetMaxLife(void) { return m_nMaxLife; }
	float GetMaxWidth(void) { return m_fMaxWidth; }
	float GetMaxHeight(void) { return m_fMaxHeight; }
	D3DXCOLOR GetMaxColor(void) { return m_MaxCol; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
private:
	TYPE m_type;		// タイプ
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_MaxMove;	// 最大い移動量
	D3DXCOLOR m_MaxCol;	// 最大色
	int m_nLife;		// 寿命
	int m_nMaxLife;		// 最大寿命
	float m_fMaxWidth;	// 幅
	float m_fMaxHeight;	// 高さ
};

// エフェクトクラス
class CEffect2DNomal : public CEffect2D // CObject2Dを継承
{
public:
	CEffect2DNomal();
	~CEffect2DNomal();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect2DNomal* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col, int nLife, float fWidth, float fHeight);

private:
};

// エフェクトクラス
class CEffect2DShaking : public CEffect2D // CObject2Dを継承
{
public:
	CEffect2DShaking();
	~CEffect2DShaking();

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fWidth, float fHeight, float fShakRadius,float fAddRot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect2DShaking* Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fWidth, float fHeight, float fShakRadius, float fAddRot);

private:
	float m_fStartRot;
	float m_fAddRot;
	float m_fShakRadius;
};

#endif
