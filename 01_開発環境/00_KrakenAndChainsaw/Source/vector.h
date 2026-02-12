//****************************************************
// 
// ベクトル処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _VECTOR_H_
#define _VECTOR_H_

// ベクトル処理名前空間
namespace my_vector
{
	float VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);
	float VectorsAngle2D(D3DXVECTOR2 vecA, D3DXVECTOR2 vecB);
	float ExtrusionPolygonNoSlip(D3DXVECTOR3* pOut, D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush);
	D3DXVECTOR3 ExtrusionPolygonSlide(D3DXVECTOR3* pOut, D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush);
	float AngleNormalize(const float fAngle);
	D3DXVECTOR3 AngleNormalize(const D3DXVECTOR3 angle);
	float AngleAtan2f(const D3DXVECTOR2 vecA, const D3DXVECTOR2 vecB);
	float AngleShortestDiff(float AngleDest, float Angle);
	D3DXVECTOR3 Vec3Normalize(const D3DXVECTOR3 vec);
	D3DXVECTOR3 Vec3MulMtx(const D3DXVECTOR3 pos, const D3DXMATRIX mtx);
	D3DXVECTOR3 Vec3MulMtxRotat(const D3DXVECTOR3 pos, const D3DXMATRIX mtx);
	float Vec3Dot(const D3DXVECTOR3 vecA, const D3DXVECTOR3 vecB);
	D3DXVECTOR3 Vec3Cross(const D3DXVECTOR3 vecA, const D3DXVECTOR3 vecB);
	float Vec3Length(const D3DXVECTOR3 vec);
	float AngleVec3AndVec3(const D3DXVECTOR3 vecA, const  D3DXVECTOR3 vecB);
	D3DXVECTOR3 Vec3Lerp(const D3DXVECTOR3 vecA, const  D3DXVECTOR3 vecB,const float fPalam);
}

#endif