//****************************************************
// 
// ベクトル処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "vector.h"

//**************************************************
// ベクトルのなす角を求める処理(ラジアン角)
//**************************************************
float vector::VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB)
{
	// ベクトルを正規化
	D3DXVec3Normalize(&vecA, &vecA);
	D3DXVec3Normalize(&vecB, &vecB);

	// 内積を求めて
	float fDot = D3DXVec3Dot(&vecA, &vecB);

	// ラジアン角に変換
	return (float)acos(fDot);
}

//**************************************************
// ベクトルのなす角を求める処理(ラジアン角)
//**************************************************
float vector::VectorsAngle2D(D3DXVECTOR2 vecA, D3DXVECTOR2 vecB)
{
	D3DXVECTOR2 vecAB = vecA - vecB;
	return atan2f(vecAB.x, vecAB.y);
}

//**************************************************
// ポリゴンの押し出し処理(滑らない)
//**************************************************
float vector::ExtrusionPolygonNoSlip(D3DXVECTOR3* pOut, D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush)
{
	// 法線とポリゴンの一頂点の内積
	float fDot = ((nor.x * posPolygon.x) + (nor.y * posPolygon.y) + (nor.z * posPolygon.z));

	//プレイヤーのポリゴン上のy座標を求める
	float fY = (-nor.x * posPush.x - nor.z * posPush.z + fDot) / nor.y;

	// 希望の位置に設定
	float fPush = fY - posPush.y;

	// 押し出した位置設定
	*pOut = posPush;
	pOut->y = fY;

	// 押し出した分を返す
	return fPush;
}

//**************************************************
// ポリゴンの押し出し処理(滑る)
//**************************************************
D3DXVECTOR3 vector::ExtrusionPolygonSlide(D3DXVECTOR3* pOut, D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush)
{
	D3DXVECTOR3 vecLinePos = posPush - posPolygon;	// ポリゴンの頂点から押し出す位置までのベクトル

	// ポリゴンから押し出す位置までの長さを求める
	float fDotLengthPos = fabsf(D3DXVec3Dot(&nor, &vecLinePos));

	// 法線方向に押し出す
	D3DXVECTOR3 vecPush = nor * fDotLengthPos;

	// 押し出す
	*pOut = posPush + vecPush;

	// 押し出した分を返す
	return vecPush;
}

//************************************************************************************************************************
// 角度の正規化
//************************************************************************************************************************
float vector::AngleNormalize(const float fAngle)
{
	float fAngleN = fAngle;

	// 角度の正規化
	if (fAngleN > D3DX_PI)
	{
		fAngleN = -D3DX_PI + (fAngleN - D3DX_PI);
	}
	else if (fAngleN < -D3DX_PI)
	{
		fAngleN = D3DX_PI + (fAngleN + D3DX_PI);
	}

	return fAngleN;
}

//************************************************************************************************************************
// 角度の正規化
//************************************************************************************************************************
D3DXVECTOR3 vector::AngleNormalize(const D3DXVECTOR3 angle)
{
	D3DXVECTOR3 angleN;
	angleN.x = AngleNormalize(angle.x);
	angleN.y = AngleNormalize(angle.y);
	angleN.z = AngleNormalize(angle.z);

	return angleN;
}

//************************************************************************************************************************
// ベクトルの角度算出
//************************************************************************************************************************
float vector::AngleAtan2f(const D3DXVECTOR2 vecA, const D3DXVECTOR2 vecB)
{
	float fX = vecB.x - vecA.x;
	float fY = vecB.y - vecA.y;
	float fAngle = atan2f(fX, fY);
	return fAngle;
}

//**************************************************
// 最短の角度差を計算する処理
//**************************************************
float vector::AngleShortestDiff(float AngleDest, float Angle)
{
	float fAngleDiff = AngleDest - Angle;

	// 差分が角度一周分を超えていたら差分から一周を引いて次に足して角度を求める
	if (fAngleDiff > D3DX_PI)
	{
		fAngleDiff = fAngleDiff - D3DX_PI * 2;
	}
	else if (fAngleDiff < -D3DX_PI)
	{
		fAngleDiff = fAngleDiff + D3DX_PI * 2;
	}

	// 結果を返す
	return fAngleDiff;
}

//************************************************************************************************************************
// ベクトルの正規化
//************************************************************************************************************************
D3DXVECTOR3 vector::Vec3Normalize(const D3DXVECTOR3 vec)
{
	float fLength = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	if (fLength == 0.0f)return InitValu::VEC3_NULL;
	D3DXVECTOR3 vecN
	{
		vec.x / fLength,
		vec.y / fLength,
		vec.z / fLength
	};
	return vecN;
}

//**************************************************
// 位置に回転マトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 vector::Vec3MulMtx(const D3DXVECTOR3 pos, const D3DXMATRIX mtx)
{
	D3DXVECTOR3 posOut;
	posOut.x = (pos.x * mtx._11) + (pos.y * mtx._21) + (pos.z * mtx._31) + mtx._41;
	posOut.y = (pos.x * mtx._12) + (pos.y * mtx._22) + (pos.z * mtx._32) + mtx._42;
	posOut.z = (pos.x * mtx._13) + (pos.y * mtx._23) + (pos.z * mtx._33) + mtx._43;
	return posOut;
}

//**************************************************
// 位置に回転マトリックスだけを掛ける処理
//**************************************************
D3DXVECTOR3 vector::Vec3MulMtxRotat(const D3DXVECTOR3 pos, const D3DXMATRIX mtx)
{
	D3DXVECTOR3 posOut;
	posOut.x = (pos.x * mtx._11) + (pos.y * mtx._21) + (pos.z * mtx._31);
	posOut.y = (pos.x * mtx._12) + (pos.y * mtx._22) + (pos.z * mtx._32);
	posOut.z = (pos.x * mtx._13) + (pos.y * mtx._23) + (pos.z * mtx._33);
	return posOut;
}

//**************************************************
// ベクトルの内積
//**************************************************
float vector::Vec3Dot(const D3DXVECTOR3 vecA, const D3DXVECTOR3 vecB)
{
	return (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z);
}

//**************************************************
// ベクトルの外積
//**************************************************
D3DXVECTOR3 vector::Vec3Cross(const D3DXVECTOR3 vecA, const D3DXVECTOR3 vecB)
{
	D3DXVECTOR3 cross;
	cross.x = vecA.y * vecB.z - vecA.z * vecB.y;
	cross.y = vecA.z * vecB.x - vecA.x * vecB.z;
	cross.z = vecA.x * vecB.y - vecA.y * vecB.x;
	return cross;
}

//**************************************************
// ベクトルの長さ
//**************************************************
float vector::Vec3Length(const D3DXVECTOR3 vec)
{
	return sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

//**************************************************
// ベクトル同士の角度を求める処理
//**************************************************
float vector::AngleVec3AndVec3(const D3DXVECTOR3 vecA, const  D3DXVECTOR3 vecB)
{
	// ベクトルを正規化
	D3DXVECTOR3 vecAN,vecBN;
	D3DXVec3Normalize(&vecAN, &vecA);
	D3DXVec3Normalize(&vecBN, &vecB);

	// 内積を求めて
	float fDot = D3DXVec3Dot(&vecAN, &vecBN);

	// ラジアン角に変換
	return (float)acos(fDot);
}