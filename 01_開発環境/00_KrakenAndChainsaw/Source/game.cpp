//*******************************************************
// 
// ゲームシーンの処理
// Author Tetsuji Yamamoto
// 
//*******************************************************
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "timer.h"
#include "score.h"
#include "input.h"
#include "result.h"
#include "spherSky.h"
#include "camera.h"
#include "player.h"
#include "ship.h"
#include "myMath.h"
#include "kraken.h"
#include "shapeCapsule.h"
#include "shapeSylinder.h"
#include "shapeSpher.h"
#include "sound.h"
#include "ocean.h"
#include "vector.h"
#include "matrix.h"
#include "compCollisionShapeSpher.h"
#include "compCollisionCapsuleTrigger.h"
#include "compMovement.h"
#include "compCorrection.h"
#include "compStateMachine.h"
#include "compMotionSkin.h"
#include "compHitPlayer.h"
#include "compGauge.h"
#include "compDamage.h"
#include "compWeapon.h"
#include "compHitBefore.h"
#include "compUpdateMotion.h"
#include "compModel.h"
#include "compCollisionShapeMesh.h"
#include "compKrakenSensing.h"
#include "compPartMotion.h"
#include "compLife.h"
#include "CompCounter.h"
#include "CompStateTentacleNewtral.h"
#include "CompStateTentacleAttack.h"
#include "CompAddScorePlayer.h"
#include "CompPalamMaximumPlayer.h"
#include "CompLifeKraken.h"
#include "compHitPlayer.h"
#include "CompPolygon2D.h"
#include "statePlayerNewtral.h"
#include "stateKrakenNewtralHead.h"
#include "MoveGuideUI.h"

using namespace my_vector;
using namespace std;
using namespace nlohmann;

//*******************************************************
// ゲームシーンコンストラクタ
//*******************************************************
CGame::CGame()
{

}

//*******************************************************
// ゲームシーンデストラクタ
//*******************************************************
CGame::~CGame()
{

}

//*******************************************************
// ゲームシーン初期化
//*******************************************************
HRESULT CGame::Init(void)
{
	return S_OK;
}

//*******************************************************
// ゲームシーン初期化
//*******************************************************
void CGame::Init(MODE mode)
{
	// 親クラス初期化
	CScene::Init(mode);

	// 情報読み取り
	ReadInfo();

	CreateSky		();	// 空生成
	CreateOcean		();	// 海生成
	CreateShip		();	// 船生成
	CreatePlayer	();	// プレイヤー生成
	CreateKraken	();	// クラーケン生成
	CreateScore		();	// スコア生成
	CreateTimer		();	// タイマー生成

	// 移動案内生成
	CMoveGuide::Create();

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラの回転取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// プレイヤー取得
	CPlayer * pPlayer = GetObj<CPlayer>("Player");

	// 情報取得
	CInfo* pInfo = pPlayer->GetComponent<CInfo>();

	float fAngleCamera = AngleAtan2f(D3DXVECTOR2(pInfo->pos.x, pInfo->pos.z), D3DXVECTOR2(0.0f, 0.0f));
	fAngleCamera = AngleNormalize(fAngleCamera);

	// カメラのセット
	CameraSet();

	// 波音を流す
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_WAVE);

	// チェンソーアイドリングオン
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_CHAINSAW_ENGIN_IDLING);
}

//*******************************************************
// ゲームシーン終了
//*******************************************************
void CGame::Uninit(void)
{
	// チェンソー切る音ストップ
	CManager::GetInstance()->GetSound()->StopSound();

	// エンジンストップ音
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHAINSAW_ENGIN_STOP);

	// オブジェクトの全破棄
	OllDeleteObjs();

	// 自身の破棄
	CScene::Uninit();
}

