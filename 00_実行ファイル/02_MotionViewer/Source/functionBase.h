//**************************************************
// 
// 機能基底クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTIONBASE_H_
#define _FANCTIONBASE_H_

// 機能基底クラス
class CFunctionBase
{
public:
	CFunctionBase() {}
	virtual ~CFunctionBase() {}

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

private:
};

// 機能編集用基底クラス
class CFunctionEditBase
{
public:
	enum EDIT
	{
		EDIT_NONE,
		EDIT_MOTION,
		EDIT_PREVIEW
	};

	CFunctionEditBase() {}
	virtual ~CFunctionEditBase() {}

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

	virtual void CrickL(void) = 0;
	virtual void CrickR(void) = 0;
	virtual void CrickCenter(void) = 0;
	virtual void LShift(void) = 0;
	virtual void RShift(void) = 0;
	virtual void Return(void) = 0;
	virtual void Right(void) = 0;
	virtual void Left(void) = 0;
	virtual void Up(void) = 0;
	virtual void Down(void) = 0;
	virtual void F1(void) = 0;
	virtual void F2(void) = 0;
	virtual void F3(void) = 0;
	virtual void F4(void) = 0;
	virtual void F5(void) = 0;
	virtual void F6(void) = 0;
	virtual void F7(void) = 0;
	virtual void F8(void) = 0;
	virtual void F9(void) = 0;
	virtual void F10(void) = 0;
	virtual void F12(void) = 0;
	virtual void A(void) = 0;
	virtual void B(void) = 0;
	virtual void C(void) = 0;
	virtual void D(void) = 0;
	virtual void E(void) = 0;
	virtual void F(void) = 0;
	virtual void G(void) = 0;
	virtual void H(void) = 0;
	virtual void I(void) = 0;
	virtual void J(void) = 0;
	virtual void K(void) = 0;
	virtual void L(void) = 0;
	virtual void M(void) = 0;
	virtual void N(void) = 0;
	virtual void O(void) = 0;
	virtual void P(void) = 0;
	virtual void Q(void) = 0;
	virtual void R(void) = 0;
	virtual void S(void) = 0;
	virtual void T(void) = 0;
	virtual void U(void) = 0;
	virtual void V(void) = 0;
	virtual void W(void) = 0;
	virtual void X(void) = 0;
	virtual void Y(void) = 0;
	virtual void Z(void) = 0;

private:
};

#endif