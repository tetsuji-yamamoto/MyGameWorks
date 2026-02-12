//**************************************************
// 
// 機能編集クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionEdit.h"
#include "light.h"
#include "camera.h"
#include "manager.h"
#include "myMath.h"
#include "edit.h"
#include "bone.h"

//-------------------------------------------------
// 機能無し編集クラス

//**************************************************
//  機能無し編集更新
//**************************************************
void CFunctionEditNone::Update(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// キーボード取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// 右クリック中注視点操作
	if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_RIGHT) == true)
	{
		pCamera->OperateMousePosV();
	}
	// 中ボタンスライド操作
	else if (pInputMouse->GetRepeat(CInputMouse::MOUSEBUTTON_CENTER) == true)
	{
		pCamera->OperateMouseSlide();
	}

	// マウスで距離を変える処理
	pCamera->OperateMouseDistance();

	// 選んでいる頂点色を変える処理
	CManager::GetEdit()->editSelectVertexColor();

	// ボーンのモデルの位置を設定する
	CManager::GetEdit()->BoneModelSettingPosition();

	// ボーンモデルのすべての色を変える
	CManager::GetEdit()->BoneModelOllSetColor(InitValu::COLOR_BONE, InitValu::COLOR_BONE_PERENT, InitValu::COLOR_BONE_SELECT);
}

//**************************************************
// 変種機能切替
//**************************************************
void CFunctionEditNone::F1(void)
{
	CManager::GetEdit()->ChangeFunctionEdit(new CFunctionEditRigging);
}

//-------------------------------------------------
// 機能リギング編集クラス

//**************************************************
// 機能リギング更新
//**************************************************
void CFunctionEditRigging::Update(void)
{
	// スキニング処理
	CManager::GetEdit()->editBoneRigging();

	// 選んでいる頂点色を変える処理
	CManager::GetEdit()->editSelectVertexColor();

	// ボーンのモデルの位置を設定する
	CManager::GetEdit()->BoneModelSettingPosition();

	// ボーンモデルのすべての色を変える
	CManager::GetEdit()->BoneModelOllSetColor(InitValu::COLOR_BONE, InitValu::COLOR_BONE_PERENT, InitValu::COLOR_BONE_SELECT);
}

//**************************************************
// 変種機能切替
//**************************************************
void CFunctionEditRigging::F1(void)
{ 
	CManager::GetEdit()->ChangeFunctionEdit(new CFunctionEditSkinning); 
}

//-------------------------------------------------
// 機能スキニング編集クラス

//**************************************************
// 機能スキニング更新
//**************************************************
void CFunctionEditSkinning::Update(void)
{
	// スキニング処理
	CManager::GetEdit()->editBoneSkinning();

	// 選んでいる頂点色を変える処理
	CManager::GetEdit()->editSelectVertexColor();

	// ボーンのモデルの位置を設定する
	CManager::GetEdit()->BoneModelSettingPosition();

	// ボーンモデルのすべての色を変える
	CManager::GetEdit()->BoneModelOllSetColor(InitValu::COLOR_BONE, InitValu::COLOR_BONE_PERENT, InitValu::COLOR_BONE_SELECT);
}

//**************************************************
// 変種機能切替
//**************************************************
void CFunctionEditSkinning::F1(void)
{
	CManager::GetEdit()->ChangeFunctionEdit(new CFunctionEditWeapon);
}	

//-------------------------------------------------
// 機能武器編集クラス

//**************************************************
// 機能武器編集初期化
//**************************************************
void CFunctionEditWeapon::Init(void)
{
	// 武器の親ボーン切替
	CManager::GetEdit()->WeaponSelectFollowBone(0);	
	
	// 操作する武器選択
	CManager::GetEdit()->WeaponSelectOperate(0);
}

//**************************************************
// 機能武器編集更新
//**************************************************
void CFunctionEditWeapon::Update(void)
{
	// 祖須佐する武器を強調表示する設定
	CManager::GetEdit()->WeaponHighlightsOperate();

	// 武器の親ボーンを強調表示する設定
	CManager::GetEdit()->WeaponHighlightsParentBone();
}

