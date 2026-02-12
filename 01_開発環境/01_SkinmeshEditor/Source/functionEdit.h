//**************************************************
// 
// 機能編集クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTIONEDIT_H_
#define _FANCTIONEDIT_H_

#include "functionBase.h"
#include "manager.h"
#include "edit.h"
#include "camera.h"
#include "input.h"

// 機能無し編集クラス
class CFunctionEditNone : public CFunctionEditBase
{
public:
	CFunctionEditNone() {}
	~CFunctionEditNone() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;
	void Debug(void) override { CManager::GetEdit()->Debug(); CManager::GetEdit()->DebugNone();}

	void WheelPlas(void) override{}
	void WheelMines(void) override {}
	void CrickL(void) override {}
	void CrickR(void) override {}
	void CrickCenter(void) override {}
	void LShift(void) override {}
	void RShift(void) override {}
	void Return(void) override {}
	void Right(void) override {}
	void Left(void) override {}
	void Up(void) override {}
	void Down(void) override {}
	void F1(void) override;
	void F2(void) override {}
	void F3(void) override {}
	void F4(void) override {}
	void F5(void) override {}
	void F6(void) override {}
	void F7(void) override {}
	void F8(void) override {}
	void F9(void) override {}
	void F10(void) override {}
	void F12(void) override {}
	void A(void) override {}
	void B(void) override {}
	void C(void) override {}
	void D(void) override {}
	void E(void) override {}
	void F(void) override {}
	void G(void) override {}
	void H(void) override {}
	void I(void) override {}
	void J(void) override {}
	void K(void) override {}
	void L(void) override {}
	void M(void) override {}
	void N(void) override {}
	void O(void) override {}
	void P(void) override {}
	void Q(void) override {}
	void R(void) override {}
	void S(void) override {}
	void T(void) override {}
	void U(void) override {}
	void V(void) override {}
	void W(void) override {}
	void X(void) override {}
	void Y(void) override {}
	void Z(void) override {}

private:
};

// 機能リギング編集クラス
class CFunctionEditRigging : public CFunctionEditBase
{
public:
	CFunctionEditRigging() {}
	~CFunctionEditRigging() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;
	void Debug(void) override { CManager::GetEdit()->Debug(); CManager::GetEdit()->DebugRigging(); }

	void WheelPlas(void) override {}
	void WheelMines(void) override {}
	void CrickL(void) override {}
	void CrickR(void) override {}
	void CrickCenter(void) override {}
	void LShift(void) override {}
	void RShift(void) override {}
	void Return(void) override {}
	void Right(void) override {}
	void Left(void) override {}
	void Up(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_UP) == true) CManager::GetEdit()->SelectBone(1); }
	void Down(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_DOWN) == true)  CManager::GetEdit()->SelectBone(-1); }
	void F1(void) override ;
	void F2(void) override {}
	void F3(void) override {}
	void F4(void) override {}
	void F5(void) override {}
	void F6(void) override {}
	void F7(void) override {}
	void F8(void) override {}
	void F9(void) override {}
	void F10(void) override {}
	void F12(void) override {}
	void A(void) override {}
	void B(void) override {}
	void C(void) override {}
	void D(void) override {}
	void E(void) override {}
	void F(void) override {}
	void G(void) override {}
	void H(void) override {}
	void I(void) override {}
	void J(void) override {}
	void K(void) override {}
	void L(void) override {}
	void M(void) override {}
	void N(void) override {}
	void O(void) override {}
	void P(void) override {}
	void Q(void) override {}
	void R(void) override {}
	void S(void) override {}
	void T(void) override {}
	void U(void) override {}
	void V(void) override {}
	void W(void) override {}
	void X(void) override {}
	void Y(void) override {}
	void Z(void) override {}

private:
};

// 機能スキニング編集クラス
class CFunctionEditSkinning : public CFunctionEditBase
{
public:
	CFunctionEditSkinning() {}
	~CFunctionEditSkinning() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;
	void Debug(void) override { CManager::GetEdit()->Debug(); CManager::GetEdit()->DebugSknning(); }

