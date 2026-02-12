//**********************************************************************************************************************
// 
// 数学
// Author Tetsuji Yamamoto
// 
//**********************************************************************************************************************
#include "myMath.h"
#include "manager.h"
#include "debugproc.h"
#include "vector.h"
#include "matrix.h"
#include <locale.h>

using namespace my_vector;
using namespace matrix;

//**********************************************************************************************************************
// 数学コンストラクタ
//**********************************************************************************************************************
CMyMath::CMyMath()
{
}

//**********************************************************************************************************************
// 数学デストラクタ
//**********************************************************************************************************************
CMyMath::~CMyMath()
{
}

//**********************************************************************************************************************
// 数学メッシュフィールドの縮退ポリゴンの数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekMeshFieldNumPolygonHid(int nDivisionY)
{
	return 4 * (nDivisionY - 1);
}

//**********************************************************************************************************************
// 数学メッシュフィールドの見えるポリゴンの数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumPolygonApp(int nDivisionX, int nDivisionY)
{
	return (nDivisionX * nDivisionY) * 2;
}

//**********************************************************************************************************************
// 数学メッシュフィールドのすべてのポリゴン数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumPolygonAll(int nDivisionX, int nDivisionY)
{
	return ((nDivisionX * nDivisionY) * 2) + (4 * (nDivisionY - 1));
}

//**********************************************************************************************************************
// 数学メッシュフィールドの頂点数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumVertexAll(int nDivisionX, int nDivisionY)
{
	return (nDivisionX + 1) * (nDivisionY + 1);
}

//**********************************************************************************************************************
// 数学メッシュフィールドのインデックス数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumIndexAll(int nNumPolygonAll)
{
	return nNumPolygonAll + 2;
}

//**********************************************************************************************************************
// 数学ファンのインデックス数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFanNumIndexAll(int nDivisionX)
{
	return nDivisionX + 3;
}

//************************************************************************************************************************
// ローカル座標から回転行列に変換する処理
//************************************************************************************************************************
D3DXMATRIX CMyMath::ConvertLocalCoordinatesToRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 nor)
{
	D3DXMATRIX mtxResult;	// 計算結果保存用変数
	D3DXVECTOR3 X, Y, Z;	// 計算したベクトル保存用変数

	Z = -(look - pos);										// 位置から見ている方向のベクトルを求める
	D3DXVec3Normalize(&Z, &Z);								// 標準化する
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, &nor), &Z);		// x軸を求める
	D3DXVec3Normalize(&X, &X);								// 標準化する
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));		// y軸を求める

	// 求めた値を回転行列に変換する
	mtxResult._11 = X.x; mtxResult._12 = X.y; mtxResult._13 = X.z; mtxResult._14 = 0;
	mtxResult._21 = Y.x; mtxResult._22 = Y.y; mtxResult._23 = Y.z; mtxResult._24 = 0;
	mtxResult._31 = Z.x; mtxResult._32 = Z.y; mtxResult._33 = Z.z; mtxResult._34 = 0;
	mtxResult._41 = 0.0f; mtxResult._42 = 0.0f; mtxResult._43 = 0.0f; mtxResult._44 = 1.0f;

	// 結果を返す
	return mtxResult;
}

//************************************************************************************************************************
// 角度の正規化
//************************************************************************************************************************
void CMyMath::NormalizingAngles(D3DXVECTOR3* pRot)
{
	// 角度の正規化
	if (pRot->x > D3DX_PI)
	{
		pRot->x = -D3DX_PI + (pRot->x - D3DX_PI);
	}
	else if (pRot->x < -D3DX_PI)
	{
		pRot->x = D3DX_PI + (pRot->x + D3DX_PI);
	}

	if (pRot->y > D3DX_PI)
	{
		pRot->y = -D3DX_PI + (pRot->y - D3DX_PI);
	}
	else if (pRot->y < -D3DX_PI)
	{
		pRot->y = D3DX_PI + (pRot->y + D3DX_PI);
	}

	if (pRot->z > D3DX_PI)
	{
		pRot->z = -D3DX_PI + (pRot->z - D3DX_PI);
	}
	else if (pRot->y < -D3DX_PI)
	{
		pRot->z = D3DX_PI + (pRot->z + D3DX_PI);
	}
}

