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
	D3DXVECTOR3 cross = my_vector::Vec3Cross(axis, vecY);
	float fAngle = my_vector::AngleVec3AndVec3(vecY, axis);
	D3DXQuaternionRotationAxis(&quat, &cross, fAngle);
	return quat;
}

//**************************************************
// クォータニオンを回転行列から作成
//**************************************************
D3DXQUATERNION Quaternion::QuatRotationMatrix(const D3DXMATRIX mtxRot)
{
	D3DXQUATERNION quat;
	D3DXQuaternionRotationMatrix(&quat,&mtxRot);
	return quat;
}

//**************************************************
// クォータニオン同士の掛け合わせ
//**************************************************
D3DXQUATERNION Quaternion::QuatMultiply(const D3DXQUATERNION quatA, const D3DXQUATERNION quatB)
{
	D3DXQUATERNION quat;
	D3DXQuaternionMultiply(&quat, &quatA, &quatB);
	return quat;
}

//**************************************************
// クォータニオンの球面線形補間処理
//**************************************************
D3DXQUATERNION Quaternion::QuatSlerp( D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric)
{
	// 出力用変数
	D3DXQUATERNION quatOut = InitValu::QUAT_NULL;

	// クォータニオン同士の内積を取る
	float fDotQuat = quatNext.x * quatNow.x + quatNext.y * quatNow.y + quatNext.z * quatNow.z + quatNext.w * quatNow.w;
	fDotQuat = Clamp(fDotQuat, 0.0f, 1.0f);

	float fCosTheta = acosf(fDotQuat);

	// 近いほうから回るようにする
	if (fCosTheta <= 0.0f)
	{
		fCosTheta = -fCosTheta;
	}

	float fSin = sinf(fCosTheta);

	// ゼロ以下だった場合線形補間で返す
	if (fSin <= FLT_EPSILON)
	{
		quatOut = quatNow + (quatNext - quatNow) * fParametric;
		return quatOut;
	}

	float fSinA = sinf(fCosTheta * (1.0f - fParametric));
	float fSinB = sinf(fCosTheta * fParametric);

	float fSinMugA = fSinA / fSin;
	float fSinMugB = fSinB / fSin;

	quatOut.x = quatNow.x * fSinMugA + quatNext.x * fSinMugB;
	quatOut.y = quatNow.y * fSinMugA + quatNext.y * fSinMugB;
	quatOut.z = quatNow.z * fSinMugA + quatNext.z * fSinMugB;
	quatOut.w = quatNow.w * fSinMugA + quatNext.w * fSinMugB;

	return quatOut;
}