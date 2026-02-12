//**************************************************
// 
// クラーケン
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _KRAKEN_H_
#define _KRAKEN_H_

#include "object.h"
#include "myMath.h"

// 前方宣言
class CEntity;	// エンティティ

// クラーケンクラス
class CKraken : public CObject
{
public:
	static constexpr float	RadiusHitCapsule00		= 250.0f;								// カプセルの半径
	static constexpr float	RadiusHitCapsule01		= 200.0f;								// カプセルの半径
	static constexpr float	RadiusHitCapsule02		= 100.0f;								// カプセルの半径
	static constexpr int	LIFE_KRAKEN				= 2500;									// クラーケンの体力
	static constexpr int	LIFE_TENTACLE			= 100;									// 触手の体力
	static constexpr int	COUNTER_TENTACLE		= 600;									// 触手カウンター
	static constexpr int	TIME_MOTIONBLEND		= 10;									// モーションをブレンドする時間
	static constexpr int	ATTACK_POWER_TENTACLE	= -10;									// 触手攻撃力
	static constexpr float	SENSING_00_HEIGHT		= 300.0f;								// 検知高さ
	static constexpr float	SENSING_00_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲
	static constexpr float	SENSING_00_ANGLE		= D3DX_PI * 0.0f - D3DX_PI * 0.125f;	// 検知向き
	static constexpr float	SENSING_01_HEIGHT		= 300.0f;								// 検知高さ01
	static constexpr float	SENSING_01_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲01
	static constexpr float	SENSING_01_ANGLE		= D3DX_PI * 0.0f + D3DX_PI * 0.125f;	// 検知向き01
	static constexpr float	SENSING_02_HEIGHT		= 300.0f;								// 検知高さ02
	static constexpr float	SENSING_02_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲02
	static constexpr float	SENSING_02_ANGLE		= D3DX_PI * 0.25f + D3DX_PI * 0.125f;	// 検知向き02
	static constexpr float	SENSING_03_HEIGHT		= 300.0f;								// 検知高さ03
	static constexpr float	SENSING_03_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲03
	static constexpr float	SENSING_03_ANGLE		= D3DX_PI * 0.5f + D3DX_PI * 0.125f;	// 検知向き03
	static constexpr float	SENSING_04_HEIGHT		= 300.0f;								// 検知高さ04
	static constexpr float	SENSING_04_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲04
	static constexpr float	SENSING_04_ANGLE		= D3DX_PI * 0.75f + D3DX_PI * 0.125f;	// 検知向き04
	static constexpr float	SENSING_05_HEIGHT		= 300.0f;								// 検知高さ05
	static constexpr float	SENSING_05_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲05
	static constexpr float	SENSING_05_ANGLE		= -D3DX_PI * 0.75f - D3DX_PI * 0.125f;	// 検知向き05
	static constexpr float	SENSING_06_HEIGHT		= 300.0f;								// 検知高さ06
	static constexpr float	SENSING_06_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲06
	static constexpr float	SENSING_06_ANGLE		= -D3DX_PI * 0.5f - D3DX_PI * 0.125f;	// 検知向き06
	static constexpr float	SENSING_07_HEIGHT		= 300.0f;								// 検知高さ07
	static constexpr float	SENSING_07_ANGLERANGE	= D3DX_PI * 0.25f;						// 検知範囲07
	static constexpr float	SENSING_07_ANGLE		= -D3DX_PI * 0.25f - D3DX_PI * 0.125f;	// 検知向き07

	// モーションタイプ
	enum
	{
		MOTION_NEWTRAL = 0,		// 待機
		MOTION_ATTACK_PRE,		// 攻撃前振り
		MOTION_ATTACK_ING,		// 攻撃中
		MOTION_DAMAGE,			// ダメージ
		MOTION_DOWN_NEWTRAL,	// ダウン待機
		MOTION_MAX
	}MOTION;

