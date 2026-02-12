//**********************************************
// 
// カメラ
// Author Tetsuji Yamamoto
// 
//**********************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#include "myMath.h"
#include "manager.h"

// カメラクラス
class CCamera
{
public:

	static constexpr float SLIDE_SENSITIVITY	= 0.5f;
	static constexpr float MOUCE_SENSITIVITY	= 0.01f;
	static constexpr float CAMERA_ANGLE_LIMIT	= 0.1f;
	static constexpr float CHECK_DIGIT_VALUE	= 10000.0f;
	static constexpr float DRAW_DISTANS			= 25000.0f;
	static constexpr float PALAM_DEST			= 0.2f;

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void OperateMousePosR(void);
	void OperateMousePosV(void);
	void OperateMouseDistance(void);
	void OperateMouseSlide(void);

	void Debug(void);

	void TrackingCamera(D3DXVECTOR3 posR, float fCoefficient = 1.0f);
	
	// ゲッター
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXVECTOR3 GetPositionR(void) { return m_posR; }
	D3DXVECTOR3 GetPositionV(void) { return m_posV; }
	D3DXVECTOR3 GetPositionRDest(void) { return m_posRDest; }
	D3DXVECTOR3 GetPositionVDest(void) { return m_posVDest; }
	D3DXMATRIX GetMtxViewInvers(void) { return m_mtxViewInvers; }
	static CCamera* GetCamera(void) { return CManager::GetInstance()->GetCamera(); }

	// セッター
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetDistance(float fDistance) { m_fDistance = fDistance; }
	void SetPositionVDest(D3DXVECTOR3 posV) { m_posVDest = posV; }
	void SetPositionRDest(D3DXVECTOR3 posR) { m_posRDest = posR; }
	void ArrangeVDest(void);
	void ArrangeRDest(void);

private:
	void SetPositionV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetPositionR(D3DXVECTOR3 posR){ m_posR = posR; }

	D3DXMATRIX m_mtxView;			// ビューマトリックス
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxViewInvers;		//  カメラの逆行列
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 中視点
	D3DXVECTOR3 m_posVDest;			// 目的視点
	D3DXVECTOR3 m_posRDest;			// 目的中視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 向き
	float m_fDistance;				// 距離
};


#endif // !_CAMERA_H_