	void WheelPlas(void) override {}
	void WheelMines(void) override {}
	void CrickL(void) override {}
	void CrickR(void) override {}
	void CrickCenter(void) override {}
	void LShift(void) override {}
	void RShift(void) override {}
	void Return(void) override {}
	void Right(void) override {}
	void Left(void) override {}
	void Up(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_UP) == true)CManager::GetEdit()->SelectBone(1); }
	void Down(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_DOWN) == true)CManager::GetEdit()->SelectBone(-1); }
	void F1(void) override ;
	void F2(void) override {}
	void F3(void) override {}
	void F4(void) override {}
	void F5(void) override {}
	void F6(void) override {}
	void F7(void) override {}
	void F8(void) override {}
	void F9(void) override {}
	void F10(void) override {}
	void F12(void) override {}
	void A(void) override {}
	void B(void) override {}
	void C(void) override {}
	void D(void) override {}
	void E(void) override {}
	void F(void) override {}
	void G(void) override {}
	void H(void) override {}
	void I(void) override {}
	void J(void) override {}
	void K(void) override {}
	void L(void) override {}
	void M(void) override {}
	void N(void) override {}
	void O(void) override {}
	void P(void) override {}
	void Q(void) override {}
	void R(void) override {}
	void S(void) override {}
	void T(void) override {}
	void U(void) override {}
	void V(void) override {}
	void W(void) override {}
	void X(void) override {}
	void Y(void) override {}
	void Z(void) override {}

private:
};

// 武器編集クラス
class CFunctionEditWeapon : public CFunctionEditBase
{
public:
	CFunctionEditWeapon() {}
	~CFunctionEditWeapon() {}

	void Init(void) override;
	void Uninit(void) override { delete this; }
	void Update(void) override;
	void Debug(void) override { CManager::GetEdit()->Debug(); CManager::GetEdit()->DebugWeapon(); }

	void WheelPlas(void) override { CManager::GetCamera()->OperateMouseDistance(); }	// カメラ距離操作
	void WheelMines(void) override { CManager::GetCamera()->OperateMouseDistance(); }	// カメラ距離操作
	void CrickL(void) override { CManager::GetCamera()->OperateMousePosV(); }			// カメラ視点操作
	void CrickR(void) override { CManager::GetCamera()->OperateMousePosR(); }			// カメラ注視点操作
	void CrickCenter(void) override { CManager::GetCamera()->OperateMouseSlide(); }		// カメラスライド処理
	void LShift(void) override {}
	void RShift(void) override {}
	void Return(void) override {}
	void Right(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_RIGHT) == true) CManager::GetEdit()->WeaponSelectFollowBone(1); }	// 武器の親ボーン切替
	void Left(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_LEFT) == true) CManager::GetEdit()->WeaponSelectFollowBone(-1); }	// 武器の親ボーン切替
	void Up(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_UP) == true) CManager::GetEdit()->WeaponSelectOperate(1); }			// 操作する武器選択
	void Down(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_DOWN) == true) CManager::GetEdit()->WeaponSelectOperate(-1); }		// 操作する武器選択
	void F1(void) override;
	void F2(void) override {}
	void F3(void) override {}
	void F4(void) override {}
	void F5(void) override {}
	void F6(void) override {}
	void F7(void) override {}
	void F8(void) override {}
	void F9(void) override {}
	void F10(void) override {}
	void F12(void) override {}
	void A(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_A) == true)CManager::GetEdit()->WeaponAddPosition(D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); }
	void B(void) override { CManager::GetEdit()->WeaponResetRotate(); }
	void C(void) override {}
	void D(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_D) == true) CManager::GetEdit()->WeaponAddPosition(D3DXVECTOR3(1.0f, 0.0f, 0.0f)); }
	void E(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_E) == true) CManager::GetEdit()->WeaponAddPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f)); }
	void F(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_F) == true) CManager::GetEdit()->WeaponAddRotate(D3DXVECTOR3(0.0f, 1.0f, 0.0f)); }
	void G(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_G) == true) CManager::GetEdit()->WeaponAddRotate(D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); }
	void H(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_H) == true) CManager::GetEdit()->WeaponAddRotate(D3DXVECTOR3(0.0f, -1.0f, 0.0f)); }
	void I(void) override {}
	void J(void) override {}
	void K(void) override {}
	void L(void) override {}
	void M(void) override {}
	void N(void) override {}
	void O(void) override {}
	void P(void) override {}
	void Q(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_Q) == true)CManager::GetEdit()->WeaponAddPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f)); }
	void R(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_R) == true)CManager::GetEdit()->WeaponAddRotate(D3DXVECTOR3(0.0f, 0.0f, 1.0f)); }
	void S(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_S) == true)CManager::GetEdit()->WeaponAddPosition(D3DXVECTOR3(0.0f, 0.0f, -1.0f)); }
	void T(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_T) == true)CManager::GetEdit()->WeaponAddRotate(D3DXVECTOR3(1.0f, 0.0f, 0.0f)); }
	void U(void) override {}
	void V(void) override {}
	void W(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_W) == true)CManager::GetEdit()->WeaponAddPosition(D3DXVECTOR3(0.0f, 0.0f, 1.0f)); }
	void X(void) override { CManager::GetEdit()->WeaponResetPosition(); }
	void Y(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_Y) == true)CManager::GetEdit()->WeaponAddRotate(D3DXVECTOR3(0.0f, 0.0f, -1.0f)); }
	void Z(void) override {}

