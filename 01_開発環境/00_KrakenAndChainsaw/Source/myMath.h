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
	// ポリゴン情報
	typedef struct
	{
		D3DXVECTOR3 posPoly;	// ポリゴンの頂点
		D3DXMATRIX* pMtxMesh;	// マトリックスポインター
	}POLYGON;

	CMyMath();
	~CMyMath();

	static int SeekMeshFieldNumPolygonHid(int nDivisionY);
	static int SeekFieldNumPolygonApp(int nDivisionX, int nDivisionY);
	static int SeekFieldNumPolygonAll(int nDivisionX, int nDivisionY);
	static int SeekFieldNumVertexAll(int nDivisionX, int nDivisionY);
	static int SeekFieldNumIndexAll(int nNumPolygonAll);
	static int SeekFanNumIndexAll(int nDivisionX);
	static D3DXMATRIX ConvertLocalCoordinatesToRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 nor);
	static void NormalizingAngles(D3DXVECTOR3* pRot);
	static float ShortestAngle(float fAngleDiff);
	static float ShortestAngle(float AngleDest, float Angle);
	static D3DXVECTOR3 Posision3Dto2D(D3DXVECTOR3* pOutPos2D, D3DXVECTOR3 pos3D);
	static D3DXVECTOR3 Posision2Dto3D(D3DXVECTOR3* pOutPos3D, D3DXVECTOR3 pos2D);
	static float LengthPositions2D(D3DXVECTOR2 posA, D3DXVECTOR2 posB);
	static float LengthPositions3D(D3DXVECTOR3 posA, D3DXVECTOR3 posB);
	static int PickOutALetter(char* pOut, int nCharNum, const char* pStr);
	static char BitFlagRaiseAFlag(char cVariable, char cFlagValue);
	static char BitFlagLowerFlag(char cVariable, char cFlagValue);
	static bool BitFlagIsSet(char cVariable, char cFlagCheck);
	static float VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);
	static float VectorsAngle2D(D3DXVECTOR2 vecA, D3DXVECTOR2 vecB);
	static D3DXMATRIX D3DXMatrixMultiplyYawPitchRollTranslation(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static void BubbleSort(int* SortData, int nNum);
	static void BubbleSort(std::vector<int>& vrTarget);
	static void BubbleSortMin(int* SortData, int nNum);
	static D3DXVECTOR3 QuaternionToEuler(const D3DXQUATERNION& quat);
	static D3DXVECTOR3 MulRotationMatrix(D3DXVECTOR3 pos, D3DXMATRIX mtxRot);
	static D3DXVECTOR3 MulWorldMatrix(D3DXVECTOR3 pos, D3DXMATRIX mtxWorld);
	static D3DXMATRIX QuatnionToMatrix(D3DXVECTOR3 centralAxis, D3DXVECTOR3 clossAxis,D3DXVECTOR3 angleAxis);
	static D3DXMATRIX QuatnionToMatrix(D3DXVECTOR3 centralAxis, float fAngle);
	static float RandamDecimal(float fMax);
	static float RandamAngle(void);
	static float RandamDecimal(float fMax, float fMin);
	static float Sinf(float fAngle, float fLength);
	static float Cosf(float fAngle, float fLength);
	static D3DXVECTOR3 SphericalCoordinates(D3DXVECTOR3 posCenter, D3DXVECTOR3 rot, float fDistance);
	static bool CountDown(int* pData,int nMin);
	static void ReadToSpace(std::ifstream& ifs, std::string& str);
	static void QuatSlerp(D3DXQUATERNION* pOut, D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric);
	static float AreaTriangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC);
	static bool PolygonInsideOutside(D3DXVECTOR3 posInter, D3DXVECTOR3 posPolyA, D3DXVECTOR3 posPolyB, D3DXVECTOR3 posPolyC);
	static float ExtrusionPolygonNoSlip(D3DXVECTOR3* pOut, D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush);
	static D3DXVECTOR3 ExtrusionPolygonSlide(D3DXVECTOR3*pOut,D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush);
	static D3DXVECTOR3 PolygonCenter(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC);
	static bool ColiisionRectTopoint2D(D3DXVECTOR2 posPoint, D3DXVECTOR2 posRect, float fWidth, float fHeight);
	static bool ColiisionCircleAndCircle2D(D3DXVECTOR2 posA, float fRadiusA, D3DXVECTOR2 posB, float fRadiusB);
	static bool ColiisionSpherAndSpher(D3DXVECTOR3 posA, float fRadiusA, D3DXVECTOR3 posB, float fRadiusB);
	static bool ColiisionCylinderAndCylinder(D3DXVECTOR3 posA, float fRadiusA, float fHeightA, D3DXVECTOR3 posB, float fRadiusB, float fHeightB);
	static bool CollisionMesh(D3DXVECTOR3* pOutHitPos, D3DXVECTOR3* pOutNor, LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius = 10.0f);
	static bool CollisionSpherAndLine(D3DXVECTOR3* pOut,D3DXVECTOR3 posSpher,float fRadius, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd);
	static bool CollisionLineAndLine(D3DXVECTOR2* pOut, D3DXVECTOR2 posStartA, D3DXVECTOR2 posEndA, D3DXVECTOR2 posStartB, D3DXVECTOR2 posEndB);

private:
};

namespace my_math
{
	float	ClosestPtSegmentSegment	(D3DXVECTOR3 p1, D3DXVECTOR3 q1, D3DXVECTOR3 p2, D3DXVECTOR3 q2, float& s, float& t, D3DXVECTOR3& c1, D3DXVECTOR3& c2);
	float	Palam					(const int nValu, const int nMax);
	bool	CountClamp				(int& nCounter, const int nMin, const int nMax);
	bool	CountWrap				(int& nCounter, const int nMin, const int nMax);
	int		Decrement				(const int nValue,const int nMin);
	int		Increment				(const int nValue, const int nMax);
	float	Random					(float fMax);
	int		Random					(int nMax, int nMin);
	float	PlusMinus				(float fValue);
	float	Plus					(float fValue);
}

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

//**********************************************************************************************************************
// 最大確認処理
//**********************************************************************************************************************
template <class T> bool Max(const T t, const T max)
{
	if (t >= max) return true;
	return false;
}

//**********************************************************************************************************************
// 最小確認処理
//**********************************************************************************************************************
template <class T> bool Min(const T t, const T min)
{
	if (t <= min) return true;
	return false;
}
#endif // !_MATH_H_
