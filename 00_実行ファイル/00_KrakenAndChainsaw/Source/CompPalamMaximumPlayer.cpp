//***************************************************************
// 
// プレイヤー最高倍率計算処理
// Author Tetesuji Yamamoto
// 
//***************************************************************
#include "CompPalamMaximumPlayer.h"
#include "CompCounter.h"
#include "player.h"
#include "manager.h"
#include "scene.h"
#include "debugproc.h"

//********************************************************
// コンストラクタ
//********************************************************
CCompPalamMaximumPlayer::CCompPalamMaximumPlayer()
{

}

//********************************************************
// デストラクタ
//********************************************************
CCompPalamMaximumPlayer::~CCompPalamMaximumPlayer()
{

}

//********************************************************
// 初期化
//********************************************************
void CCompPalamMaximumPlayer::Init()
{

}

//********************************************************
// 終了
//********************************************************
void CCompPalamMaximumPlayer::Uninit()
{
	// 自身の破棄
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CCompPalamMaximumPlayer::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetObj<CPlayer>("Player");

	// 取得出来無ければ処理を切り上げる
	if (pPlayer == nullptr)return;

	// 攻撃カウンター取得
	CCounter* pCounterAttack = pPlayer->GetComponent<CCounter>("AttackCounter");

	// 取得出来無ければ処理を切り上げる
	if(pCounterAttack == nullptr)return;

	// タイムパラメータ取得
	float fPalamTime = pCounterAttack->GetPalam();

	// パラメータ設定
	pPlayer->SetPalamMaximum(fPalamTime);
}

//********************************************************
// 描画
//********************************************************
void CCompPalamMaximumPlayer::Draw()
{

}