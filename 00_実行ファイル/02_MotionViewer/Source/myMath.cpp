//**********************************************************************************************************************
// 
// 数学
// Author Tetsuji Yamamoto
// 
//**********************************************************************************************************************
#include "myMath.h"
#include "manager.h"
#include <locale.h>

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
// 数学球座標計算
//**********************************************************************************************************************
D3DXVECTOR3 CMyMath::SphericalCoordinates(D3DXVECTOR3 centerPos, D3DXVECTOR3 rot, float fDistance)
{
	D3DXVECTOR3 outPos;
	outPos.x = centerPos.x + sinf(rot.x) * sinf(rot.y) * fDistance;
	outPos.y = centerPos.y + cosf(rot.x) * fDistance;
	outPos.z = centerPos.z + sinf(rot.x) * cosf(rot.y) * fDistance;

	return outPos;
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
		&pos3D,		// 変換したいワールド座標
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
// 三角関数で点と点の長さを図る処理2D
//**************************************************
float CMyMath::LengthTrigonometric2D(D3DXVECTOR2 posA, D3DXVECTOR2 posB)
{
	float fX = posA.x - posB.x;
	float fY = posA.y - posB.y;
	return sqrtf((fX * fX) + (fY * fY));
}

//**************************************************
// 三角関数で点と点の長さを図る処理3D
//**************************************************
float CMyMath::LengthTrigonometric3D(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	float fX = posA.x - posB.x;
	float fY = posA.y - posB.y;
	float fZ = posA.z - posB.z;
	return sqrtf((fX * fX) + (fY * fY) + (fZ * fZ));
}

//**************************************************
// 円と円の当たり判定2D
//**************************************************
bool CMyMath::IsColiisionCircleAndCircle2D(D3DXVECTOR2 posA, float fRadiusA, D3DXVECTOR2 posB, float fRadiusB)
{
	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthTrigonometric2D(posA, posB);

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
bool CMyMath::IsColiisionSpherAndSpher(D3DXVECTOR3 posA, float fRadiusA, D3DXVECTOR3 posB, float fRadiusB)
{
	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthTrigonometric3D(posA, posB);

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
bool CMyMath::IsColiisionCylinderAndCylinder(D3DXVECTOR3 posA, float fRadiusA, float fHeightA, D3DXVECTOR3 posB, float fRadiusB, float fHeightB)
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
	float fLength = CMyMath::LengthTrigonometric2D(pos2D, pos2DPar);

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
	return acosf(fDot);
}

//**************************************************
// 位置にマトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 CMyMath::MultiplyingAPosition(D3DXVECTOR3 pos, D3DXMATRIX mtx)
{
	D3DXVECTOR3 posAns;

	posAns.x = (pos.x * mtx._11) + (pos.y * mtx._21) + (pos.z * mtx._31) + mtx._41;
	posAns.y = (pos.x * mtx._12) + (pos.y * mtx._22) + (pos.z * mtx._32) + mtx._42;
	posAns.z = (pos.x * mtx._13) + (pos.y * mtx._23) + (pos.z * mtx._33) + mtx._43;

	return posAns;
}

//**************************************************
// 位置にマトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 CMyMath::MatrixMulRotation(D3DXVECTOR3 pos, D3DXMATRIX mtx)
{
	D3DXVECTOR3 posAns;

	posAns.x = (pos.x * mtx._11) + (pos.y * mtx._21) + (pos.z * mtx._31);
	posAns.y = (pos.x * mtx._12) + (pos.y * mtx._22) + (pos.z * mtx._32);
	posAns.z = (pos.x * mtx._13) + (pos.y * mtx._23) + (pos.z * mtx._33);

	return posAns;
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
void CMyMath::SlerpQuaternion(D3DXQUATERNION* pOut, D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric)
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