private:
};

// 当たり判定用武器編集クラス
class CFunctionEditCollModel : public CFunctionEditBase
{
public:
	CFunctionEditCollModel() {}
	~CFunctionEditCollModel() {}

	void Init(void) override;
	void Uninit(void) override { delete this; }
	void Update(void) override;
	void Debug(void) override { CManager::GetEdit()->Debug(); CManager::GetEdit()->DebugCollModel(); }

	void WheelPlas(void) override { CManager::GetCamera()->OperateMouseDistance(); }	// カメラ距離操作
	void WheelMines(void) override { CManager::GetCamera()->OperateMouseDistance(); }	// カメラ距離操作
	void CrickL(void) override { CManager::GetCamera()->OperateMousePosV(); }			// カメラ視点操作
	void CrickR(void) override { CManager::GetCamera()->OperateMousePosR(); }			// カメラ注視点操作
	void CrickCenter(void) override { CManager::GetCamera()->OperateMouseSlide(); }		// カメラスライド処理
	void LShift(void) override {}
	void RShift(void) override {}
	void Return(void) override {}
	void Right(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_RIGHT) == true) CManager::GetEdit()->CollModelSelectFollowBone(1); }	// 武器の親ボーン切替
	void Left(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_LEFT) == true) CManager::GetEdit()->CollModelSelectFollowBone(-1); }	// 武器の親ボーン切替
	void Up(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_UP) == true) CManager::GetEdit()->CollModelSelectOperate(1); }			// 操作する武器選択
	void Down(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_DOWN) == true) CManager::GetEdit()->CollModelSelectOperate(-1); }		// 操作する武器選択
	void F1(void) override;
	void F2(void) override {}
	void F3(void) override {}
	void F4(void) override {}
	void F5(void) override {}
	void F6(void) override {}
	void F7(void) override {}
	void F8(void) override {}
	void F9(void) override {}
	void F10(void) override {}
	void F12(void) override {}
	void A(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_A) == true)CManager::GetEdit()->CollModelAddPosition(D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); }
	void B(void) override { CManager::GetEdit()->CollModelResetRotate(); }
	void C(void) override {}
	void D(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_D) == true) CManager::GetEdit()->CollModelAddPosition(D3DXVECTOR3(1.0f, 0.0f, 0.0f)); }
	void E(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_E) == true) CManager::GetEdit()->CollModelAddPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f)); }
	void F(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_F) == true) CManager::GetEdit()->CollModelAddRotate(D3DXVECTOR3(0.0f, 1.0f, 0.0f)); }
	void G(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_G) == true) CManager::GetEdit()->CollModelAddRotate(D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); }
	void H(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_H) == true) CManager::GetEdit()->CollModelAddRotate(D3DXVECTOR3(0.0f, -1.0f, 0.0f)); }
	void I(void) override {}
	void J(void) override {}
	void K(void) override {}
	void L(void) override {}
	void M(void) override {}
	void N(void) override {}
	void O(void) override {}
	void P(void) override {}
	void Q(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_Q) == true)CManager::GetEdit()->CollModelAddPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f)); }
	void R(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_R) == true)CManager::GetEdit()->CollModelAddRotate(D3DXVECTOR3(0.0f, 0.0f, 1.0f)); }
	void S(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_S) == true)CManager::GetEdit()->CollModelAddPosition(D3DXVECTOR3(0.0f, 0.0f, -1.0f)); }
	void T(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_T) == true)CManager::GetEdit()->CollModelAddRotate(D3DXVECTOR3(1.0f, 0.0f, 0.0f)); }
	void U(void) override {}
	void V(void) override {}
	void W(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_W) == true)CManager::GetEdit()->CollModelAddPosition(D3DXVECTOR3(0.0f, 0.0f, 1.0f)); }
	void X(void) override { CManager::GetEdit()->CollModelResetPosition(); }
	void Y(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_Y) == true)CManager::GetEdit()->CollModelAddRotate(D3DXVECTOR3(0.0f, 0.0f, -1.0f)); }
	void Z(void) override {}