//**************************************************
// 最短の角度に変換する処理
//**************************************************
float CMyMath::ShortestAngle(float fAngleDiff)
{
	// 差分が角度一周分を超えていたら差分から一周を引いて次に足せばいい角度を求める
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

//**************************************************
// 最短の角度に変換する処理
//**************************************************
float CMyMath::ShortestAngle(float AngleDest, float Angle)
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

//**************************************************
// 3D座標から2D座標に変換処理
//**************************************************
D3DXVECTOR3 CMyMath::Posision3Dto2D(D3DXVECTOR3* pOutPos2D, D3DXVECTOR3 pos3D)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXVECTOR3 PosScreen = {};		// 出力先
	D3DVIEWPORT9 viewport = {};		// ビューポート
	D3DXMATRIX projectionMtx = {};	// プロジェクションマトリックス
	D3DXMATRIX viewMtx = {};		// ビューマトリックス
	D3DXMATRIX worldMtx = {};		// ワールドマトリックス

	pDevice->GetViewport(&viewport);							// ビューポート取得
	pDevice->GetTransform(D3DTS_PROJECTION, &projectionMtx);	// プロジェクションマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &viewMtx);				// ビューマトリックス取得
	D3DXMatrixIdentity(&worldMtx);								// ワールドマトリックスの初期化

	// 変換する
	D3DXVec3Project(
		&PosScreen,		// 出力先のスクリーン座標
		&pos3D,			// 変換したいワールド座標
		&viewport,		// ビューポート情報
		&projectionMtx,	// プロジェクションマトリックス
		&viewMtx,		// ビューマトリックス
		&worldMtx		// ワールドマトリックス
	);

	// 結果を代入
	*pOutPos2D = PosScreen;

	// 結果を返す
	return PosScreen;
}

//**************************************************
// スクリーン座標から3D座標に変換処理
//**************************************************
D3DXVECTOR3 CMyMath::Posision2Dto3D(D3DXVECTOR3* pOutPos3D, D3DXVECTOR3 pos2D)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXVECTOR3 out;
	D3DVIEWPORT9 vp;
	D3DXMATRIX view;
	D3DXMATRIX proj;

	// ビューポート行列を作成
	D3DXMATRIX vpMat;
	D3DXMatrixIdentity(&vpMat);

	pDevice->GetViewport(&vp);						// ビューポート取得
	pDevice->GetTransform(D3DTS_VIEW, &view);		// ビュー行列取得
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);	// プロジェクションマトリックス取得

	vpMat._11 = (float)vp.Width / 2;
	vpMat._22 = -1.0f * (float)(vp.Height / 2);
	vpMat._33 = (float)vp.MaxZ - vp.MinZ;
	vpMat._41 = (float)(vp.X + vp.Width / 2);
	vpMat._42 = (float)(vp.Y + vp.Height / 2);
	vpMat._43 = vp.MinZ;

	// スクリーン位置をワールドへ
	out.x = pos2D.x;
	out.y = pos2D.y;
	out.z = pos2D.z;

	D3DXMATRIX invMat, inv_proj, inv_view;
	D3DXMatrixInverse(&invMat, 0, &vpMat);
	D3DXMatrixInverse(&inv_proj, 0, &proj);
	D3DXMatrixInverse(&inv_view, 0, &view);

	invMat *= inv_proj * inv_view;

	D3DXVec3TransformCoord(&out, &out, &invMat);

	*pOutPos3D = out;
	return out;
}

//**************************************************
// 三角関数で点と点の長さを図る処理2D
//**************************************************
float CMyMath::LengthPositions2D(D3DXVECTOR2 posA, D3DXVECTOR2 posB)
{
	float fX = posA.x - posB.x;
	float fY = posA.y - posB.y;
	return sqrtf((fX * fX) + (fY * fY));
}

//**************************************************
// 点と点の長さを図る処理3D
//**************************************************
float CMyMath::LengthPositions3D(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	float fX = posA.x - posB.x;
	float fY = posA.y - posB.y;
	float fZ = posA.z - posB.z;
	return sqrtf((fX * fX) + (fY * fY) + (fZ * fZ));
}

//**************************************************
// 円と円の当たり判定2D
//**************************************************
bool CMyMath::ColiisionRectTopoint2D(D3DXVECTOR2 posPoint, D3DXVECTOR2 posRect, float fWidth, float fHeight)
{
	/*
	自身の位置が相手の上辺より大きく
	自身の位置が相手の底辺より小さく
	自身の位置が相手の左辺より大きく
	自身の位置が相手の右辺より小さく
	 */
	if (posRect.y - fHeight < posPoint.y &&
		posRect.y + fHeight > posPoint.y &&
		posRect.x - fWidth < posPoint.x &&
		posRect.x + fWidth > posPoint.x
		)
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}

//**************************************************
// 円と円の当たり判定2D
//**************************************************
bool CMyMath::ColiisionCircleAndCircle2D(D3DXVECTOR2 posA, float fRadiusA, D3DXVECTOR2 posB, float fRadiusB)
{
	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthPositions2D(posA, posB);

	// 限界の長さより短ければ
	if (fLength <= fLimitLength)
	{
		// 当たっている判定
		return true;
	}

	// 当たっていない
	return false;
}

//**************************************************
// 球と球の当たり判定
//**************************************************
bool CMyMath::ColiisionSpherAndSpher(D3DXVECTOR3 posA, float fRadiusA, D3DXVECTOR3 posB, float fRadiusB)
{
	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthPositions3D(posA, posB);

	// 限界の長さより短ければ
	if (fLength <= fLimitLength)
	{
		// 当たっている判定
		return true;
	}

	// 当たっていない
	return false;
}

