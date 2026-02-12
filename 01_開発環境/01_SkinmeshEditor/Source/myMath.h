//*********************************************
// 
// 数学
// Author Tetsuji Yamamoto
// 
//*********************************************
#ifndef _MYMATH_H_
#define _MYMATH_H_

#include "main.h"

// 数学クラス
class CMyMath
{
public:
	CMyMath();
	~CMyMath();

	static D3DXVECTOR3 SphericalCoordinates(D3DXVECTOR3 centerPos, D3DXVECTOR3 rot, float fDistance);
	static int SeekMeshFieldNumPolygonHid(int nDivisionY);
	static int SeekFieldNumPolygonApp(int nDivisionX, int nDivisionY);
	static int SeekFieldNumPolygonAll(int nDivisionX, int nDivisionY);
	static int SeekFieldNumVertexAll(int nDivisionX, int nDivisionY);
	static int SeekFieldNumIndexAll(int nNumPolygonAll);
	static int SeekFanNumIndexAll(int nDivisionX);
	static D3DXMATRIX ConvertLocalCoordinatesToRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 nor);
	static void NormalizingAngles(D3DXVECTOR3* pRot);
	static float ShortestAngle(float fAngleDiff);
	static D3DXVECTOR3 Posision3Dto2D(D3DXVECTOR3* pOutPos2D, D3DXVECTOR3 pos3D);
	static float LengthTrigonometric2D(D3DXVECTOR2 posA, D3DXVECTOR2 posB);
	static float LengthTrigonometric3D(D3DXVECTOR3 posA, D3DXVECTOR3 posB);
	static bool IsColiisionCircleAndCircle2D(D3DXVECTOR2 posA, float fRadiusA, D3DXVECTOR2 posB, float fRadiusB);
	static bool IsColiisionSpherAndSpher(D3DXVECTOR3 posA, float fRadiusA, D3DXVECTOR3 posB, float fRadiusB);
	static bool IsColiisionCylinderAndCylinder(D3DXVECTOR3 posA, float fRadiusA, float fHeightA, D3DXVECTOR3 posB, float fRadiusB, float fHeightB);
	static bool IsColiisionLectAndPoint(D3DXVECTOR2 posA, float fWidthA, float fHeightA, D3DXVECTOR2 posB);
	static int PickOutALetter(char* pOut, int nCharNum, const char* pStr);
	static char BitFlagRaiseAFlag(char cVariable,char cFlagValue);
	static char BitFlagLowerFlag(char cVariable, char cFlagValue);
	static bool BitFlagIsSet(char cVariable, char cFlagCheck);
	static float VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);
	static D3DXVECTOR3 MultiplyingAPosition(D3DXVECTOR3 pos, D3DXMATRIX mtx);
	static D3DXVECTOR3 MatrixMulRotation(D3DXVECTOR3 pos, D3DXMATRIX mtx);
	static void ReadToSpace(std::ifstream& ifs, std::string& str);

private:
};

//**********************************************************************************************************************
// クランプ せき止める処理
//**********************************************************************************************************************
template <class T> T Clamp(const T t, const T min, const T max)
{
	if (t > max) return max;
	if (t < min) return min;
	return t;
}
//**********************************************************************************************************************
// ラップ 繰り返す処理
//**********************************************************************************************************************
template <class T> T Wrap(const T t, const T min, const T max)
{
	if (t > max) return min;
	if (t < min) return max;
	return t;
}


#endif // !_MATH_H_
