//********************************************************
// 
// 位置ポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENT_TRANSFORM_H_

#include "component.h"

// 位置コンポーネントクラス
class CTransfom : public CComponent
{
public:

	// 引数構造体
	typedef struct ArgumentTransfom
	{
		// コンストラクタ
		ArgumentTransfom()
		{
			fGravity = 0.0f;			// 重力
			pos = InitValu::VEC3_NULL;	// 位置
			quat = InitValu::QUAT_NULL;	// 向き
			move = InitValu::VEC3_NULL;	// 移動量
		}

		// 変数
		float fGravity;			// 重力
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 move;		// 移動量
		D3DXQUATERNION quat;	// 向き
	}ArgumentTransfom;

	CTransfom(ArgumentTransfom argment);
	~CTransfom();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MulQuat(D3DXQUATERNION quat);
	void MulMtxAddRot(const D3DXMATRIX mtxRot);

	// セッター
	void SetPotision(D3DXVECTOR3 pos) { m_argument.pos = pos; }
	void SetQuat(D3DXQUATERNION quat) { m_argument.quat = quat; }
	void SetMtxAddRot(const D3DXMATRIX mtxRot) { m_mtxAddRot = mtxRot; }

	D3DXVECTOR3 GetMove(void) { return m_argument.move; }
	D3DXMATRIX GetmtxRot(void) { return  m_mtxAddRot; }

private:
	ArgumentTransfom m_argument;	// 各種値
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX m_mtxAddRot;			// 追加の回転マトリックス
};
#endif