//**************************************************
// 円柱と円柱の当たり判定
//**************************************************
bool CMyMath::ColiisionCylinderAndCylinder(D3DXVECTOR3 posA, float fRadiusA, float fHeightA, D3DXVECTOR3 posB, float fRadiusB, float fHeightB)
{
	/*
			 |---|		|---|
			 |   |		|   |
		|---||   |		|   ||---|
		|   ||---|		|---||   |
		|   |				 |   |
		|---|				 |---|
		my   par		 my   par
	*/

	// 自身の底辺が相手の底辺より小さい、かつ自身の上辺が相手の底辺より高い
	if (posA.y <= posB.y && posA.y + fHeightA >= posB.y)
	{

	}
	// 自身の底辺が相手の底辺より大きい、かつ自身の底辺が相手の上辺より低い
	else if (posA.y >= posB.y && posA.y <= posB.y + fHeightB)
	{

	}
	// どれにも当てはまらない
	else
	{
		return false;
	}

	// D3DXVECTOR3からD3DXVECTOR2へ移し替える
	D3DXVECTOR2 pos2D, pos2DPar;
	pos2D.x = posA.x;
	pos2D.y = posA.z;
	pos2DPar.x = posB.x;
	pos2DPar.y = posB.z;

	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthPositions2D(pos2D, pos2DPar);

	// 限界の長さより短ければ当たっている判定
	if (fLength <= fLimitLength)
	{
		return true;
	}

	// 当たっていない判定
	return false;
}

//**************************************************
// 一文字抜き取る処理
//**************************************************
int CMyMath::PickOutALetter(char* pOut, int nCharNum, const char* pStr)
{
	// マルチバイト文字系関数を使用するための設定
	setlocale(LC_ALL, "");

	// 文字列の長さを調べる
	int nLength = strlen(pStr);

	// 計算用変数
	int nData = 0;
	char cDataA = 0;

	// 文字を生成
	for (int nCntLen = 0; nCntLen < nLength; nCntLen++)
	{
		cDataA = pStr[nCntLen];

		// 目的の文字に達していなければ処理しない
		if (nCntLen < nCharNum)
		{
			continue;
		}

		// データがマイナス、1バイト目ならばもう一度検査
		if (cDataA < 0 && nData <= 0)
		{
			pOut[nData] = cDataA;
			nData++;
			continue;
		}
		// 2バイト目ならば処理を終わる
		else
		{
			pOut[nData] = cDataA;
			return nCntLen;
		}

		nData = 0;
	}

	return 0;
}

//**************************************************b
// フラグを立てる処理
//**************************************************
char CMyMath::BitFlagRaiseAFlag(char cVariable, char cFlagValue)
{
	return cVariable | cFlagValue;
}

//**************************************************
// フラグを下げる処理
//**************************************************
char CMyMath::BitFlagLowerFlag(char cVariable, char cFlagValue)
{
	return (cVariable & cFlagValue) ? cVariable ^ cFlagValue : cVariable;
}

//**************************************************
// フラグが立っているか確認する
//**************************************************
bool CMyMath::BitFlagIsSet(char cVariable, char cFlagCheck)
{
	return (cVariable & cFlagCheck) ? true : false;
}

//**************************************************
// ベクトルのなす角を求める処理(ラジアン角)
//**************************************************
float CMyMath::VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB)
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
float CMyMath::VectorsAngle2D(D3DXVECTOR2 vecA, D3DXVECTOR2 vecB)
{
	D3DXVECTOR2 vecAB = vecA - vecB;
	return atan2f(vecAB.x, vecAB.y);
}

//**************************************************
// 位置と角度からワールドマトリックスを作る処理
//**************************************************
D3DXMATRIX CMyMath::D3DXMatrixMultiplyYawPitchRollTranslation(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxWorld;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	return mtxWorld;
}

//**********************************************************************************************************************
// バブルソート処理
//**********************************************************************************************************************
void CMyMath::BubbleSort(int* SortData, int nNum)
{
	// バブルソートで並べなおす
	for (int nCntRankA = 0; nCntRankA < nNum; nCntRankA++)
	{
		for (int nCntRankB = 0; nCntRankB < nNum - nCntRankA; nCntRankB++)
		{
			// 数字を比較大きければ入れ替える
			int nNumA = SortData[nCntRankB];
			int nNumB = SortData[nCntRankB + 1];
			if (nNumA < nNumB)
			{
				SortData[nCntRankB + 1] = nNumA;
				SortData[nCntRankB] = nNumB;
			}
		}
	}
}

//**********************************************************************************************************************
// バブルソート処理
//**********************************************************************************************************************
void CMyMath::BubbleSort(std::vector<int>& vrTarget)
{
	// バブルソートで並べなおす
	for (int nCntRankA = 0; nCntRankA < (int)vrTarget.size(); nCntRankA++)
	{
		for (int nCntRankB = 0; nCntRankB < (int)vrTarget.size() - nCntRankA - 1; nCntRankB++)
		{
			// 数字を比較大きければ入れ替える
			auto iterNumA = vrTarget.begin() + nCntRankB;
			auto iterNumB = vrTarget.begin() + nCntRankB + 1;
			if ((*iterNumA) < (*iterNumB))
			{
				int nData = (*iterNumB);
				(*iterNumB) = (*iterNumA);
				(*iterNumA) = nData;
			}
		}
	}
}