//*******************************************************
// ゲームシーン更新
//*******************************************************
void CGame::Update(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInstance()->GetInputKeyboard();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// フェード移行状態であればこの先は処理しない
	if (CManager::GetInstance()->GetFade() != CFade::FADE_NONE)
	{
		return;
	}

	// オブジェクト取得
	CKraken*	pKraken = GetObj	<CKraken>	("Kraken");
	CPlayer*	pPlayer	= GetObj	<CPlayer>	("Player");
	CTimer*		pTimer	= GetObj	<CTimer>	("Timer");
	CScore*		pScore	= GetObj	<CScore>	("Score");

	// タコが死んでいたら
	if (pKraken->GetIsDeath() == true)
	{
		// フェードをセットする
		CManager::GetInstance()->SetFade(CScene::MODE_RESULT_GAMECLEAR);
	}

	// プレイヤーが死んでいたら
	if (pPlayer->GetComponent<CLife>("PlayerLife")->GetIsMin() == true)
	{
		// フェードをセットする
		CManager::GetInstance()->SetFade(CScene::MODE_RESULT_GAMEOVER);
	}

	// 時間切れであればリザルトへ遷移
	if (pTimer != nullptr)
	{
		// 時間切れであった
		if (pTimer->GetIsTimeOut() == true)
		{
			// フェードをセットする
			CManager::GetInstance()->SetFade(CScene::MODE_RESULT_GAMEOVER);
		}
	}

	// カメラの設定
	CameraSet();
}

//*******************************************************
// ゲームシーン描画
//*******************************************************
void CGame::Draw(void) 
{
}

//************************************************************************
// 情報の読込
//************************************************************************
void CGame::ReadInfo(void)
{
	// ファイルを読込
	ifstream ifs("data\\json\\game.json");

	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "CGame::ReadInfo() > ファイルが開けませんでした", "Error", MB_OK);
		return;
	}

	// ジェイソンオブジェクト生成
	json json;

	// jsonの情報を読み込む
	ifs >> json;

	// 情報を移し替える
	m_info.MAXTIME			= json["MAXTIME"];			// タイマー
	m_info.CAMERA_DISTANCE	= json["CAMERA_DISTANCE"];	// カメラ距離
	m_info.CAMERA_HEIGHT	= json["CAMERA_HEIGHT"];	// カメラ高さ
	m_info.CAMERA_DIPS		= json["CAMERA_DIPS"];		// カメラ奥行き
	m_info.CAMERA_ROT_X		= json["CAMERA_ROT_X"];		// カメラ角度X
	m_info.SCORE_WIDTH		= json["SCORE_WIDTH"];		// スコア幅
	m_info.SCORE_HEIGHT		= json["SCORE_HEIGHT"];		// スコア高さ
	m_info.SCORE_ADDPOINT_A	= json["SCORE_ADDPOINT_A"];	// スコア加算値
	m_info.SCORE_ADDPOINT_B	= json["SCORE_ADDPOINT_B"];	// スコア加算値
	m_info.SCORE_ADDPOINT_C	= json["SCORE_ADDPOINT_C"];	// スコア加算値

	// ファイルを閉じる
	ifs.close();
}

//************************************************************************
// 空の生成
//************************************************************************
void CGame::CreateSky(void)
{
	// 空生成
	CSphereSky* pSky = CSphereSky::Create(20, 20, 20000.0f);

	// ヴェクターに追加
	AddObj(pSky);
}

//************************************************************************
// 海の生成
//************************************************************************
void CGame::CreateOcean(void)
{
	// 海生成
	COcean* pOcean = COcean::Create();

	// ヴェクターに追加
	AddObj(pOcean);
}

//************************************************************************
// 船の生成
//************************************************************************
void CGame::CreateShip(void)
{
	// 船生成
	CShip* pShip = CShip::Create(InitValu::VEC3_NULL);

	// コンポーネントの設定
	CInfo*		pInfo			= pShip->AddComponent	<CInfo>		();																				// 情報
	CModel*		pModelShip		= pShip->AddComponent	<CModel>	(string("Ship"),			"data\\model\\ship\\ship_001.x");					// 船モデル
	CModel*		pModelShipColl	= pShip->AddComponent	<CModel>	(string("CollisonModel"),	"data\\model\\ship\\ship_collision_001.x",false);	// 船当たり判定モデル
	CMovement*	pMovement		= pShip->AddComponent	<CMovement>	();																				// 移動更新処理

	// メッシュの取得
	LPD3DXMESH pMesh = pModelShipColl->GetMesh();

	// 当たり判定の設定
	pShip->AddComponent	<CCollisionShapeMesh>	(string("MeshClider"), pMesh,&pInfo->mtxWorld);	// 船当たり判定モデル
	
	// 位置設定
	pInfo->pos = D3DXVECTOR3(0.0f, 0.0f, 4000.0f);

	// ヴェクターに追加
	AddObj(pShip);
}

