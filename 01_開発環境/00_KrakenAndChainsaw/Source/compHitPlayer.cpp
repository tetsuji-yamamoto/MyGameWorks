//********************************************************
//
// プレイヤーヒット処理コンポーネントクラス
// Author Tetsuji Yamamoto
//
//********************************************************
#include "compHitPlayer.h"
#include "object.h"
#include "compInfo.h"
#include "compCollisionCapsuleTrigger.h"
#include "kraken.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "compStateMachine.h"
#include "statePlayerBlowoff.h"
#include "compLife.h"
#include "compGauge.h"

//********************************************************
// コンストラクタ
//********************************************************
CPlayerHit::CPlayerHit()
{

}

//********************************************************
// デストラクタ
//********************************************************
CPlayerHit::~CPlayerHit() {

}

//********************************************************
// 初期化
//********************************************************
void CPlayerHit::Init(void)
{

}

//********************************************************
// 終了
//********************************************************
void CPlayerHit::Uninit(void)
{
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CPlayerHit::Update(void)
{
	// マネージャ取得
	CManager* pManager = CManager::GetInstance();

	// シーン取得
	CScene* pScene = pManager->GetScene<CScene>();

	// 使用するオブジェクト取得
	CKraken* pKraken = pScene->GetObj<CKraken>("Kraken");
	CPlayer* pPlayer = pScene->GetObj<CPlayer>("Player");

	// 取得出来ていなければ処理しない
	if (pKraken == nullptr)return;
	if (pPlayer == nullptr)return;

	// カプセルコライダー取得
	CCollisionCapsuleTrigger* pCollCapsuleTrii = pPlayer->GetComponent<CCollisionCapsuleTrigger>(std::string("PlayerCapsule"));
	
	// 体力取得
	CLife* pLife = pPlayer->GetComponent<CLife>(std::string("PlayerLife"));

	// ゲージ取得
	CGauge* pGauge = pPlayer->GetComponent<CGauge>(std::string("PlayerGaugeLife"));
	
	// 取得出来ていなければ処理しない
	if (pCollCapsuleTrii	== nullptr)return;
	if (pLife				== nullptr)return;

	// ダメージか否か
	bool bDamage = false;

	// タコの攻撃に当たった
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle00")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle01")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle02")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle03")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle04")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle05")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle06")) == true)bDamage = true;
	if(pCollCapsuleTrii->FindHitedName(std::string("CapsuleColliderTentacle07")) == true)bDamage = true;
	
	// ダメージを受けている
	if (bDamage == true)
	{
		// 吹っ飛ばし状態にする
		pPlayer->GetComponent<CStateMachine>()->ChangeState(new CStatePlayerBlowoff);

		// 体力を減らす
		pLife->AddLife(CKraken::ATTACK_POWER_TENTACLE);

		// ゲージの長さ設定
		pGauge->SetNow(pLife->GetLife());
	}
}