//**********************************************************************************************************************
// バブルソート処理
//**********************************************************************************************************************
void CMyMath::BubbleSortMin(int* SortData, int nNum)
{
	// バブルソートで並べなおす
	for (int nCntRankA = 0; nCntRankA < nNum; nCntRankA++)
	{
		for (int nCntRankB = 0; nCntRankB < nNum - nCntRankA; nCntRankB++)
		{
			// 数字を比較小さければ入れ替える
			int nNumA = SortData[nCntRankB];
			int nNumB = SortData[nCntRankB + 1];
			if (nNumA > nNumB)
			{
				SortData[nCntRankB + 1] = nNumA;
				SortData[nCntRankB] = nNumB;
			}
		}
	}
}

//**************************************************
// クォータニオンからオイラー角への変換
//**************************************************
D3DXVECTOR3 CMyMath::QuaternionToEuler(const D3DXQUATERNION& quat)
{
	// オイラー角
	D3DXVECTOR3 euler;

	// X軸
	float sinr_cosp = 2.0f * (quat.w * quat.x + quat.y * quat.z);
	float cosr_cosp = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
	euler.x = (float)atan2(sinr_cosp, cosr_cosp);

	// Y軸
	float sinp = 2.0f * (quat.w * quat.y - quat.z * quat.x);
	if (fabs(sinp) >= 1)
	{
		euler.y = (float)copysign(D3DX_PI / 2.0f, sinp); // 90度にクランプ
	}
	else
	{
		euler.y = (float)asin(sinp);
	}

	// Z軸
	float siny_cosp = 2.0f * (quat.w * quat.z + quat.x * quat.y);
	float cosy_cosp = 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);
	euler.z = (float)atan2(siny_cosp, cosy_cosp);

	return euler;
}

//**************************************************
// 位置に回転マトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 CMyMath::MulRotationMatrix(D3DXVECTOR3 pos,D3DXMATRIX mtxRot)
{
	D3DXVECTOR3 posOut;
	posOut.x = (pos.x * mtxRot._11) + (pos.y * mtxRot._21) + (pos.z * mtxRot._31);
	posOut.y = (pos.x * mtxRot._12) + (pos.y * mtxRot._22) + (pos.z * mtxRot._32);
	posOut.z = (pos.x * mtxRot._13) + (pos.y * mtxRot._23) + (pos.z * mtxRot._33);
	return posOut;
}

//**************************************************
// 位置に回転マトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 CMyMath::MulWorldMatrix(D3DXVECTOR3 pos, D3DXMATRIX mtxWorld)
{
	D3DXVECTOR3 posOut;
	posOut.x = (pos.x * mtxWorld._11) + (pos.y * mtxWorld._21) + (pos.z * mtxWorld._31) + mtxWorld._41;
	posOut.y = (pos.x * mtxWorld._12) + (pos.y * mtxWorld._22) + (pos.z * mtxWorld._32) + mtxWorld._42;
	posOut.z = (pos.x * mtxWorld._13) + (pos.y * mtxWorld._23) + (pos.z * mtxWorld._33) + mtxWorld._43;
	return posOut;
}

//**************************************************
// クォータニオンで回転行列を求める処理
//**************************************************
D3DXMATRIX CMyMath::QuatnionToMatrix(D3DXVECTOR3 centralAxis, D3DXVECTOR3 clossAxis, D3DXVECTOR3 angleAxis)
{
	D3DXMATRIX mtxRot;
	D3DXQUATERNION quat;
	D3DXVECTOR3 vecRotationAxis;
	float fAngle;

	fAngle = CMyMath::VectorAngleToRadian(centralAxis, angleAxis);
	D3DXVec3Cross(&vecRotationAxis,&centralAxis, &clossAxis);
	D3DXQuaternionRotationAxis(&quat, &vecRotationAxis,fAngle);
	D3DXMatrixRotationQuaternion(&mtxRot ,&quat);
	return mtxRot;
}

//**************************************************
// クォータニオンで回転行列を求める処理
//**************************************************
D3DXMATRIX CMyMath::QuatnionToMatrix(D3DXVECTOR3 centralAxis,float fAngle)
{
	D3DXMATRIX mtxRot;
	D3DXQUATERNION quat;
	D3DXVECTOR3 vecRotationAxis;
	D3DXQuaternionRotationAxis(&quat, &vecRotationAxis, fAngle);
	D3DXMatrixRotationQuaternion(&mtxRot, &quat);
	return mtxRot;
}

//**************************************************
// ランダムで小数点を出す処理
//**************************************************
float CMyMath::RandamDecimal(float fMax)
{
	int nMax = (int)fMax;
	return (float)(rand() % nMax);
}

//**************************************************
// ランダムで小数点を出す処理
//**************************************************
float CMyMath::RandamDecimal(float fMax, float fMin)
{
	int nMax = (int)(fMax * 1000.0f);
	int nMin = (int)(fMin * 1000.0f);
	return (float)(rand() % nMax + nMin) * 0.001f;
}

//**************************************************
// ランダムで角度を出す処理
//**************************************************
float CMyMath::RandamAngle()
{
	return (float)(rand() % 628 - 314) * 0.01f;
}

