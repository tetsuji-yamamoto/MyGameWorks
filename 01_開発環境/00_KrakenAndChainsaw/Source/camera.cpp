//**********************************************
// 
// カメラ
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "camera.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "quaternion.h"

using namespace Quaternion;

//**********************************************
// カメラ処理
//**********************************************
CCamera::CCamera()
{
	m_mtxProjection	= InitValu::MTX_NULL;	// ビューマトリックス
	m_mtxView		= InitValu::MTX_NULL;	// プロジェクションマトリックス
	m_posR			= InitValu::VEC3_NULL;	// 視点
	m_posV			= InitValu::VEC3_NULL;	// 中視点
	m_vecU			= InitValu::VEC3_NULL;	// 上方向ベクトル
	m_rot			= InitValu::VEC3_NULL;	// 向き
	m_posVDest		= InitValu::VEC3_NULL;	// 目的視点
	m_posRDest		= InitValu::VEC3_NULL;	// 目的中視点
	m_fDistance		= NULL;					// 距離
	m_mtxViewInvers	= InitValu::MTX_NULL;	//  カメラの逆行列
}

//**********************************************
// カメラ処理
//**********************************************
CCamera::~CCamera()
{
}

//**********************************************
// カメラ処理
//**********************************************
HRESULT CCamera::Init(void)
{
	// 視点・中視点・上方向を設定する
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 視点の位置
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 注視点の位置
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);							// 固定
	m_rot = D3DXVECTOR3(-D3DX_PI * 0.25f, 0.0f, 0.0f);				// 向き
	m_fDistance = 500.0f;											// 距離

	m_posV.x = m_posR.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y + cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	m_posVDest = m_posV;
	m_posRDest = m_posR;

	return S_OK;
}

//**********************************************
// カメラ処理
//**********************************************
void CCamera::Uninit(void)
{
	delete this;
}

//**********************************************
// カメラ処理
//**********************************************
void CCamera::Update(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// マウス取得
	CInputMouse* pInputMouce = CManager::GetInstance()->GetInputMouse();

	// 角度の正規化
	CMyMath::NormalizingAngles(&m_rot);

	// ビューマトリックス取得
	D3DXMATRIX mtxview;
	pDevice->GetTransform(D3DTS_VIEW, &mtxview);

	// カメラの逆行列を設定
	m_mtxViewInvers._11 = mtxview._11;
	m_mtxViewInvers._12 = mtxview._21;
	m_mtxViewInvers._13 = mtxview._31;
	m_mtxViewInvers._21 = mtxview._12;
	m_mtxViewInvers._22 = mtxview._22;
	m_mtxViewInvers._23 = mtxview._32;
	m_mtxViewInvers._31 = mtxview._13;
	m_mtxViewInvers._32 = mtxview._23;
	m_mtxViewInvers._33 = mtxview._33;

	D3DXVECTOR3 DiffR = m_posRDest - m_posR;
	D3DXVECTOR3 DiffV = m_posVDest - m_posV;
	m_posR = m_posR + DiffR * PALAM_DEST;
	m_posV = m_posV + DiffV * PALAM_DEST;
}

//**********************************************
// カメラ処理
//**********************************************
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ビューマックスの初期化
	D3DXMatrixIsIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクトマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		30.0f,
		DRAW_DISTANS);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//**********************************************
// カメラマウスでの注視点処理
//**********************************************
void CCamera::OperateMousePosR(void)
{
	// マウス取得
	CInputMouse* pInput = CManager::GetInstance()->GetInputMouse();

	// 変化量取得
	D3DXVECTOR3 velocity = pInput->GetVelocity();

	m_rot.x += velocity.y * MOUCE_SENSITIVITY;
	m_rot.y += velocity.x * MOUCE_SENSITIVITY;

	if (m_rot.x >= -D3DX_PI * CAMERA_ANGLE_LIMIT)
	{
		m_rot.x = -D3DX_PI * CAMERA_ANGLE_LIMIT;
	}

	if (m_rot.x <= -D3DX_PI * (1.0f - CAMERA_ANGLE_LIMIT))
	{
		m_rot.x = -D3DX_PI * (1.0f - CAMERA_ANGLE_LIMIT);
	}

	m_posRDest.x = m_posVDest.x + sinf(m_rot.x) * sinf(m_rot.y) * -m_fDistance;
	m_posRDest.y = m_posVDest.y + cosf(m_rot.x) * -m_fDistance;
	m_posRDest.z = m_posVDest.z + sinf(m_rot.x) * cosf(m_rot.y) * -m_fDistance;
}