//************************************************************************
// プレイヤーの生成
//************************************************************************
void CGame::CreatePlayer(void)
{
	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create(InitValu::VEC3_NULL);
	
	// マネージャ取得
	CManager*pManager = CManager::GetInstance();

	// 名前設定
	pPlayer->SetName("Player");

	float fMovement = pManager->GetMovementPlayer();
	float fFriction = pManager->GetFrictionPlayer();
	float fGravity = pManager->GetGravityPlayer();

	// 体力引数
	CLife::LifeArgument ArguLife;
	ArguLife.nLife	= CPlayer::LIFE;
	ArguLife.nMax	= CPlayer::LIFE;
	ArguLife.nMin	= 0;

	// ポリゴン2D用変数
	CCompPolygon2D::Argment Polygon2DArg;
	Polygon2DArg.col = Color::_WHITE;
	Polygon2DArg.fHeight = 20.0f;
	Polygon2DArg.fWidth = 530.0f;
	Polygon2DArg.pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.9f, 0.0f);
	Polygon2DArg.rot = InitValu::VEC3_NULL;
	Polygon2DArg.TexturePath = "data\\texture\\Bar_001.png";

	pPlayer->AddComponent<CInfo>					();																																		// 情報
	pPlayer->AddComponent<CLife>					(string("PlayerLife"), ArguLife);																										// 体力
	//pPlayer->AddComponent<CCompPolygon2D>			(Polygon2DArg);																															// ゲージ裏
	pPlayer->AddComponent<CGauge>					(string("PlayerGaugeLife"), ArguLife.nMax,D3DXVECTOR3(0.0f,SCREEN_HEIGHT * 0.9f,0.0f),InitValu::VEC3_NULL, Color::_GREEN,500.0f,10.0f);	// 体力ゲージ
	pPlayer->AddComponent<CCompAddScorePlayer>		();																																		// スコア加算処理
	pPlayer->AddComponent<CStateMachine>			();																																		// ステートマシーン
	pPlayer->AddComponent<CMovement>				(CComponent::PRIORITY_01);																												// 移動処理
	pPlayer->AddComponent<CMotionSkin>				(CComponent::PRIORITY_MOTION_01,CManager::GetInstance()->GetFilePathMotionPlayer().c_str());											// モーション情報
	pPlayer->AddComponent<CUpdateMotion>			(CComponent::PRIORITY_MOTION_02);																										// モーション更新
	pPlayer->AddComponent<CHitBefore>				(CComponent::PRIORITY_HITBEFORE);																										// 当たり判定前
	pPlayer->AddComponent<CCollisionShapeSpher>		(CComponent::PRIORITY_COLLISION, std::string("PlayerSpher"),D3DXVECTOR3(0.0f, 60.0f, 0.0f),70.0f, D3DX_PI * 0.7f);						// スフィアコライダー
	pPlayer->AddComponent<CCollisionCapsuleTrigger>	(CComponent::PRIORITY_COLLISION,std::string("PlayerCapsule"), D3DXVECTOR3(0.0f, 50.0f, 0.0f), InitValu::QUAT_NULL, 100.0f, 50.0f);		// カプセルコライダー
	pPlayer->AddComponent<CPlayerHit>				(CComponent::PRIORITY_CORRECTION);																										// 当たった時の処理
	pPlayer->AddComponent<CCorrection>				(CComponent::PRIORITY_CORRECTION);																										// 修正
	pPlayer->AddComponent<CCounter>					(std::string("AttackCounter"), CPlayer::TIME_LARGE,false);																				// 攻撃カウンター
	pPlayer->AddComponent<CCompPalamMaximumPlayer>	(std::string("PalamMaximum"));																											// 最高倍率

	// 情報取得
	CInfo* pInfo = pPlayer->GetComponent<CInfo>();
	pInfo->pos = D3DXVECTOR3(0.0f,200.0f,3900.0f);
	pInfo->fMovement = fMovement;
	pInfo->fFriction = fFriction;
	pInfo->fGravity = fGravity;

	// 待機状態に設定
	pPlayer->GetComponent<CStateMachine>()->ChangeState(new CStatePllayerNewtral);

	// チェンソーアイドリングオン
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_CHAINSAW_ENGIN_IDLING);

	// ヴェクターに追加
	AddObj(pPlayer);
}

