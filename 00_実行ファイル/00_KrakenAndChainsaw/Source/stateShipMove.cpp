//***************************************************************
// 
// 船移動状態
// Author Tetsuji Yamamoto
// 
//***************************************************************
#include "stateShipMove.h"
#include "stateShipNewtral.h"
#include "compMovement.h"
#include "compShipVariable.h"
#include "compCollisionShapeMesh.h"
#include "compStateMachine.h"
#include "input.h"
#include "camera.h"
#include "vector.h"

using namespace my_vector;

//***************************************************************
// 船移動状態コンストラクタ
//***************************************************************
CStateShipMove::CStateShipMove()
{
}

//***************************************************************
// 船移動状態初期化
//***************************************************************
bool CStateShipMove::Init(void)
{
	return true;
}

//***************************************************************
// 船移動状態更新
//***************************************************************
void CStateShipMove::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// 移動処理取得
	CMovement* pMovement = pOwner->GetComponent<CMovement>();
	CInfo * pInfo = pOwner->GetComponent<CInfo>();

	// 船用変数取得
	CShipVariable* pShipVariable = pOwner->GetComponent<CShipVariable>();

	// メッシュのコリジョン取得
	CCollisionShapeMesh* pCollMesh = pOwner->GetComponent<CCollisionShapeMesh>();

	// 変数が無ければ処理しない
	if (pShipVariable == nullptr)return;

	// コリジョンがなければ処理しない
	if (pCollMesh == nullptr)return;

	// 乗っているか調べる
	bool bRanding = false;
	for (auto& iter : pCollMesh->GetHitedName())
	{
		// プレイヤーが乗っている
		if (iter == "PlayerSpher")
		{
			// 乗っているフラグを立てる
			bRanding = true;
		}
	}

	// 乗っていなかったら
	if (bRanding == false)
	{
		// 待機状態に移行
		pOwner->GetComponent<CStateMachine>()->ChangeState(new CStateShipNewtral);
		return;
	}

	// 船の変数取得
	D3DXVECTOR3 pos = pInfo->pos;
	D3DXVECTOR3 posOld = pInfo->posOld;
	D3DXVECTOR3 posCenter = pShipVariable->GetPositionCenter();
	float fAngleMove = pShipVariable->GetAngleMove();
	float fAngleMoveDest = pShipVariable->GetAngleMoveDest();
	float fMovement = pShipVariable->GetMovement();
	float fRadius = pShipVariable->GetRadius();

	// 移動量変数
	float fAddMove = 0.0f;
	float fAngle = pInfo->rotDest.y;

	// 右回り
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{
		fAddMove = fMovement;
		fAngle = AngleAtan2f(D3DXVECTOR2(posCenter.x, posCenter.z), D3DXVECTOR2(pos.x, pos.z));
		fAngle -= D3DX_PI * 0.5f;
	}

	// 角度の正規化
	fAngle = AngleNormalize(fAngle);

	// 角度を足す
	fAngleMoveDest += fAddMove;

	// 角度の正規化
	fAngleMoveDest = AngleNormalize(fAngleMoveDest);

	// 位置を求める
	pos.x = sinf(fAngleMove) * fRadius;
	pos.z = cosf(fAngleMove) * fRadius;

	// 位置と向き設定
	pInfo->rotDest.y = fAngle;
	pInfo->rotDest.y = AngleNormalize(pInfo->rotDest.y);
	pInfo->pos = pos;

	// 船変数設定
	pShipVariable->SetPositionCenter(posCenter);
	pShipVariable->SetAngleMove(fAngleMove);
	pShipVariable->SetAngleMoveDest(fAngleMoveDest);
	pShipVariable->SetMovement(fMovement);
	pShipVariable->SetRadius(fRadius);
}

//***************************************************************
// 船移動状態修了
//***************************************************************
void CStateShipMove::Uninit(void)
{
	delete this;
}

//***************************************************************
// 船移動状態描画
//***************************************************************
void CStateShipMove::Draw(void)
{

}