	// ボーンタイプ
	enum
	{
		BONE_00 = 0,			// 起点
		BONE_HEAD,				// 頭部
		BONE_HEAD_JOINT,		// 頭部つなぎ目
		BONE_ABDOMEN_LOWER,		// 下腹部
		BONE_ABDOMEN,			// 腹部
		BONE_EYE_RIGHT,			// 右目
		BONE_EYE_LEFT,			// 左目

		BONE_TENTACLE_00_00,	// 第一触手01
		BONE_TENTACLE_00_01,	// 第一触手02
		BONE_TENTACLE_00_02,	// 第一触手03
		BONE_TENTACLE_00_03,	// 第一触手04
		BONE_TENTACLE_00_04,	// 第一触手05
		BONE_TENTACLE_00_05,	// 第一触手06
		
		BONE_TENTACLE_01_00,	// 第二触手01
		BONE_TENTACLE_01_01,	// 第二触手02
		BONE_TENTACLE_01_02,	// 第二触手03
		BONE_TENTACLE_01_03,	// 第二触手04
		BONE_TENTACLE_01_04,	// 第二触手05
		BONE_TENTACLE_01_05,	// 第二触手06
		
		BONE_TENTACLE_02_00,	// 第三触手01
		BONE_TENTACLE_02_01,	// 第三触手02
		BONE_TENTACLE_02_02,	// 第三触手03
		BONE_TENTACLE_02_03,	// 第三触手04
		BONE_TENTACLE_02_04,	// 第三触手05
		BONE_TENTACLE_02_05,	// 第三触手06

		BONE_TENTACLE_03_00,	// 第四触手01
		BONE_TENTACLE_03_01,	// 第四触手02
		BONE_TENTACLE_03_02,	// 第四触手03
		BONE_TENTACLE_03_03,	// 第四触手04
		BONE_TENTACLE_03_04,	// 第四触手05
		BONE_TENTACLE_03_05,	// 第四触手06

		BONE_TENTACLE_04_00,	// 第五触手01
		BONE_TENTACLE_04_01,	// 第五触手02
		BONE_TENTACLE_04_02,	// 第五触手03
		BONE_TENTACLE_04_03,	// 第五触手04
		BONE_TENTACLE_04_04,	// 第五触手05
		BONE_TENTACLE_04_05,	// 第五触手06

		BONE_TENTACLE_05_00,	// 第六触手01
		BONE_TENTACLE_05_01,	// 第六触手02
		BONE_TENTACLE_05_02,	// 第六触手03
		BONE_TENTACLE_05_03,	// 第六触手04
		BONE_TENTACLE_05_04,	// 第六触手05
		BONE_TENTACLE_05_05,	// 第六触手06

		BONE_TENTACLE_06_00,	// 第七触手01
		BONE_TENTACLE_06_01,	// 第七触手02
		BONE_TENTACLE_06_02,	// 第七触手03
		BONE_TENTACLE_06_03,	// 第七触手04
		BONE_TENTACLE_06_04,	// 第七触手05
		BONE_TENTACLE_06_05,	// 第七触手06

		BONE_TENTACLE_07_00,	// 第八触手01
		BONE_TENTACLE_07_01,	// 第八触手02
		BONE_TENTACLE_07_02,	// 第八触手03
		BONE_TENTACLE_07_03,	// 第八触手04
		BONE_TENTACLE_07_04,	// 第八触手05
		BONE_TENTACLE_07_05,	// 第八触手06
		BONE_MAX
	}BONE;

	CKraken();
	~CKraken();

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CKraken* Create(D3DXVECTOR3 pos);

	// セッター
	void SetMovementWhenLanding				(void);
	void SetCollisionCapsuleTriggerTentacle	(const int nStartBone, const std::string name);
	void SetIsDeath							(bool bDeath){m_bDeath = bDeath;}

	// ゲッター
	bool GetIsHitTentacle	( const std::string name, const std::string nameHit);
	bool GetIsDeath			(void){return m_bDeath;}

private:
	bool m_bDeath;	// 死亡フラグ
};

#endif