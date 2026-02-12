//**********************************************
// 
// タコ触手状態待機コンポーネントクラス
// Author Tetsuji Yamamoto
//
//**********************************************
#include "CompStateTentacleNewtral.h"
#include "CompStateTentacleAttack.h"
#include "compPartMotion.h"
#include "compKrakenSensing.h"
#include "CompCounter.h"
#include "manager.h"
#include "player.h"
#include "kraken.h"

//********************************************************
// コンストラクタ
//********************************************************
CCompStateTentacleNewtral::CCompStateTentacleNewtral(Argument arg)
{
	m_Arg = arg;	// 引数情報保存
}

//********************************************************
// デストラクタ
//********************************************************
CCompStateTentacleNewtral::~CCompStateTentacleNewtral()
{

}

//********************************************************
// 初期化
//********************************************************
void CCompStateTentacleNewtral::Init(void)
{
	// オーナー取得
	CObject* pObject = GetOwner();

	// モーションスキンコンポーネント取得
	CPartMotion* pPartMotionTentacle00 = m_Arg.pPartMotion;

	// 待機モーションに設定
	pPartMotionTentacle00->SetMotion(CKraken::MOTION_NEWTRAL, m_Arg.nTimeMotionBlend);
}

//********************************************************
// 終了
//********************************************************
void CCompStateTentacleNewtral::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 感知コンポーネント取得
	CKrakenSensing* pkrakenSensing = pOwner->GetComponent<CKrakenSensing>();

	// アタックタイマー取得
	CCounter* pCounterAuto = m_Arg.pCounter;

	// プレイヤー取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetObj<CPlayer>("Player");

	// プレイヤー情報取得
	CInfo* pInfoPlayer = pPlayer->GetComponent<CInfo>();

	// 取得できてい無ければ処理しない
	if (pCounterAuto	== nullptr)return;
	if (pkrakenSensing	== nullptr)return;
	if (pPlayer			== nullptr)return;

	// プレイヤー位置取得
	D3DXVECTOR3 posPlayer = pInfoPlayer->pos;

	// 範囲にいるかを取得
	bool bRange = pkrakenSensing->GetIsRangePosition(posPlayer, m_Arg.fSensingHeight, m_Arg.fSensingAngle, m_Arg.fSensingAngleRange);

	// 検知範囲内でタイマーが満たされていれば
	if (bRange == true && pCounterAuto->GetIsMax() == true)
	{
		// 引数設定
		CCompStateTentacleAttack::Argument AtttackArg;
		AtttackArg.CapsuleColliderName = "CapsuleColliderTentacle" + m_Arg.TentacleNumber;
		AtttackArg.NewtralArg = m_Arg;

		// 名前作成
		std::string Name = "StateTentacleAttack" + m_Arg.TentacleNumber;

		// 攻撃コンポーネント追加
		pOwner->AddComponent<CCompStateTentacleAttack>(Name,AtttackArg);

		// カウンターのリセット
		pCounterAuto->ResetCount();

		// 自身を破棄
		pOwner->DeleteComponent<CCompStateTentacleNewtral>(GetName());

		// 処理を終了
		return;
	}
}

//********************************************************
// 更新
//********************************************************
void CCompStateTentacleNewtral::Uninit(void)
{
	// 自身の破棄
	delete this;
}