//**************************************************
// 機能武器編集クラスF1処理
//**************************************************
void CFunctionEditWeapon::F1(void)
{ 
	CManager::GetEdit()->ChangeFunctionEdit(new CFunctionEditCollModel); 
}	

//-------------------------------------------------
// 機能当たり判定用モデル編集クラス

//**************************************************
// 機能武器編集初期化
//**************************************************
void CFunctionEditCollModel::Init(void)
{
	// 武器の親ボーン切替
	CManager::GetEdit()->CollModelSelectFollowBone(0);

	// 操作する武器選択
	CManager::GetEdit()->CollModelSelectOperate(0);
}

//**************************************************
// 機能武器編集更新
//**************************************************
void CFunctionEditCollModel::Update(void)
{
	// 祖須佐する武器を強調表示する設定
	CManager::GetEdit()->CollModelHighlightsOperate();

	// 武器の親ボーンを強調表示する設定
	CManager::GetEdit()->CollModelHighlightsParentBone();
}

//**************************************************
// 機能武器編集クラスF1処理
//**************************************************
void CFunctionEditCollModel::F1(void)
{
	CManager::GetEdit()->ChangeFunctionEdit(new CFunctionEditPreviview);
}

//-------------------------------------------------
// 機能プレビュー編集クラス

//**************************************************
// 機能プレビュー更新
//**************************************************
void CFunctionEditPreviview::Update(void)
{
	// ボーンのプレビュー処理
	CManager::GetEdit()->editBonePreview();
	
	// 選んでいる頂点色を変える処理
	CManager::GetEdit()->editSelectVertexColor();
	
	// ボーンのモデルの位置を設定する
	CManager::GetEdit()->BoneModelSettingPosition();
	
	// ボーンモデルのすべての色を変える
	CManager::GetEdit()->BoneModelOllSetColor(InitValu::COLOR_BONE, InitValu::COLOR_BONE_PERENT, InitValu::COLOR_BONE_SELECT);
}

//**************************************************
// 変種機能切替
//**************************************************
void CFunctionEditPreviview::F1(void)
{ 
	CManager::GetEdit()->ChangeFunctionEdit(new CFunctionEditNone); 
}	

//**************************************************
// 機能ライトクラス更新
//**************************************************
void CFunctionLight::Update(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// ライト取得
	D3DLIGHT9 light;
	pDevice->GetLight(0, &light);

	// カメラの向き取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// カメラの注視点取得
	D3DXVECTOR3 posRCamera = pCamera->GetPosR();

	// ライトの向き
	D3DXVECTOR3 direction = InitValu::VEC3_NULL;

	// 角度作成
	D3DXVECTOR3 directionRot = rotCamera;
	directionRot.x += D3DX_PI * 1.0f;
	directionRot.y += D3DX_PI * 0.25f;

	// 球座標で位置作成
	direction = CMyMath::SphericalCoordinates(InitValu::VEC3_NULL, directionRot, 1.0f);

	// 設定
	light.Direction = direction;

	// ライト設定
	pDevice->SetLight(0, &light);
}

//**************************************************
// 機能ボーンの大きさクラス更新
//**************************************************
void CFunctionBoneDispSize::Update(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// 編集クラス取得
	CEdit* pEdit = CManager::GetEdit();

	// 距離取得
	float fDistance = pCamera->GetDistance();

	// 距離係数を計算
	float fPalamDistance = (fDistance / 200.0f);

	if (fPalamDistance <= 0.5f)
	{
		fPalamDistance = 0.0f;
	}

	fPalamDistance *= fPalamDistance;

	if (fPalamDistance >= 30.0f)
	{
		fPalamDistance = 30.0f;
	}

	// ボーンをスケーリング
	pEdit->BoneScaling(1.0f + fPalamDistance);
}

//**************************************************
// 機能ボーンスケーリングクラス更新
//**************************************************
void CFunctionBoneMovementScale::Update(void)
{
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// 編集クラス取得
	CEdit* pEdit = CManager::GetEdit();

	// 距離取得
	float fDistance = pCamera->GetDistance();

	// 距離係数を計算
	float fPalamDistance = (fDistance / 50.0f);

	if (fPalamDistance <= 0.2f)
	{
		fPalamDistance = 0.0f;
	}

	// ボーンをスケーリング
	pEdit->BoneMovementScaling(fPalamDistance);
}