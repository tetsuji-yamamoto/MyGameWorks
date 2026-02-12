//********************************************************
// 
// クラーケン感知コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#include "compKrakenSensing.h"
#include "compCollisionShapeMesh.h"
#include "compInfo.h"
#include "object.h"
#include "vector.h"

using namespace my_vector;

//********************************************************
// コンストラクタ
//********************************************************
CKrakenSensing::CKrakenSensing()
{
}

//********************************************************
// デストラクタ
//********************************************************
CKrakenSensing::~CKrakenSensing()
{

}

//********************************************************
// 初期化
//********************************************************
void CKrakenSensing::Init(void)
{

}

//********************************************************
// 破棄
//********************************************************
void CKrakenSensing::Uninit(void)
{
	delete this;
}

//********************************************************
// 更新
//********************************************************
void CKrakenSensing::Update(void)
{
	
}

//********************************************************
// 範囲感知
//********************************************************
bool CKrakenSensing::GetIsRangePosition(const D3DXVECTOR3 pos,const float fHeight, const float fAngle, const float fRange)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 情報取得
	CInfo* pInfo = pOwner->GetComponent<CInfo>();

	// 取得出来ていなければ処理しない
	if (pInfo == nullptr)return false;

	// 高さが範囲以上だったらここにはいない
	if (pos.y > fHeight)return false;

	// 角度を計算
	float fAnglePos = AngleAtan2f(D3DXVECTOR2(pos.x, pos.z), D3DXVECTOR2(pInfo->pos.x, pInfo->pos.z));
	float fAngleA = fAngle + fRange * Parameters::HALF;
	float fAngleB = fAngle - fRange * Parameters::HALF;

	// 角度が範囲外ならここにはいない
	if (!(fAngleA >= fAnglePos && fAngleB <= fAnglePos))return false;

	// ここにいると
	return true;
}

//********************************************************
// 範囲感知
//********************************************************
bool CKrakenSensing::GetIsAttaked(const float fHeight, const float fAngle, const float fRange)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// 情報取得
	CInfo* pInfo = pOwner->GetComponent<CInfo>();

	// メッシュの当たり判定取得
	CCollisionShapeMesh* pCollisonMesh = pOwner->GetComponent<CCollisionShapeMesh>(std::string("KrakenBody"));

	// 取得出来ていなければ処理しない
	if (pInfo == nullptr)return false;
	if (pCollisonMesh == nullptr)return false;

	// 体に攻撃されていない
	if (pCollisonMesh->FindHitedName(std::string("AttackChainsaw")) == false)return false;

	// 高さを取得
	D3DXVECTOR3 posHit = pCollisonMesh->m_posHit;

	// 高さが範囲以上だったらここにはいない
	if (posHit.y > fHeight)return false;

	// 角度を計算
	float fAnglePos = AngleAtan2f(D3DXVECTOR2(posHit.x, posHit.z), D3DXVECTOR2(pInfo->pos.x, pInfo->pos.z));
	float fAngleA = fAngle + fRange * Parameters::HALF;
	float fAngleB = fAngle - fRange * Parameters::HALF;

	// 角度が範囲外ならここにはいない
	if (!(fAngleA >= fAnglePos && fAngleB <= fAnglePos))return false;

	// ここにいると
	return true;
}