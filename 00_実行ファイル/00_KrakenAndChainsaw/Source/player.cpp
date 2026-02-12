//**************************************************
// 
// プレイヤー
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "Player.h"
#include "debugproc.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "myMath.h"
#include "component.h"
#include "compCollisionShapeSpher.h"
#include "compCollisionCapsuleTrigger.h"
#include "compMovement.h"
#include "compCorrection.h"
#include "compStateMachine.h"
#include "compMotionSkin.h"
#include "compHitPlayer.h"
#include "compGauge.h"
#include "compDamage.h"
#include "statePlayerNewtral.h"
#include "statePlayerMove.h"
#include "statePlayerBlowoff.h"
#include "debugPoint.h"
#include "compWeapon.h"
#include "compHitBefore.h"
#include "compUpdateMotion.h"
#include "vector.h"
#include "debugCapsule.h"
#include "sound.h"

using namespace my_vector;

//**************************************************
// プレイヤーデストラクタ
//**************************************************
CPlayer::CPlayer() : CObject(PRIORITY_3)
{
	m_nLife				= 100;		// 体力
	m_bAttackedKraken	= false;	// クラーケンへの攻撃有無
	m_fPalamMaximum		= 0.0f;		// 最高倍率
	m_nAttackPower		= DAMAGE;	// 攻撃力
}

//**************************************************
// プレイヤーコンストラクタ
//**************************************************
CPlayer::~CPlayer()
{

}

//**************************************************
// プレイヤー初期化
//**************************************************
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//**************************************************
// プレイヤー初期化
//**************************************************
void CPlayer::Init(D3DXVECTOR3 pos)
{
	
}

//**************************************************
// プレイヤー終了
//**************************************************
void CPlayer::Uninit(void)
{
	// 自身の破棄
	Release();
}

//**************************************************
// プレイヤー更新
//**************************************************
void CPlayer::Update(void)
{

}

//**************************************************
// プレイヤー描画
//**************************************************
void CPlayer::Draw(void)
{
	DrawComponent();
}

//**************************************************
// プレイヤー訂正処理
//**************************************************
void CPlayer::Correction(void)
{
	GetComponent<CMovement>()->Correction();
}

//***************************************************************
// プレイヤー生成
//***************************************************************
CPlayer* CPlayer::Create(D3DXVECTOR3 pos)
{
	// プレイヤーインスタンス生成
	CPlayer* pPlayer = new CPlayer;

	// 初期化
	if (pPlayer != nullptr)
	{
		pPlayer->Init(pos);
	}

	return pPlayer;
}

//***************************************************************
// プレイヤー着地している場合の移動量処理
//***************************************************************
void CPlayer::SetMovementWhenLanding(void)
{
	// 情報取得
	CInfo* pInfo = GetComponent<CInfo>();

	// Yの移動量をなくす
	pInfo->move.y = 0.0f;
	pInfo->bRanding = true;
}

//***************************************************************
// プレイヤー着地していない場合の移動量処理
//***************************************************************
void CPlayer::SetMovementWhenNodLanding(void)
{
	// 情報取得
	CInfo* pInfo = GetComponent<CInfo>();

	pInfo->bRanding = false;
}

//***************************************************************
// プレイヤーの攻撃レベル取得
//***************************************************************
int CPlayer::GetAttacklevel(void)
{
	return 0;
}

//***************************************************************
// プレイヤーがタコに攻撃しているか否かを取得する処理
//***************************************************************
bool CPlayer::GetIsAttackKraken(void)
{
	// 当たり判定を取得
	CCollisionShapeSpherTrigger * pCollAtaack00 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw00");
	CCollisionShapeSpherTrigger * pCollAtaack01 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw01");
	CCollisionShapeSpherTrigger * pCollAtaack02 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw02");
	CCollisionShapeSpherTrigger * pCollAtaack03 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw03");
	CCollisionShapeSpherTrigger * pCollAtaack04 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw04");

	// 取得出来なかったら処理をやめる
	if (pCollAtaack00 == nullptr)return false;
	if (pCollAtaack01 == nullptr)return false;
	if (pCollAtaack02 == nullptr)return false;
	if (pCollAtaack03 == nullptr)return false;
	if (pCollAtaack04 == nullptr)return false;

	bool bAttakedKraken = false;

	// 攻撃がクラーケンに当たっていたらtrueを返す
	if (pCollAtaack00->FindHitedName("KrakenBody") == true)bAttakedKraken = true;
	if (pCollAtaack01->FindHitedName("KrakenBody") == true)bAttakedKraken = true;
	if (pCollAtaack02->FindHitedName("KrakenBody") == true)bAttakedKraken = true;
	if (pCollAtaack03->FindHitedName("KrakenBody") == true)bAttakedKraken = true;
	if (pCollAtaack04->FindHitedName("KrakenBody") == true)bAttakedKraken = true;

	if (bAttakedKraken == true)
	{
		return true;
	}

	// 結果を返す
	return false;
}

//***************************************************************
// プレイヤーがタコに攻撃しているか否かを取得する処理
//***************************************************************
bool CPlayer::GetIsAttackKrakenTentacle(void)
{
	// 当たり判定を取得
	CCollisionShapeSpherTrigger* pCollAtaack00 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw00");
	CCollisionShapeSpherTrigger* pCollAtaack01 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw01");
	CCollisionShapeSpherTrigger* pCollAtaack02 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw02");
	CCollisionShapeSpherTrigger* pCollAtaack03 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw03");
	CCollisionShapeSpherTrigger* pCollAtaack04 = GetComponent<CCollisionShapeSpherTrigger>("AttackChainsaw04");

	// 取得出来なかったら処理をやめる
	if (pCollAtaack00 == nullptr)return false;
	if (pCollAtaack01 == nullptr)return false;
	if (pCollAtaack02 == nullptr)return false;
	if (pCollAtaack03 == nullptr)return false;
	if (pCollAtaack04 == nullptr)return false;

	bool bAttakedKrakenTentacle = false;

	// 攻撃がクラーケンの触手に当たっていたらtrueを返す
	if (pCollAtaack00->FindHitedName("krakenTentacle00") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle01") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle02") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle03") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle04") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle05") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle06") == true)bAttakedKrakenTentacle = true;
	if (pCollAtaack00->FindHitedName("krakenTentacle07") == true)bAttakedKrakenTentacle = true;

	if (bAttakedKrakenTentacle == true)
	{
		return true;
	}

	// 結果を返す
	return false;
}