//**********************************************
// カメラマウスでの視点処理
//**********************************************
void CCamera::OperateMousePosV(void)
{
	// マウス取得
	CInputMouse* pInput = CManager::GetInstance()->GetInputMouse();

	// 変化量取得
	D3DXVECTOR3 velocity = pInput->GetVelocity();

	m_rot.x += velocity.y * MOUCE_SENSITIVITY;
	m_rot.y += velocity.x * MOUCE_SENSITIVITY;

	if (m_rot.x >= -D3DX_PI * CAMERA_ANGLE_LIMIT)
	{
		m_rot.x = -D3DX_PI * CAMERA_ANGLE_LIMIT;
	}

	if (m_rot.x <= -D3DX_PI * (1.0f - CAMERA_ANGLE_LIMIT))
	{
		m_rot.x = -D3DX_PI * (1.0f - CAMERA_ANGLE_LIMIT);
	}

	m_posVDest.x = m_posRDest.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posRDest.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//**********************************************
// カメラマウスでの距離操作
//**********************************************
void CCamera::OperateMouseDistance(void)
{
	// マウス取得
	CInputMouse* pInput = CManager::GetInstance()->GetInputMouse();

	// 変化量取得
	D3DXVECTOR3 pos = pInput->GetPosition();

	// 変化があれば処理する
	if ((int)pos.z != 0)
	{
		int nNomalizeZ = (int)sqrtf(pos.z * pos.z);
		nNomalizeZ = (int)pos.z / nNomalizeZ;

		m_fDistance = m_fDistance + (m_fDistance * 0.1f) * (float)nNomalizeZ;

		if (m_fDistance <= 10.0f)
		{
			m_fDistance = 10.0f;
		}

		m_posVDest.x = m_posRDest.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * m_fDistance;
		m_posVDest.z = m_posRDest.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	}
}

//**********************************************
// カメラマウスでのスライド操作
//**********************************************
void CCamera::OperateMouseSlide(void)
{
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	// 変化量取得
	D3DXVECTOR3 velocityMouse = pInputMouse->GetVelocity();

	// カメラの向いている方向ベクトル
	D3DXVECTOR3 vecCamera = m_posRDest - m_posVDest;

	// カメラに対しての2D方向のベクトルの基準を決める
	D3DXVECTOR3 vecX, vecY;

	// X方向ベクトルを求める
	D3DXVec3Cross(&vecX, &vecCamera, &m_vecU);
	D3DXVec3Normalize(&vecX, &vecX);

	// Y方向ベクトルを求める
	D3DXVec3Cross(&vecY, &vecX, &vecCamera);
	D3DXVec3Normalize(&vecY, &vecY);

	// 移動量を決める
	D3DXVECTOR3 move = InitValu::VEC3_NULL;

	// プラス方向かマイナス方向か
	float fPlusMinus = 0.0f;

	// 移動量
	float fAmount = 0.0f;

	// X方向を求める
	// 変化があれば処理する
	if ((int)(velocityMouse.x * CHECK_DIGIT_VALUE) != 0)
	{
		// プラスマイナスを求める
		int nNomalizeX = (int)sqrtf(velocityMouse.x * velocityMouse.x);
		fPlusMinus = (float)((int)velocityMouse.x / nNomalizeX);

		// 大きさを求める
		fAmount = sqrtf(velocityMouse.x * velocityMouse.x);

		// Xベクトル方向の移動量を求める
		vecX = vecX * fAmount * fPlusMinus;
		vecX *= SLIDE_SENSITIVITY;
	}
	// 変化が無ければ
	else
	{
		// ゼロ
		vecX = InitValu::VEC3_NULL;
	}

	if ((int)(velocityMouse.y * CHECK_DIGIT_VALUE) != 0)
	{
		// プラスマイナスを求める
		int nNomalizeY = (int)sqrtf(velocityMouse.y * velocityMouse.y);
		fPlusMinus = (float)((int)velocityMouse.y / nNomalizeY);

		// 大きさを求める
		fAmount = sqrtf(velocityMouse.y * velocityMouse.y);

		// Yベクトル方向の移動量を求める
		vecY = vecY * fAmount * fPlusMinus;
		vecY *= SLIDE_SENSITIVITY;
	}
	// 変化が無ければ
	else
	{
		// ゼロ
		vecY = InitValu::VEC3_NULL;
	}

	// X方向とY方向を足した物を移動量とする
	move = vecX + vecY;

	// 視点注視点に移動量を足す
	m_posVDest += move;
	m_posRDest += move;

}

//**********************************************
// 追従カメラ
//**********************************************
void CCamera::TrackingCamera(D3DXVECTOR3 posR,float fCoefficient)
{
	// 距離を図る
	D3DXVECTOR3 vecDiff = posR - m_posRDest;
	vecDiff *= fCoefficient;

	m_posRDest += vecDiff;

	m_posVDest.x = m_posRDest.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posRDest.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//**********************************************
// カメラ視点計算
//**********************************************
void CCamera::ArrangeVDest(void)
{ 
	m_posVDest.x = m_posRDest.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posRDest.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	//m_posVDest = CMyMath::SphericalCoordinates(m_posRDest, m_rot, m_fDistance); 
}

//**********************************************
// カメラ注視点計算
//**********************************************
void CCamera::ArrangeRDest(void) 
{
	m_posRDest.x = m_posVDest.x + sinf(m_rot.x) * sinf(m_rot.y) * -m_fDistance;
	m_posRDest.y = m_posVDest.y + cosf(m_rot.x) * -m_fDistance;
	m_posRDest.z = m_posVDest.z + sinf(m_rot.x) * cosf(m_rot.y) * -m_fDistance;
	//m_posRDest = CMyMath::SphericalCoordinates(m_posVDest, m_rot, -m_fDistance); 
}

//**********************************************
// カメラ処理
//**********************************************
void CCamera::Debug(void)
{
	// デバッグプロック取得
	CDebugprocc* pDebug = CManager::GetInstance()->GetDebugprocc();

	pDebug->Print("[ Camera ]\n");
	pDebug->Print("m_posV.x     : %0.3f .y : %0.3f .z : %0.3f\n", m_posV.x, m_posV.y, m_posV.z);
	pDebug->Print("m_posR.x     : %0.3f .y : %0.3f .z : %0.3f\n", m_posR.x, m_posR.y, m_posR.z);
	pDebug->Print("m_posVDest.x : %0.3f .y : %0.3f .z : %0.3f\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	pDebug->Print("m_posRDest.x : %0.3f .y : %0.3f .z : %0.3f\n", m_posRDest.x, m_posRDest.y, m_posRDest.z);
	pDebug->Print("m_rot.x      : %0.3f .y : %0.3f .z : %0.3f\n", m_rot.x, m_rot.y, m_rot.z);
	pDebug->Print("m_fDistance  : %0.3f\n", m_fDistance);
	pDebug->Print("\n");
}