//***************************************************************
// 
// プレイヤースコア加算処理
// Author Tetesuji Yamamoto
// 
//***************************************************************
#include "CompAddScorePlayer.h"
#include "player.h"
#include "kraken.h"
#include "scene.h"
#include "score.h"
#include "manager.h"
#include "game.h"

//********************************************************
// コンストラクタ
//********************************************************
CCompAddScorePlayer::CCompAddScorePlayer()
{

}

//********************************************************
// デストラクタ
//********************************************************
CCompAddScorePlayer::~CCompAddScorePlayer()
{

}

//********************************************************
// 初期化
//********************************************************
void CCompAddScorePlayer::Init()
{

}

//********************************************************
// 終了
//********************************************************
void CCompAddScorePlayer::Uninit()
{
	// 自身の破棄
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CCompAddScorePlayer::Update()
{
	// マネージャ取得
	CManager* pManager = CManager::GetInstance();

	// シーン取得
	CGame* pGame = pManager->GetScene<CGame>();

	// 使用するオブジェクト取得
	CKraken*	pKraken	= pGame->GetObj<CKraken>	("Kraken");
	CPlayer*	pPlayer	= pGame->GetObj<CPlayer>	("Player");
	CScore*		pScore	= pGame->GetObj<CScore>		("Score");

	// 取得出来ていなければ処理しない
	if(pKraken	== nullptr)return;
	if(pPlayer	== nullptr)return;
	if(pScore	== nullptr)return;

	// 体力が無ければ処理しない
	if (pKraken->GetIsDeath() == true)
	{
	return;
	}

	// クラーケンに攻撃したか否かを取得
	bool bAttakedKraken			= pPlayer->GetIsAttackKraken();
	bool bAttakedkrakenTentacle	= pPlayer->GetIsAttackKrakenTentacle();

	// クラーケンに攻撃していた
	if (bAttakedKraken			== true && 
		bAttakedkrakenTentacle	== false)
	{
		// スコア加算A
		pScore->AddScore(pGame->GetScoreAddPointA());
	}
	// クラーケンの触手に攻撃していた
	else if (	bAttakedKraken			== true &&
				bAttakedkrakenTentacle	== true)
	{
		// スコア加算B
		pScore->AddScore(pGame->GetScoreAddPointB());
	}
}

//********************************************************
// 描画
//********************************************************
void CCompAddScorePlayer::Draw()
{

}