//************************************************************************
// クラーケンの生成
//************************************************************************
void CGame::CreateKraken(void)
{
	float fMovement = 0.0f;
	float fFriction = 0.0f;
	float fGravity = 0.0f;

	CLife::LifeArgument arguLifeKraken;
	CLife::LifeArgument arguLifeTentacle;
	arguLifeKraken.nLife = CKraken::LIFE_KRAKEN;
	arguLifeKraken.nMax = CKraken::LIFE_KRAKEN;
	arguLifeTentacle.nLife = CKraken::LIFE_TENTACLE;
	arguLifeTentacle.nMax = CKraken::LIFE_TENTACLE;

	// ポリゴン2D用変数
	CCompPolygon2D::Argment Polygon2DArg;
	Polygon2DArg.col = Color::_BLACK;
	Polygon2DArg.fHeight = 20.0f;
	Polygon2DArg.fWidth = SCREEN_WIDTH * 0.3f;
	Polygon2DArg.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.85, 0.0f);
	Polygon2DArg.rot = InitValu::VEC3_NULL;
	Polygon2DArg.TexturePath = "data\\texture\\Bar_002.png";

	// プレイヤー取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetObj<CPlayer>();

	// クラーケン生成
	CKraken* pKraken = CKraken::Create(InitValu::VEC3_NULL);

	// 名前設定
	pKraken->SetName("Kraken");

	pKraken->AddComponent<CInfo>				();																												// 情報
	pKraken->AddComponent<CKrakenSensing>		();																												// 感知処理
	pKraken->AddComponent<CMovement>			();																												// 移動処理
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLife"), arguLifeKraken);													// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle00"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle01"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle02"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle03"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle04"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle05"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle06"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CLife>				(CComponent::PRIORITY_00,string("KrakenLifeTentacle07"),arguLifeTentacle);										// 体力
	pKraken->AddComponent<CCompLifeKraken>		(CComponent::PRIORITY_00,string("KrakenLifeManager"));															// 体力管理
	pKraken->AddComponent<CDamage>				();																												// ダメージ
	//pKraken->AddComponent<CCompPolygon2D>		(Polygon2DArg);																									// ゲージ裏
	pKraken->AddComponent<CGauge>				(CComponent::PRIORITY_03,string("GaugeLife"), CKraken::LIFE_KRAKEN, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85, 0.0f), InitValu::VEC3_NULL, Color::_BLUE, SCREEN_WIDTH * 0.5f, 10.0f);			// 体力ゲージ
	pKraken->AddComponent<CStateMachine>		(CComponent::PRIORITY_01, std::string("stateMachineKrakenHead"));												// ステートマシン頭

	CCounter* pCounterTentacle00 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor00"), CKraken::COUNTER_TENTACLE);	// 触手カウンター00
	CCounter* pCounterTentacle01 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor01"), CKraken::COUNTER_TENTACLE);	// 触手カウンター01
	CCounter* pCounterTentacle02 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor02"), CKraken::COUNTER_TENTACLE);	// 触手カウンター02
	CCounter* pCounterTentacle03 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor03"), CKraken::COUNTER_TENTACLE);	// 触手カウンター03
	CCounter* pCounterTentacle04 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor04"), CKraken::COUNTER_TENTACLE);	// 触手カウンター04
	CCounter* pCounterTentacle05 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor05"), CKraken::COUNTER_TENTACLE);	// 触手カウンター05
	CCounter* pCounterTentacle06 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor06"), CKraken::COUNTER_TENTACLE);	// 触手カウンター06
	CCounter* pCounterTentacle07 = pKraken->AddComponent<CCounter>(string("TentacleCountAutor07"), CKraken::COUNTER_TENTACLE);	// 触手カウンター07

	CPartMotion* pPartMotionHead	= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenHead"));			// 頭のモーション担当
	CPartMotion* pPartMotion00		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle00"));	// 触手のモーション担当00
	CPartMotion* pPartMotion01		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle01"));	// 触手のモーション担当01
	CPartMotion* pPartMotion02		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle02"));	// 触手のモーション担当02
	CPartMotion* pPartMotion03		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle03"));	// 触手のモーション担当03
	CPartMotion* pPartMotion04		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle04"));	// 触手のモーション担当04
	CPartMotion* pPartMotion05		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle05"));	// 触手のモーション担当05
	CPartMotion* pPartMotion06		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle06"));	// 触手のモーション担当06
	CPartMotion* pPartMotion07		= pKraken->AddComponent<CPartMotion>(CComponent::PRIORITY_MOTION_00, std::string("PartMotionKrakenTentacle07"));	// 触手のモーション担当07

	pKraken->AddComponent<CMotionSkin>			(CComponent::PRIORITY_MOTION_04,CManager::GetInstance()->GetFilePathMotionKraken().c_str());					// モーション情報
	pKraken->AddComponent<CCollisionShapeMesh>	(CComponent::PRIORITY_COLLISION, std::string("KrakenBody"), pKraken->GetComponent<CMotionSkin>()->GetMesh());	// メッシュコライダー
	
	// 頭ボーン設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_HEAD);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_HEAD_JOINT);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_ABDOMEN_LOWER);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_ABDOMEN);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_EYE_RIGHT);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenHead")->PushBackBoneIdx(CKraken::BONE_EYE_LEFT);

	// 触手ボーン00設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle00")->PushBackBoneIdx(CKraken::BONE_TENTACLE_00_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle00")->PushBackBoneIdx(CKraken::BONE_TENTACLE_00_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle00")->PushBackBoneIdx(CKraken::BONE_TENTACLE_00_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle00")->PushBackBoneIdx(CKraken::BONE_TENTACLE_00_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle00")->PushBackBoneIdx(CKraken::BONE_TENTACLE_00_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle00")->PushBackBoneIdx(CKraken::BONE_TENTACLE_00_05);

	// 触手ボーン01設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle01")->PushBackBoneIdx(CKraken::BONE_TENTACLE_01_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle01")->PushBackBoneIdx(CKraken::BONE_TENTACLE_01_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle01")->PushBackBoneIdx(CKraken::BONE_TENTACLE_01_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle01")->PushBackBoneIdx(CKraken::BONE_TENTACLE_01_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle01")->PushBackBoneIdx(CKraken::BONE_TENTACLE_01_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle01")->PushBackBoneIdx(CKraken::BONE_TENTACLE_01_05);

	// 触手ボーン02設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle02")->PushBackBoneIdx(CKraken::BONE_TENTACLE_02_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle02")->PushBackBoneIdx(CKraken::BONE_TENTACLE_02_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle02")->PushBackBoneIdx(CKraken::BONE_TENTACLE_02_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle02")->PushBackBoneIdx(CKraken::BONE_TENTACLE_02_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle02")->PushBackBoneIdx(CKraken::BONE_TENTACLE_02_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle02")->PushBackBoneIdx(CKraken::BONE_TENTACLE_02_05);

	// 触手ボーン03設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle03")->PushBackBoneIdx(CKraken::BONE_TENTACLE_03_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle03")->PushBackBoneIdx(CKraken::BONE_TENTACLE_03_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle03")->PushBackBoneIdx(CKraken::BONE_TENTACLE_03_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle03")->PushBackBoneIdx(CKraken::BONE_TENTACLE_03_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle03")->PushBackBoneIdx(CKraken::BONE_TENTACLE_03_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle03")->PushBackBoneIdx(CKraken::BONE_TENTACLE_03_05);

	// 触手ボーン04設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle04")->PushBackBoneIdx(CKraken::BONE_TENTACLE_04_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle04")->PushBackBoneIdx(CKraken::BONE_TENTACLE_04_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle04")->PushBackBoneIdx(CKraken::BONE_TENTACLE_04_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle04")->PushBackBoneIdx(CKraken::BONE_TENTACLE_04_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle04")->PushBackBoneIdx(CKraken::BONE_TENTACLE_04_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle04")->PushBackBoneIdx(CKraken::BONE_TENTACLE_04_05);

	// 触手ボーン05設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle05")->PushBackBoneIdx(CKraken::BONE_TENTACLE_05_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle05")->PushBackBoneIdx(CKraken::BONE_TENTACLE_05_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle05")->PushBackBoneIdx(CKraken::BONE_TENTACLE_05_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle05")->PushBackBoneIdx(CKraken::BONE_TENTACLE_05_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle05")->PushBackBoneIdx(CKraken::BONE_TENTACLE_05_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle05")->PushBackBoneIdx(CKraken::BONE_TENTACLE_05_05);

	// 触手ボーン06設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle06")->PushBackBoneIdx(CKraken::BONE_TENTACLE_06_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle06")->PushBackBoneIdx(CKraken::BONE_TENTACLE_06_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle06")->PushBackBoneIdx(CKraken::BONE_TENTACLE_06_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle06")->PushBackBoneIdx(CKraken::BONE_TENTACLE_06_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle06")->PushBackBoneIdx(CKraken::BONE_TENTACLE_06_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle06")->PushBackBoneIdx(CKraken::BONE_TENTACLE_06_05);

	// 触手ボーン07設定
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle07")->PushBackBoneIdx(CKraken::BONE_TENTACLE_07_00);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle07")->PushBackBoneIdx(CKraken::BONE_TENTACLE_07_01);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle07")->PushBackBoneIdx(CKraken::BONE_TENTACLE_07_02);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle07")->PushBackBoneIdx(CKraken::BONE_TENTACLE_07_03);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle07")->PushBackBoneIdx(CKraken::BONE_TENTACLE_07_04);
	pKraken->GetComponent<CPartMotion>("PartMotionKrakenTentacle07")->PushBackBoneIdx(CKraken::BONE_TENTACLE_07_05);

	// すべて待機状態に設定
	pKraken->GetComponent<CStateMachine>("stateMachineKrakenHead")->ChangeState(new CStateKrakenNewtralHead);

	// 触手への当たり判定追加
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_00_02, "krakenTentacle00");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_01_02, "krakenTentacle01");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_02_02, "krakenTentacle02");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_03_02, "krakenTentacle03");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_04_02, "krakenTentacle04");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_05_02, "krakenTentacle05");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_06_02, "krakenTentacle06");
	pKraken->SetCollisionCapsuleTriggerTentacle(CKraken::BONE_TENTACLE_07_02, "krakenTentacle07");

	CCompStateTentacleNewtral::Argument NewtralArg00;
	CCompStateTentacleNewtral::Argument NewtralArg01;
	CCompStateTentacleNewtral::Argument NewtralArg02;
	CCompStateTentacleNewtral::Argument NewtralArg03;
	CCompStateTentacleNewtral::Argument NewtralArg04;
	CCompStateTentacleNewtral::Argument NewtralArg05;
	CCompStateTentacleNewtral::Argument NewtralArg06;
	CCompStateTentacleNewtral::Argument NewtralArg07;
	NewtralArg00.pPartMotion			= pPartMotion00;					// 特定部位のモーション制御ポインタ
	NewtralArg00.pCounter				= pCounterTentacle00;				// カウンターポインタ
	NewtralArg00.TentacleNumber			= "00";								// 触手番号
	NewtralArg00.Name					= "StateTentacleNewtral00";			// 名前
	NewtralArg00.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg00.fSensingHeight			= CKraken::SENSING_00_HEIGHT;		// 検知高さ
	NewtralArg00.fSensingAngleRange		= CKraken::SENSING_00_ANGLERANGE;	// 検知範囲
	NewtralArg00.fSensingAngle			= CKraken::SENSING_00_ANGLE;		// 検知向き

	NewtralArg01.pPartMotion			= pPartMotion01;					// 特定部位のモーション制御ポインタ
	NewtralArg01.pCounter				= pCounterTentacle01;				// カウンターポインタ
	NewtralArg01.TentacleNumber			= "01";								// 触手番号
	NewtralArg01.Name					= "StateTentacleNewtral01";			// 名前
	NewtralArg01.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg01.fSensingHeight			= CKraken::SENSING_01_HEIGHT;		// 検知高さ
	NewtralArg01.fSensingAngleRange		= CKraken::SENSING_01_ANGLERANGE;	// 検知範囲
	NewtralArg01.fSensingAngle			= CKraken::SENSING_01_ANGLE;		// 検知向き

	NewtralArg02.pPartMotion			= pPartMotion02;					// 特定部位のモーション制御ポインタ
	NewtralArg02.pCounter				= pCounterTentacle02;				// カウンターポインタ
	NewtralArg02.TentacleNumber			= "02";								// 触手番号
	NewtralArg02.Name					= "StateTentacleNewtral02";			// 名前
	NewtralArg02.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg02.fSensingHeight			= CKraken::SENSING_02_HEIGHT;		// 検知高さ
	NewtralArg02.fSensingAngleRange		= CKraken::SENSING_02_ANGLERANGE;	// 検知範囲
	NewtralArg02.fSensingAngle			= CKraken::SENSING_02_ANGLE;		// 検知向き

	NewtralArg03.pPartMotion			= pPartMotion03;					// 特定部位のモーション制御ポインタ
	NewtralArg03.pCounter				= pCounterTentacle03;				// カウンターポインタ
	NewtralArg03.TentacleNumber			= "03";								// 触手番号
	NewtralArg03.Name					= "StateTentacleNewtral03";			// 名前
	NewtralArg03.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg03.fSensingHeight			= CKraken::SENSING_03_HEIGHT;		// 検知高さ
	NewtralArg03.fSensingAngleRange		= CKraken::SENSING_03_ANGLERANGE;	// 検知範囲
	NewtralArg03.fSensingAngle			= CKraken::SENSING_03_ANGLE;		// 検知向き

	NewtralArg04.pPartMotion			= pPartMotion04;					// 特定部位のモーション制御ポインタ
	NewtralArg04.pCounter				= pCounterTentacle04;				// カウンターポインタ
	NewtralArg04.TentacleNumber			= "04";								// 触手番号
	NewtralArg04.Name					= "StateTentacleNewtral04";			// 名前
	NewtralArg04.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg04.fSensingHeight			= CKraken::SENSING_04_HEIGHT;		// 検知高さ
	NewtralArg04.fSensingAngleRange		= CKraken::SENSING_04_ANGLERANGE;	// 検知範囲
	NewtralArg04.fSensingAngle			= CKraken::SENSING_04_ANGLE;		// 検知向き

	NewtralArg05.pPartMotion			= pPartMotion05;					// 特定部位のモーション制御ポインタ
	NewtralArg05.pCounter				= pCounterTentacle05;				// カウンターポインタ
	NewtralArg05.TentacleNumber			= "05";								// 触手番号
	NewtralArg05.Name					= "StateTentacleNewtral05";			// 名前
	NewtralArg05.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg05.fSensingHeight			= CKraken::SENSING_05_HEIGHT;		// 検知高さ
	NewtralArg05.fSensingAngleRange		= CKraken::SENSING_05_ANGLERANGE;	// 検知範囲
	NewtralArg05.fSensingAngle			= CKraken::SENSING_05_ANGLE;		// 検知向き

	NewtralArg06.pPartMotion			= pPartMotion06;					// 特定部位のモーション制御ポインタ
	NewtralArg06.pCounter				= pCounterTentacle06;				// カウンターポインタ
	NewtralArg06.TentacleNumber			= "06";								// 触手番号
	NewtralArg06.Name					= "StateTentacleNewtral06";			// 名前
	NewtralArg06.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg06.fSensingHeight			= CKraken::SENSING_06_HEIGHT;		// 検知高さ
	NewtralArg06.fSensingAngleRange		= CKraken::SENSING_06_ANGLERANGE;	// 検知範囲
	NewtralArg06.fSensingAngle			= CKraken::SENSING_06_ANGLE;		// 検知向き

	NewtralArg07.pPartMotion			= pPartMotion07;					// 特定部位のモーション制御ポインタ
	NewtralArg07.pCounter				= pCounterTentacle07;				// カウンターポインタ
	NewtralArg07.TentacleNumber			= "07";								// 触手番号
	NewtralArg07.Name					= "StateTentacleNewtral07";			// 名前
	NewtralArg07.nTimeMotionBlend		= CKraken::TIME_MOTIONBLEND;		// モーションをブレンドする時間
	NewtralArg07.fSensingHeight			= CKraken::SENSING_07_HEIGHT;		// 検知高さ
	NewtralArg07.fSensingAngleRange		= CKraken::SENSING_07_ANGLERANGE;	// 検知範囲
	NewtralArg07.fSensingAngle			= CKraken::SENSING_07_ANGLE;		// 検知向き

	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg00.Name, NewtralArg00);	// 待機状態触手00
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg01.Name, NewtralArg01);	// 待機状態触手01
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg02.Name, NewtralArg02);	// 待機状態触手02
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg03.Name, NewtralArg03);	// 待機状態触手03
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg04.Name, NewtralArg04);	// 待機状態触手04
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg05.Name, NewtralArg05);	// 待機状態触手05
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg06.Name, NewtralArg06);	// 待機状態触手06
	pKraken->AddComponent<CCompStateTentacleNewtral>(NewtralArg07.Name, NewtralArg07);	// 待機状態触手07
	
	// ヴェクターに追加
	AddObj(pKraken);
}