//**************************************************
// sinfで値を求める関数
//**************************************************
float CMyMath::Sinf(float fAngle,float fLength)
{
	return sinf(fAngle) * fLength;
}

//**************************************************
// cosfで値を求める関数
//**************************************************
float CMyMath::Cosf(float fAngle, float fLength)
{
	return cosf(fAngle) * fLength;
}

//**************************************************
// 球座標で位置を求める関数 
//**************************************************
D3DXVECTOR3 CMyMath::SphericalCoordinates(D3DXVECTOR3 posCenter,D3DXVECTOR3 rot,float fDistance)
{
	D3DXVECTOR3 pos;
	pos.x = posCenter.x + sinf(rot.x) * sinf(rot.y) * fDistance;
	pos.y = posCenter.y + cosf(rot.x) * fDistance;
	pos.z = posCenter.z + sinf(rot.x) * cosf(rot.y) * fDistance;

	return pos;
}

//**************************************************
// カウントダウン処理 
//**************************************************
bool CMyMath::CountDown(int* pData, int nMin)
{
	// 値を減らす
	*pData -= 1;

	// 最小値の値になったら止める
	if (*pData <= nMin)
	{
		*pData = nMin;
		return true;
	}

	return false;
}

//**************************************************
// 文字列を空白まで読み取る処理
//**************************************************
void CMyMath::ReadToSpace(std::ifstream& ifs, std::string& str)
{
	// 一文字保存用
	char cData;

	// 最初に読み取った空白
	bool bFarstSpace = false;

	// 文字列を削除
	str.clear();

	// 一文字ずつ読み取る
	while (ifs.get(cData))
	{
		// 空白だったら
		if (cData == ' ')
		{
			if (bFarstSpace == false)
			{
				bFarstSpace = true;
			}
			else
			{
				return;
			}
		}
		// 改行だったら
		else if (cData == '\n')
		{
			return;
		}
		// タブ文字だったら
		else if (cData == '\t')
		{
			return;
		}
		// キャリッジリターンだったら
		else if (cData == '\r')
		{
			return;
		}
		// 当てはまらないので文字列追加
		else
		{
			// 文字列追加
			str += cData;
		}

		// スペースの読み取りを不可にする
		bFarstSpace = true;
	}
}

//**************************************************
// クォータニオンの球面線形補間処理
//**************************************************
void CMyMath::QuatSlerp(D3DXQUATERNION* pOut, D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric)
{
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
		*pOut = quatNow + (quatNext - quatNow) * fParametric;
		return;
	}

	float fSinA = sinf(fCosTheta * (1.0f - fParametric));
	float fSinB = sinf(fCosTheta * fParametric);

	float fSinMugA = fSinA / fSin;
	float fSinMugB = fSinB / fSin;

	pOut->x = quatNow.x * fSinMugA + quatNext.x * fSinMugB;
	pOut->y = quatNow.y * fSinMugA + quatNext.y * fSinMugB;
	pOut->z = quatNow.z * fSinMugA + quatNext.z * fSinMugB;
	pOut->w = quatNow.w * fSinMugA + quatNext.w * fSinMugB;
}

