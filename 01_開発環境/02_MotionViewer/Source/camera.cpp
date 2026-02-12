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

//**********************************************
// カメラ処理
//**********************************************
CCamera::CCamera()
{
	ZeroMemory(m_mtxProjection,sizeof(m_mtxProjection));// ビューマトリックス
	ZeroMemory(m_mtxView, sizeof(m_mtxView));			// プロジェクションマトリックス
	ZeroMemory(m_posR, sizeof(m_posR));					// 視点
	ZeroMemory(m_posV, sizeof(m_posV));					// 中視点
	ZeroMemory(m_vecU, sizeof(m_vecU));					// 上方向ベクトルs
	ZeroMemory(m_rot, sizeof(m_rot));					// 向き
	m_fDistance = 0.0f;									// 距離
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
	m_rot = D3DXVECTOR3(-D3DX_PI * 0.25f,0.0f, 0.0f);				// 向き
	m_fDistance = 500.0f;											// 距離
	
	m_posV.x = m_posR.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y + cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	return S_OK;
}

//**********************************************
// カメラ処理
//**********************************************
void CCamera::Uninit(void)
{
	
}

//**********************************************
// カメラ処理
//**********************************************
void CCamera::Update(void)
{
	// マウス取得
	CInputMouse* pInputMouce = CManager::GetInputMouse();
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
		10.0f,
		DRAW_DISTANCE * 5.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//**********************************************
// カメラマウスでの注視点処理
//**********************************************
void CCamera::OperateMousePosR(void)
{
	// マウス取得
	CInputMouse* pInput = CManager::GetInputMouse();

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

	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * -m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * -m_fDistance;
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * -m_fDistance;
}

//**********************************************
// カメラマウスでの視点処理
//**********************************************
void CCamera::OperateMousePosV(void)
{
	// マウス取得
	CInputMouse* pInput = CManager::GetInputMouse();

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

	m_posV.x = m_posR.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y + cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//**********************************************
// カメラマウスでの距離操作
//**********************************************
void CCamera::OperateMouseDistance(void)
{
	// マウス取得
	CInputMouse* pInput = CManager::GetInputMouse();

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

		m_posV.x = m_posR.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posV.y = m_posR.y + cosf(m_rot.x) * m_fDistance;
		m_posV.z = m_posR.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	}
}

//**********************************************
// カメラマウスでのスライド操作
//**********************************************
void CCamera::OperateMouseSlide(void)
{
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// 変化量取得
	D3DXVECTOR3 velocityMouse = pInputMouse->GetVelocity();

	// カメラの向いている方向ベクトル
	D3DXVECTOR3 vecCamera = m_posR - m_posV;
	
	// カメラに対しての2D方向のベクトルの基準を決める
	D3DXVECTOR3 vecX, vecY;

	// X方向ベクトルを求める
	D3DXVec3Cross(&vecX, &vecCamera, &m_vecU);
	D3DXVec3Normalize(&vecX,&vecX);

	// Y方向ベクトルを求める
	D3DXVec3Cross(&vecY, &vecX, &vecCamera);
	D3DXVec3Normalize(&vecY,&vecY);

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
		vecX *= (SLIDE_SENSITIVITY + ((m_fDistance / DRAW_DISTANCE) * 4.0f));
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
		vecY *= (SLIDE_SENSITIVITY + ((m_fDistance / DRAW_DISTANCE) * 4.0f));
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
	m_posV += move;
	m_posR += move;
}