//**************************************************
// 
// 機能編集用クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTIONEDIT_H_
#define _FANCTIONEDIT_H_

#include "functionBase.h"

// 機能パーツ編集クラス
class CFunctionEditPrts : public CFunctionEditBase
{
public:
	CFunctionEditPrts();
	virtual ~CFunctionEditPrts();

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;

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
	void F1(void) override {}
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

#endif