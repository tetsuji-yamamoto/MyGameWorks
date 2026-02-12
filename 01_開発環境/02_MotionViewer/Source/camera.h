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

// カメラクラス
class CCamera
{
public:

	static constexpr float SLIDE_SENSITIVITY = 0.5f;
	static constexpr float MOUCE_SENSITIVITY = 0.01f;
	static constexpr float CAMERA_ANGLE_LIMIT = 0.1f;
	static constexpr float CHECK_DIGIT_VALUE = 10000.0f;
	static constexpr float DRAW_DISTANCE = 10000.0f;

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

	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	float GetDistance(void) { return m_fDistance; }

	// セッター
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetDistance(float fDistance) { m_fDistance = fDistance; }
	void SetPositionV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetPositionR(D3DXVECTOR3 posR) { m_posR = posR; }
	void ArrangeV(void) { m_posV = CMyMath::SphericalCoordinates(m_posR, m_rot, m_fDistance); }
	void ArrangeR(void) { m_posR = CMyMath::SphericalCoordinates(m_posV, m_rot, -m_fDistance); }


private:
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 中視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
	float m_fDistance;			// 距離
};


#endif // !_CAMERA_H_
