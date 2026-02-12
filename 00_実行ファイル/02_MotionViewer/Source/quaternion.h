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
}

#endif