//***************************************************************
// 
// 船待機状態
// Author Tetsuji Yamamoto
// 
//***************************************************************
#include "stateShipNewtral.h"
#include "object.h"
#include "compCollisionShapeMesh.h"
#include "compInfo.h"
#include "compStateMachine.h"
#include "stateShipMove.h"
#include "input.h"

//***************************************************************
// 船待機状態コンストラクタ
//***************************************************************
CStateShipNewtral::CStateShipNewtral()
{

}

//***************************************************************
// 船待機状態初期化
//***************************************************************
bool CStateShipNewtral::Init(void)
{
	return true;
}

//***************************************************************
// 船待機状態更新
//***************************************************************
void CStateShipNewtral::Update(void)
{
	// オーナー取得
	CObject* pOwner = GetOwner();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// メッシュのコリジョン取得
	CCollisionShapeMesh* pCollMesh = pOwner->GetComponent<CCollisionShapeMesh>();

	// コリジョンがなければ処理しない
	if (pCollMesh == nullptr)return;

	for (auto& iter : pCollMesh->GetHitedName())
	{
		// プレイヤーが乗っている
		if (iter == "PlayerSpher")
		{
			// 移動状態に移行
			pOwner->GetComponent<CStateMachine>()->ChangeState(new CStateShipMove);
			return;
		}
	}

}

//***************************************************************
// 船待機状態修了
//***************************************************************
void CStateShipNewtral::Uninit(void)
{
	delete this;
}

//***************************************************************
// 船待機状態描画
//***************************************************************
void CStateShipNewtral::Draw(void)
{

}
