//**********************************************
// 
// プレイヤー処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "object2D.h"
#include "state.h"

#define MAX_PLAYER (3)	// プレイヤーの最大数

// プレイヤークラス
class CPlayer : public CObject2D
{
public:
	CPlayer(int nPriority = PRIORITY_NOMAL);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPatternTex, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTexAnim(int nPatternTex, int nCntPattern);
	static CPlayer* Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot, float fWidth, float fHeight, int nPatternTex, int nLife);
	void Hit(int nDamage);
	void SetLife(int nLife) { m_nLife = nLife; }
	int GetLife(void) { return m_nLife; }

private:
	static int m_nPlayerNum;				// プレイヤーの数
	int m_nMaxTexPattern;					// テクスチャのパターン数
	int m_nNowPattern;						// テクスチャの今のパターン数
	int m_nCntNextPattern;					// パターンカウンター
	int m_nLife;							// 体力
	D3DXVECTOR3 m_move;						// 移動量
	CState* m_pState;						// 状態
};

#endif // !PLAYER_H_
