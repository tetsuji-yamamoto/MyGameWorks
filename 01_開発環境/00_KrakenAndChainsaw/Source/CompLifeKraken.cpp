//***************************************************************
// 
// クラーケン体力処理
// Author Tetesuji Yamamoto
// 
//***************************************************************
#include "CompLifeKraken.h"
#include "kraken.h"
#include "manager.h"
#include "scene.h"
#include "compLife.h"
#include "compCollisionShapeMesh.h"
#include "compGauge.h"
#include "player.h"
#include "score.h"
#include "manager.h"
#include "scene.h"

//********************************************************
// コンストラクタ
//********************************************************
CCompLifeKraken::CCompLifeKraken()
{

}

//********************************************************
// デストラクタ
//********************************************************
CCompLifeKraken::~CCompLifeKraken()
{

}

//********************************************************
// 初期化
//********************************************************
void CCompLifeKraken::Init()
{

}

//********************************************************
// 終了
//********************************************************
void CCompLifeKraken::Uninit()
{
	// 自身の破棄
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CCompLifeKraken::Update()
{
	// マネージャ取得
	CManager* pManager = CManager::GetInstance();

	// シーン取得
	CScene* pScene = pManager->GetScene();

	// クラーケンー取得
	CKraken* pKraken = CManager::GetInstance()->GetScene()->GetObj<CKraken>("Kraken");

	// プレイヤー取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetObj<CPlayer>("Player");

	// スコア取得
	CScore* pScore = pScene->GetObj<CScore>("Score");

	//取得出来なければ処理しない
	if(pKraken	== nullptr)return;
	if(pPlayer	== nullptr)return;
	if(pScore	== nullptr)return;

	// 体力の取得
	CLife* pLife = pKraken->GetComponent<CLife>("KrakenLife");

	// ゲージ取得
	CGauge* pGauge = pKraken->GetComponent<CGauge>("GaugeLife");

	// 体の当たり判定取得
	CCollisionShapeMesh* pCollMesh = pKraken->GetComponent<CCollisionShapeMesh>("KrakenBody");

	// 取得出来ていなければ処理しない
	if(pLife		== nullptr)return;
	if(pGauge		== nullptr)return;
	if(pCollMesh	== nullptr)return;

	// ダメージフラグ
	bool bDamage = false;

	// チェーンソーに攻撃されたらダメージフラグを立てる
	if(pCollMesh->FindHitedName("AttackChainsaw00") == true)bDamage = true;
	if(pCollMesh->FindHitedName("AttackChainsaw01") == true)bDamage = true;
	if(pCollMesh->FindHitedName("AttackChainsaw02") == true)bDamage = true;
	if(pCollMesh->FindHitedName("AttackChainsaw03") == true)bDamage = true;
	if(pCollMesh->FindHitedName("AttackChainsaw04") == true)bDamage = true;

	// ダメージの発生
	if (bDamage == true)
	{
		// ダメージ取得
		int nDamage = pPlayer->GetAttackPower();

		// ダメージ追加
		pLife->AddLife(nDamage);
	}

	// ゲージ設定
	pGauge->SetNow(pLife->GetLife());

	// 体力がなくなったら
	if (pLife->GetIsMin() == true)
	{
		pKraken->SetIsDeath(true);
	}
}

//********************************************************
// 描画
//********************************************************
void CCompLifeKraken::Draw()
{

}