//**************************************************
// メッシュでの当たり判定処理
//**************************************************
bool CMyMath::CollisionMesh(D3DXVECTOR3* pOutHitPos, D3DXVECTOR3* pOutNor, LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius)
{
	// 頂点フォーマットのサイズを取得
	WORD sizeFVF = (WORD)D3DXGetFVFVertexSize(pMesh->GetFVF());

	// メッシュの頂点数の取得
	int nNumVtx = pMesh->GetNumVertices();
	
	// インデックスバッファ取得
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;
	pMesh->GetIndexBuffer(&pIdxBuff);
	
	// インデックスバッファの情報取得
	D3DINDEXBUFFER_DESC descIdx;
	pIdxBuff->GetDesc(&descIdx);

	// インデックス数を割り出す
	int nIdxNum = 0;
	if (descIdx.Format == D3DFMT_INDEX16) {
		nIdxNum = (descIdx.Size / sizeof(WORD)); // 16ビットインデックスの場合
	}
	else if (descIdx.Format == D3DFMT_INDEX32) {
		nIdxNum = (descIdx.Size / sizeof(DWORD)); // 32ビットインデックスの場合
	}

	// 頂点バッファのロック
	BYTE* pVtxBuff = NULL;
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// インデックスバッファのロック
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdxBuff);
	WORD* pIdx = (WORD*)pIdxBuff;

	// 返す情報をリセット
	*pOutHitPos = InitValu::VEC3_NULL;
	*pOutNor = {0.0f,1.0f,0.0f};

	// 当たっていないことにする
	bool bCollision = false;

	// 現在深さゼロ
	float fDipsNow = 0.0f;

	// モデルの頂点数分回す
	for (int nCntIdx = 0; nCntIdx < nIdxNum;)
	{
		D3DXVECTOR3 aPos[3];			// ポリゴンの頂点座標
		D3DXVECTOR3* apPos[3];			// ポリゴンの頂点座標
		D3DXVECTOR3 nor;				// ポリゴンの法線
		D3DXVECTOR3 aVecPolyLine[3];	// ポリゴンの一辺のベクトル
		D3DXVECTOR3 aVecLinePos[3];		// posとのベクトル
		D3DXVECTOR3 aVecLinePosOld[3];	// posOldとのベクトル

		// 三回回す
		for (int nCntTriAngle = 0; nCntTriAngle < 3; nCntTriAngle++, nCntIdx++)
		{
			// インデックス取得
			int nIdx = pIdx[nCntIdx];

			// 頂点座標の代入
			D3DXVECTOR3* pPosVtx = (D3DXVECTOR3*)(pVtxBuff + sizeFVF * nIdx);

			// 座標にマトリックスを掛けて代入
			//aPos[nCntTriAngle] = MulWorldMatrix(*pPosVtx, mtxMesh);

			// 位置ポインター設定
			apPos[nCntTriAngle] = pPosVtx;
			aPos[nCntTriAngle] = *pPosVtx;
		}

		// ポリゴンの一辺のベクトルを求める
		aVecPolyLine[0] = aPos[1] - aPos[0];
		aVecPolyLine[1] = aPos[2] - aPos[1];
		aVecPolyLine[2] = aPos[0] - aPos[2];

		// 外積で法線を求める
		D3DXVec3Cross(&nor, &aVecPolyLine[2], &aVecPolyLine[0]);
		D3DXVec3Normalize(&nor, &nor);

		// 三回回す
		for (int nCntTriAngle = 0; nCntTriAngle < 3; nCntTriAngle++)
		{
			// pos,posOldベクトルを求める
			aVecLinePos[nCntTriAngle] = pos - aPos[nCntTriAngle];
			aVecLinePosOld[nCntTriAngle] = posOld - aPos[nCntTriAngle];
		}

		// 位置と古い位置のポリゴンまでの距離を求める
		D3DXVECTOR3 vecMove = posOld - pos;
		float fLengthMove = D3DXVec3Length(&vecMove);
		float fDotLengthPos = D3DXVec3Dot(&nor, &aVecLinePos[0]);
		float fDotLengthPosOld = D3DXVec3Dot(&nor, &aVecLinePosOld[0]);
		
		// 表面で半径以下だったら
		if (!(fDotLengthPos < fRadius && fDotLengthPosOld >= fRadius * 0.4f))
		{
			continue;
		}

		// 交差点を計算する
		float fPalmDotLength = fDotLengthPos / (fDotLengthPos - fDotLengthPosOld);
		D3DXVECTOR3 posHit = pos - nor * fDotLengthPos;
		D3DXVECTOR3  posHitLine;

		// 当たっている場所の判別変数
		bool bCollEdge = false;
		bool bCollArea = false;

		// ポリゴンの辺に当たっているか調べる
		if (CollisionSpherAndLine(&posHitLine,posHit, fRadius, aPos[0], aPos[1]) == true)
		{
			bCollEdge = true;
		}
		if (CollisionSpherAndLine(&posHitLine, posHit, fRadius, aPos[1], aPos[2]) == true)
		{
			bCollEdge = true;
		}
		if (CollisionSpherAndLine(&posHitLine, posHit, fRadius, aPos[2], aPos[0]) == true)
		{
			bCollEdge = true;
		}

		// ポリゴンの中か外にいるかを調べる
		if (PolygonInsideOutside(posHit, aPos[0], aPos[1], aPos[2]) == true)
		{
			bCollArea = true;
		}

		// どれにも当たっていなかった
		if (bCollArea == false && bCollEdge == false)
		{
			continue;
		}

		// 深さを図る
		float fDips = fRadius - fDotLengthPos;

		// より深ければ当たっている
		if (fDipsNow <= fDips)
		{
			// 深さを映す
			fDipsNow = fDips;

			// 出力先の値を設定
			*pOutHitPos = posHit;
			*pOutNor = nor;
			*pOutHitPos = posHit;

			// 当たったことにする
			bCollision = true;
		}
	}

	// 頂点バッファのアンロック
	pMesh->UnlockVertexBuffer();

	// インデックスバッファのアンロック
	pMesh->UnlockIndexBuffer();

	return bCollision;
}

//**************************************************
// 三角形の面積を求める処理処理
//**************************************************
float CMyMath::AreaTriangle(D3DXVECTOR3 posA,D3DXVECTOR3 posB,D3DXVECTOR3 posC)
{
	// 外積を2で割る
	D3DXVECTOR3 AB = posB - posA;
	D3DXVECTOR3 AC = posC - posA;
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &AB, &AC);
	return 0.5f * D3DXVec3Length(&cross);
}

