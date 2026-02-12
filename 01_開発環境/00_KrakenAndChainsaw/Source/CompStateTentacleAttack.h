//**********************************************
// 
// タコ触手状態攻撃コンポーネントクラス
// Author Tetsuji Yamamoto
//
//**********************************************
#ifndef _COMPONENT_STATE_ATTACK_TENTACLE_H_
#define _COMPONENT_STATE_ATTACK_TENTACLE_H_

#include "component.h"
#include "CompStateTentacleNewtral.h"

// 前方宣言
class CPartMotion;
class CCounter;
class CPlayer;

// 待機クラス
class CCompStateTentacleAttack : public CComponent
{
public:

	static constexpr int	TIME_MOTIONBLEND	= 10;		// モーションをブレンドする時間
	static constexpr int	BONEIDX_START		= 2;		// ボーンインデックスのスタート数
	static constexpr int	BONEIDX_NUM			= 4;		// 使うボーン数
	static constexpr float	OffsetScale			= 0.5f;		// オフセットのスケール値
	static constexpr float	RadiusHitCapsule00	= 200.0f;	// カプセルの半径
	static constexpr float	RadiusHitCapsule01	= 180.0f;	// カプセルの半径
	static constexpr float	RadiusHitCapsule02	= 150.0f;	// カプセルの半径
	static constexpr float	LengthScale			= 0.5f;		// カプセルの長さのスケール値

	// 使うボーンの列挙
	typedef enum
	{
		BONEIDX_02 = 2,
		BONEIDX_03,
		BONEIDX_04,
		BONEIDX_05,
		BONEIDX_MAX,
	}BONEIDX;

	// 攻撃する時間しない時間列挙
	enum
	{
		ATTACKTIME_00_NONE		= 273,											// 攻撃しない時間
		ATTACKTIME_01_ATTACK	= 50,											// 攻撃する時間
		ATTACKTIME_MAX			= ATTACKTIME_00_NONE + ATTACKTIME_01_ATTACK,	// 攻撃する時間
	}ATTACKTIME;

	// 引数情報
	typedef struct Argument
	{
		CCompStateTentacleNewtral::Argument	NewtralArg;				// 待機状態の引数
		std::string							CapsuleColliderName;	// カプセルコライダー名
	}Argument;

	CCompStateTentacleAttack(Argument arg);
	~CCompStateTentacleAttack();

	void Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}

	void AddCapsuleCollider(void);

private:
	Argument		m_Arg;			// 引数情報
	CCounter*		m_pMyCounter;	// カウンターのポインタ
	bool			m_bAttack;		// 攻撃してるか否か
};
#endif