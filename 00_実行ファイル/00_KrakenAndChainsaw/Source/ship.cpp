//**************************************************
// 
// 船
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "ship.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "myMath.h"
#include "compShipVariable.h"
#include "compCollisionShapeMesh.h"
#include "compInfo.h"
#include "compMovement.h"
#include "compMotionSkin.h"
#include "compStateMachine.h"
#include "stateShipNewtral.h"
#include "stateShipMove.h"

//**************************************************
// 船デストラクタ
//**************************************************
CShip::CShip() : CObject(PRIORITY_3)
{

}

//**************************************************
// 船コンストラクタ
//**************************************************
CShip::~CShip()
{

}

//**************************************************
// 船初期化
//**************************************************
HRESULT CShip::Init(void)
{
	return S_OK;
}

//**************************************************
// 船初期化
//**************************************************
void CShip::Init(D3DXVECTOR3 pos)
{
	
}

//**************************************************
// 船終了
//**************************************************
void CShip::Uninit(void)
{
	// 自身の破棄
	Release();
}

//**************************************************
// 船更新
//**************************************************
void CShip::Update(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
}

//**************************************************
// 船描画
//**************************************************
void CShip::Draw(void)
{
	// コンポーネント描画
	DrawComponent();
}

//***************************************************************
// 船生成
//***************************************************************
CShip* CShip::Create(D3DXVECTOR3 pos)
{
	// 船インスタンス生成
	CShip* pShip = new CShip;

	// 初期化
	if (pShip != nullptr)
	{
		pShip->Init(pos);
	}

	return pShip;
}