//**************************************************
// ポリゴン内外判定
//**************************************************
bool CMyMath::PolygonInsideOutside(D3DXVECTOR3 posInter, D3DXVECTOR3 posPolyA, D3DXVECTOR3 posPolyB, D3DXVECTOR3 posPolyC)
{
	// バリセントリック座標で内外判定をする
	float fAreaOll = AreaTriangle(posPolyA, posPolyB, posPolyC);
	float fArea1 = AreaTriangle(posInter, posPolyB, posPolyC);
	float fArea2 = AreaTriangle(posPolyA, posInter, posPolyC);
	float fArea3 = AreaTriangle(posPolyA, posPolyB, posInter);
	float fAreaTry = fArea1 + fArea2 + fArea3;
	float fDIffArea = fAreaOll - fAreaTry;

	// 三つの面積の合計がポリゴンの面積と同じ場合はポリゴンの内部にいる
	if (!(fabsf (fAreaOll - fAreaTry) <= 0.1f))
	{
		// 入っていない
		return false;
	}

	// 入っている
	return true;
}

//**************************************************
// ポリゴンの押し出し処理(滑らない)
//**************************************************
float CMyMath::ExtrusionPolygonNoSlip(D3DXVECTOR3* pOut,D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush)
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
D3DXVECTOR3 CMyMath::ExtrusionPolygonSlide(D3DXVECTOR3* pOut, D3DXVECTOR3 posPolygon, D3DXVECTOR3 nor, D3DXVECTOR3 posPush)
{
	D3DXVECTOR3 vecLinePos = posPush - posPolygon;	// ポリゴンの頂点から押し出す位置までのベクトル

	// ポリゴンから押し出す位置までの長さを求める
	float fDotLengthPos = D3DXVec3Dot(&nor, &vecLinePos);

	// 法線方向に押し出す
	D3DXVECTOR3 vecPush = nor * fDotLengthPos;
	D3DXVECTOR3 posNewP = posPush - vecPush;
	
	// 押し出した分を求める
	D3DXVECTOR3 vecPos = posNewP - posPush;

	// 押し出す
	*pOut = posPush + vecPos;

	// 押し出した分を返す
	return vecPos;
}

//**************************************************
// ポリゴンの中心点を求める処理
//**************************************************
D3DXVECTOR3 CMyMath::PolygonCenter(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	// 平均値を取る
	D3DXVECTOR3 center = InitValu::VEC3_NULL;
	center += posA;
	center += posB;
	center += posC;
	center /= 3.0f;
	return center;
}

