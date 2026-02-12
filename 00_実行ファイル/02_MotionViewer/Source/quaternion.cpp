//****************************************************
// 
// クォータニオン計算処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "quaternion.h"
#include "vector.h"
#include "myMath.h"

//****************************************************
// クォータニオン作成
//****************************************************
D3DXQUATERNION Quaternion::QuatRotationAxis(const D3DXVECTOR3 axis, const float fAngle)
{
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, fAngle);
	return quat;
}

//****************************************************
// クォータニオン作成
//****************************************************
D3DXQUATERNION Quaternion::QuatRotationAxis(const D3DXVECTOR3 axis, const D3DXVECTOR3 vecY)
{
	D3DXQUATERNION quat;
	D3DXVECTOR3 cross = vector::Vec3Cross(axis, vecY);
	float fAngle = vector::AngleVec3AndVec3(vecY, axis);
	D3DXQuaternionRotationAxis(&quat, &cross, fAngle);
	return quat;
}