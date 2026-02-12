//****************************************************
// 
// クォータニオン計算処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

// ベクトル処理名前空間
namespace Quaternion
{
	D3DXQUATERNION QuatRotationAxis(const D3DXVECTOR3 axis, const float fAngle);
	D3DXQUATERNION QuatRotationAxis(const D3DXVECTOR3 axis, const D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXQUATERNION QuatRotationMatrix(const D3DXMATRIX mtxRot);
	D3DXQUATERNION QuatMultiply(const D3DXQUATERNION quatA, const D3DXQUATERNION quatB);
	D3DXQUATERNION QuatSlerp(D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric);
}

#endif