//************************************************************************
// タイマーの生成
//************************************************************************
void CGame::CreateTimer(void)
{
	// タイマー生成
	CTimer* pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH* 0.5f,SCREEN_HEIGHT * 0.05f,0.0f),Color::_WHITE,100.0f,40.0f);

	// 名前設定
	pTimer->SetName("Timer");

	// ヴェクターに追加
	AddObj(pTimer);
}

//************************************************************************
// スコアの生成
//************************************************************************
void CGame::CreateScore(void)
{
	// スコア生成
	CScore* pScore = CScore::Create(D3DXVECTOR3(m_info.SCORE_WIDTH,SCREEN_HEIGHT - m_info.SCORE_HEIGHT,0.0f), Color::_WHITE, m_info.SCORE_WIDTH, m_info.SCORE_HEIGHT);
	
	// 名前設定
	pScore->SetName("Score");
	
	// ヴェクターに追加
	AddObj(pScore);
}

//************************************************************************
// プレイヤー攻撃レベル更新
//************************************************************************
void CGame::UpdatePlyerAttackLevel(void)
{
	// プレイヤー取得
	CPlayer*	pPlayer		= GetObj<CPlayer>	("Player");

	// カウンター取得
	CCounter*	pCounter	= pPlayer->GetComponent<CCounter>("AttackCounter");

	// 攻撃レベル取得
	int nLevel = pPlayer->GetAttacklevel();
}

