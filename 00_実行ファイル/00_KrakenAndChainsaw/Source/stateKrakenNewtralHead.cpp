//**********************************************
// 
// タコ頭待機状態待機
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "stateKrakenNewtralHead.h"
#include "compMotionSkin.h"
#include "compPartMotion.h"
#include "compKrakenSensing.h"
#include "kraken.h"
#include "input.h"
#include "object.h"

//**********************************************
// コンストラクタ
//**********************************************
CStateKrakenNewtralHead::CStateKrakenNewtralHead()
{
	// タイプ設定
	SetType(CKraken::MOTION_NEWTRAL);
}

//**********************************************
// 初期化
//**********************************************
bool CStateKrakenNewtralHead::Init(void)
{
	// モーションスキンコンポーネント取得
	CPartMotion* pPartMotionHead = GetOwner()->GetComponent<CPartMotion>("PartMotionKrakenHead");

	// 待機モーションに設定
	pPartMotionHead->SetMotion(CKraken::MOTION_NEWTRAL, TIME_MOTIONBLEND);

	return true;
}

//**********************************************
// 更新
//**********************************************
void CStateKrakenNewtralHead::Update(void)
{
	
}

//**********************************************
// 描画
//**********************************************
void CStateKrakenNewtralHead::Uninit(void)
{
	delete this;
}