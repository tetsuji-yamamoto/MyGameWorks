//**********************************************
// 
// 情報クラス
// Aunthor Tetsuji Yamamoto
// 
//**********************************************
#ifndef _COMPONENT_INFO_H_
#define _COMPONENT_INFO_H_

#include "component.h"
#include "hitResult.h"

// 情報クラス
class CInfo : public CComponent
{
public:
	CInfo()
	{
		mtxWorld = InitValu::MTX_NULL;		// ワールドマトリックス
		pos = InitValu::VEC3_NULL;			// 位置
		posOld = InitValu::VEC3_NULL;		// 古い位置
		posInverse = InitValu::VEC3_NULL;	// 逆行列を掛けた位置
		rot = InitValu::VEC3_NULL;			// 向き
		rotDest = InitValu::VEC3_NULL;		// 目的向き
		move = InitValu::VEC3_NULL;			// 移動量
		hitResult.Reset();					// 衝突結果
		fMovement = 0.0f;					// 移動量
		fFriction = 0.0f;					// 摩擦係数
		fFriction = 0.0f;					// 摩擦係数
		fGravity = 0.0f;					// 重力
		bRanding = false;					// 乗っているか否か
	}

	~CInfo()
	{

	}

	void Init() {}
	void Uninit() { delete this; }
	void Update() { }
	void Draw() {}


	void Reset(void)
	{
		mtxWorld = InitValu::MTX_NULL;		// ワールドマトリックス
		pos = InitValu::VEC3_NULL;			// 位置
		posOld = InitValu::VEC3_NULL;		// 古い位置
		posInverse = InitValu::VEC3_NULL;	// 逆行列を掛けた位置
		rot = InitValu::VEC3_NULL;			// 向き
		rotDest = InitValu::VEC3_NULL;		// 目的向き
		move = InitValu::VEC3_NULL;			// 移動量
		hitResult.Reset();					// 衝突結果
		fMovement = 0.0f;					// 移動量
		bRanding = false;					// 乗っているか否か
	}

	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXMATRIX* pMtxRnding;		// 乗っている物のマトリックス
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 古い位置
	D3DXVECTOR3 posInverse;		// 逆行列を掛けた位置
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 rotDest;		// 目的向き
	D3DXVECTOR3 move;			// 移動量
	CHitResult hitResult;		// 衝突結果
	float fMovement;			// 移動量
	float fFriction;			// 摩擦係数
	float fGravity;				// 重力
	bool bRanding;				// 乗っているか否か
};

#endif