//************************************************************************
// カメラの設定
//************************************************************************
void CGame::CameraSet(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// プレイヤー取得
	CPlayer* pPlayer = GetObj	<CPlayer>("Player");

	// カメラの回転取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// 情報取得
	CInfo* pInfoPlayer = pPlayer->GetComponent<CInfo>();

	// 角度
	float fAngleCamera = AngleAtan2f(D3DXVECTOR2(pInfoPlayer->pos.x, pInfoPlayer->pos.z), D3DXVECTOR2(0.0f, 0.0f));
	fAngleCamera = AngleNormalize(fAngleCamera);

	// カメラ注視点(プレイヤーの位置にする)
	D3DXVECTOR3 cameraPosR = pInfoPlayer->pos;
	cameraPosR.y += m_info.CAMERA_HEIGHT;

	// カメラ始点設定
	pCamera->SetRotation(D3DXVECTOR3(m_info.CAMERA_ROT_X, fAngleCamera, 0.0f));
	pCamera->SetPositionRDest(cameraPosR);
	pCamera->SetDistance(m_info.CAMERA_DISTANCE);
	pCamera->ArrangeVDest();

	// カメラ注視点をプレイヤーの向きで移動
	cameraPosR.x += sinf(pInfoPlayer->rotDest.y) * m_info.CAMERA_DIPS;
	cameraPosR.z += cosf(pInfoPlayer->rotDest.y) * m_info.CAMERA_DIPS;
	pCamera->SetPositionRDest(cameraPosR);

	// 視点取得
	D3DXVECTOR3 cameraPosVDest = pCamera->GetPositionVDest();

	// プレイヤー向き取得(反転させる)
	float fAnglePlayer = pInfoPlayer->rotDest.y + D3DX_PI;
	fAnglePlayer = AngleNormalize(fAnglePlayer);

	// カメラが海より下にいかないようにする
	if (cameraPosVDest.y <= 100.0f)
	{
		cameraPosVDest.y = 100.0f;
		pCamera->SetPositionVDest(cameraPosVDest);
	}
}