//**************************************************
// 球と線分の当たり判定
//**************************************************
bool CMyMath::CollisionSpherAndLine(D3DXVECTOR3* pOut,D3DXVECTOR3 posSpher, float fRadius, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd)
{
	D3DXVECTOR3 vec = posEnd - posStart;
	D3DXVECTOR3 posT0 = InitValu::VEC3_NULL;
	D3DXVECTOR3 posT1 = InitValu::VEC3_NULL;

	// 二次方程式に使うabcを算出
	float fxa = posStart.x - posSpher.x;
	float fya = posStart.y - posSpher.y;
	float fza = posStart.z - posSpher.z;

	float fa = (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	float fb = 2.0f * ((vec.x* fxa) + (vec.y * fya) + (vec.z * fza));
	float fc = (fxa * fxa) + (fya * fya) + (fza * fza) - (fRadius * fRadius);

	// 二次方程式が解けない場合は衝突していないとみなす
	float fd = fb * fb -4.0f * fa * fc;
	if (fd < 0.0f)
	{
		return false;
	}

	// 解の方程式を解く
	fd = sqrtf(fd);
	float ft0 = (-fb + fd) / (2.0f * fa);
	float ft1 = (-fb - fd) / (2.0f * fa);

	// 線分内の最寄りのtを探る
	float ft = 2.0f;
	if ((ft0 >= 0.0f) && (ft0 <= 1.0f) && (ft0 < ft))
	{
		ft = ft0;
		posT0 = posStart + vec * ft0;
		*pOut = posT0;
	}
	if ((ft1 >= 0.0f) && (ft1 <= 1.0f) && (ft1 < ft))
	{
		ft = ft1;
		posT1 = posStart + vec * ft1;
		D3DXVECTOR3 vecHit = posT1 - posT0;
		*pOut = posT0 + vecHit * 0.5f;
	}
	if (ft > 1.0f)
	{
		// 線分内にはいない
		return false;
	}

	return true;

}

//**************************************************
// 線分と線分の当たり判定
//**************************************************
bool CMyMath::CollisionLineAndLine(D3DXVECTOR2* pOut, D3DXVECTOR2 posStartA, D3DXVECTOR2 posEndA, D3DXVECTOR2 posStartB, D3DXVECTOR2 posEndB)
{
	*pOut = { 0.0f,0.0f };
	D3DXVECTOR2 vecA = posEndA - posStartA;
	D3DXVECTOR2 vecB = posEndB - posStartB;

	// 外積を取る
	float fCross = vecA.x* vecB.y - vecA.y * vecB.x;
	
	// ゼロであれば平行、又は同一直線
	if (fabsf(fCross) <= FLT_EPSILON)
	{
		return false;
	}

	// 距離係数を求める
	D3DXVECTOR2 diff = posStartB - posStartA;
	float fT = (diff.x * vecB.y - diff.y * vecB.x) / fCross;
	float fU = (diff.x * vecA.y - diff.y * vecA.x) / fCross;

	// 線分の範囲外である
	if (fT < 0.0f || fT > 1.0f || fU < 0.0f || fU > 1.0f)
	{
		return false;
	}

	// 交点の算出
	D3DXVECTOR2 intersection = posStartA + vecA * fT;
	*pOut = intersection;

	return true;
}

//**************************************************
// カプセルとカプセルの最近点の距離を取得する処理
//**************************************************
float my_math::ClosestPtSegmentSegment(D3DXVECTOR3 p1, D3DXVECTOR3 q1, D3DXVECTOR3 p2, D3DXVECTOR3 q2,float& s, float& t, D3DXVECTOR3& c1, D3DXVECTOR3& c2)
{
	D3DXVECTOR3 d1 = q1 - p1;	// 線分S1の方向ベクトル
	D3DXVECTOR3 d2 = q2 - p2;	// 線分S2の方向ベクトル
	D3DXVECTOR3 r = p1 - p2;
	float a = Vec3Dot(d1, d1);	// 線分S1の距離の平行,常に非負
	float e = Vec3Dot(d2, d2);	// 線分S1の距離の平行,常に非負
	float f = Vec3Dot(d2, r);

	// 片方あるいは両方の線分が点に縮退しているかチェック
	if (a <= FLT_EPSILON && e <= FLT_EPSILON)
	{
		// 両方の線分が点に縮退
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return Vec3Dot(c1 - c2, c1 - c2);
	}

	if (a <= FLT_EPSILON)
	{
		// 最初の線分が点に縮退
		s = 0.0f;
		t = f / e;// s = 0 => t = (b*s + f) / e = f / e
		t = Clamp(t, 0.0f, 1.0f);
	}
	else
	{
		float c = Vec3Dot(d1, r);
		if (e <= FLT_EPSILON)
		{
			// 二番目の線分が点に縮退
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			// ここから一般的な縮退の場合を開始
			float b = Vec3Dot(d1, d2);
			float denom = a * e - b * b;// 常に非負
			// 線分が平行でない場合、L1上のL2に対する歳最接近点を計算、そして
			// 線分S1に対してクランプ。そうでない場合は任意s(ここでは0)を選択
			if (denom != 0.0f)
			{
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				s = 0.0f;
			}
			// L2上のS1(s)に対する最近接点を以下を用いて計算
			// t = Dot((p1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
			t = (b * s + f) / e;
			// tが[0,1]の中にあれば終了。そうでなければtをクランプ,sをtの新しい値に対して以下を用いて再計算
			// s = Dot((P2+D2*t) - P1,D1) / Dot(D1,D1) = (t*b - c) / a
			// そしてsを[0,1]に対してクランプ
			if (t < 0.0f)
			{
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f)
			{
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	return sqrtf(Vec3Dot(c1 - c2, c1 - c2));
}

//**************************************************
// 倍率を計算する処理
//**************************************************
float my_math::Palam(const int nValu, const int nMax)
{
	if (nMax == 0)return 0.0f;
	return (float)nValu / (float)nMax;
}

//**************************************************
// カウント処理
//**************************************************
bool my_math::CountClamp(int& nCounter, const int nMin, const int nMax)
{
	// カウンターインクリメント
	nCounter++;

	// 最大か否かを調べる
	bool bMax = Max(nCounter, nMax);

	// カウンターの繰り返し
	if (nCounter >= nMax)nCounter = nMax;

	// 最大か否かを返す
	return bMax;
}

//**************************************************
// カウント処理
//**************************************************
bool my_math::CountWrap(int& nCounter, const int nMin, const int nMax)
{
	// カウンターインクリメント
	nCounter++;

	// 最大か否かを調べる
	bool bMax = Max(nCounter, nMax);

	// カウンターの繰り返し
	if (nCounter >= nMax)nCounter = nMin;

	// 最大か否かを返す
	return bMax;
}

//**************************************************
// デクリメント処理
//**************************************************
int my_math::Decrement(const int nValue, const int nMin)
{
	// デクリメントした値を設定
	int nOut = nValue - 1;

	// クランプする
	nOut = Clamp(nOut, nMin, nOut);

	// 値を返す
	return nOut;
}

//**************************************************
// インクリメント処理
//**************************************************
int my_math::Increment(const int nValue, const int nMax)
{
	// インクリメントした値を設定
	int nOut = nValue + 1;

	// クランプする
	nOut = Clamp(nOut, 0, nMax);

	// 値を返す
	return nOut;
}

//**************************************************
// ランダム処理
//**************************************************
float my_math::Random(float fMax)
{
	return (float)(rand() % (int)(fMax * 100.0f)) * 0.001f;
}

//**************************************************
// ランダム処理
//**************************************************
int my_math::Random(int nMax,int nMin)
{
	return rand() % nMax + nMin;
}

//**********************************************************************************************************************
// プラスマイナス生成処理
//**********************************************************************************************************************
float my_math::PlusMinus(float fValue)
{
	if (fValue == 0.0f)
	{
		return 1.0f;
	}

	float fValuePlus = sqrtf(fValue * fValue);
	return fValue / fValuePlus;
}

//**********************************************************************************************************************
// プラス処理
//**********************************************************************************************************************
float my_math::Plus(float fValue)
{
	return sqrtf(fValue * fValue);
}