private:
};

// 機能プレビュー編集クラス
class CFunctionEditPreviview : public CFunctionEditBase
{
public:
	CFunctionEditPreviview() {}
	~CFunctionEditPreviview() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;

	void Debug(void) override { CManager::GetEdit()->Debug(); CManager::GetEdit()->DebugPreview(); }

	void WheelPlas(void) override { CManager::GetCamera()->OperateMouseDistance(); }	// カメラ距離操作
	void WheelMines(void) override { CManager::GetCamera()->OperateMouseDistance(); }	// カメラ距離操作
	void CrickL(void) override { CManager::GetCamera()->OperateMousePosV(); }			// カメラ視点操作
	void CrickR(void) override { CManager::GetCamera()->OperateMousePosR(); }			// カメラ注視点操作
	void CrickCenter(void) override { CManager::GetCamera()->OperateMouseSlide(); }		// カメラスライド処理
	void LShift(void) override {}
	void RShift(void) override {}
	void Return(void) override {}
	void Right(void) override {}
	void Left(void) override {}
	void Up(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_UP) == true)CManager::GetEdit()->SelectBone(1); }
	void Down(void) override { if (CManager::GetInputKeyboard()->GetTriggerPress(DIK_DOWN) == true)CManager::GetEdit()->SelectBone(-1); }
	void F1(void) override;
	void F2(void) override {}
	void F3(void) override {}
	void F4(void) override {}
	void F5(void) override {}
	void F6(void) override {}
	void F7(void) override {}
	void F8(void) override {}
	void F9(void) override {}
	void F10(void) override {}
	void F12(void) override {}
	void A(void) override {}
	void B(void) override {}
	void C(void) override {}
	void D(void) override {}
	void E(void) override {}
	void F(void) override {}
	void G(void) override {}
	void H(void) override {}
	void I(void) override {}
	void J(void) override {}
	void K(void) override {}
	void L(void) override {}
	void M(void) override {}
	void N(void) override {}
	void O(void) override {}
	void P(void) override {}
	void Q(void) override {}
	void R(void) override {}
	void S(void) override {}
	void T(void) override {}
	void U(void) override {}
	void V(void) override {}
	void W(void) override {}
	void X(void) override {}
	void Y(void) override {}
	void Z(void) override {}

private:
};

// 機能ライトクラス
class CFunctionLight : public CFunctionBase
{
public:
	CFunctionLight() {}
	virtual ~CFunctionLight() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;

private:
};

// 機能ボーンスケーリングクラス
class CFunctionBoneDispSize : public CFunctionBase
{
public:
	CFunctionBoneDispSize() {}
	virtual ~CFunctionBoneDispSize() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;

private:
};

// 機能ボーンスケーリングクラス
class CFunctionBoneMovementScale : public CFunctionBase
{
public:
	CFunctionBoneMovementScale() {}
	virtual ~CFunctionBoneMovementScale() {}

	void Init(void) override {}
	void Uninit(void) override { delete this; }
	void Update(void) override;

private:
};

#endif