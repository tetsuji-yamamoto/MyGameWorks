//********************************************************
// 
// 船用変数コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_SHIP_VARIABLE_H_
#define _COMPONENT_SHIP_VARIABLE_H_

#include "component.h"

//船用変数コンポーネントクラス
class CShipVariable : public CComponent
{
public:

	CShipVariable();
	~CShipVariable();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// ゲッター
	D3DXVECTOR3 GetPositionCenter(void) { return m_posCenter; }
	float GetAngleMove(void) { return m_fAngleMove; }
	float GetAngleMoveDest(void) { return m_fAngleMoveDest; }
	float GetMovement(void) { return m_fMovement; }
	float GetRadius(void) { return m_fRadius; }

	// セッター
	void SetPositionCenter(D3DXVECTOR3 pos) { m_posCenter = pos; }
	void SetAngleMove(float fAngle) { m_fAngleMove = fAngle; }
	void SetAngleMoveDest(float fAngle) { m_fAngleMoveDest = fAngle; }
	void SetMovement(float fMovement) { m_fMovement = fMovement; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetfFriction(float fFriction) { m_fFriction = fFriction; }

private:
	D3DXVECTOR3 m_posCenter;	// 移動中心位置
	float m_fAngleMove;			// 移動角度
	float m_fAngleMoveDest;		// 目的移動角度
	float m_fMovement;			// 移動量
	float m_fRadius;			// 半径
	float m_fFriction;			// 摩擦係数
};
#endif