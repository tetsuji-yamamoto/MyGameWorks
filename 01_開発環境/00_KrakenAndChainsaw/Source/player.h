//**************************************************
// 
// プレイヤー
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "myMath.h"
#include "compCollision.h"
#include "hitResult.h"
#include "compInfo.h"

// 前方宣言
class CEntity;	// エンティティ

// プレイヤークラス
class CPlayer : public CObject
{
public:

	static constexpr int	TIME_AVERAGE	= 120;	// 時間並み
	static constexpr int	TIME_LARGE		= 180;	// 時間大
	static constexpr int	DAMAGE			= -10;	// ダメージの加算地
	static constexpr int	LIFE			= 100;	// 体力

	// 攻撃レベル
	enum
	{
		ATTACK_SMALL = 0,	// 小
		ATTACK_AVERAGE,		// 並み
		ATTACK_LARGE,		// 大
	}ATTACK;

	// モーション
	enum
	{
		MOTION_NEWTRAL = 0,		// 待機
		MOTION_MOVE,			// 移動
		MOTION_ATTACK_PRE,		// 攻撃開始
		MOTION_ATTACK_ING,		// 攻撃中
		MOTION_ATTACK_GAP,		// 攻撃終了
		MOTION_BLOWOFF,			// 吹っ飛ばされ
		MOTION_FALLDOWN,		// 倒れこみ
		MOTION_GETUP,			// 起き上がり
		MOTION_CROUCHING,		// しゃがみ
		MOTION_JUMP,			// ジャンプ
		MOTION_AIR,				// 空中
		MOTION_RANDING,			// 着地
		MOTION_DRIVING,			// 運転
		MOTION_STOP,			// モーションストップ
		MOTION_MAX
	}MOTION;

	CPlayer();
	~CPlayer();

	HRESULT			Init		(void);
	void			Init		(D3DXVECTOR3 pos);
	void			Uninit		(void);
	void			Update		(void);
	void			Draw		(void);
	void			Correction	(void);
	static CPlayer*	Create		(D3DXVECTOR3 pos);
	
	void SetMovementWhenLanding(void);
	void SetMovementWhenNodLanding(void);

	// ゲッター
	int		GetAttacklevel				(void);
	bool	GetIsAttackKraken			(void);
	bool	GetIsAttackKrakenTentacle	(void);
	float	GetPalamMaximum				(void){return m_fPalamMaximum;}
	int		GetAttackPower				(void){return m_nAttackPower;}

	// セッター
	void	SetPalamMaximum	(float fPlam)	{m_fPalamMaximum = fPlam;}
	void	SetAttackPower	(int nPower)	{m_nAttackPower = nPower;}

private:
	int		m_nLife;			// 体力
	bool	m_bAttackedKraken;	// クラーケンへの攻撃有無
	float	m_fPalamMaximum;	// 最高倍率
	int		m_nAttackPower;		// 攻撃力
};

#endif