//****************************************************
// 
// 当たり判定処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "hit.h"
#include "vector.h"
#include "mymath.h"
#include "matrix.h"
#include "vector.h"

using namespace hit;
using namespace my_vector;
using namespace my_math;

//**************************************************
// 三角形の面積を求める処理処理
//**************************************************
float hit::AreaTriangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
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
bool hit::PolygonInsideOutside(float* pDiff,D3DXVECTOR3 posInter, D3DXVECTOR3 posPolyA, D3DXVECTOR3 posPolyB, D3DXVECTOR3 posPolyC)
{
	// バリセントリック座標で内外判定をする
	float fAreaOll = AreaTriangle(posPolyA, posPolyB, posPolyC);
	float fArea1 = AreaTriangle(posInter, posPolyB, posPolyC);
	float fArea2 = AreaTriangle(posPolyA, posInter, posPolyC);
	float fArea3 = AreaTriangle(posPolyA, posPolyB, posInter);
	float fAreaTry = fArea1 + fArea2 + fArea3;
	float fDIffArea = fAreaOll - fAreaTry;
	*pDiff = fDIffArea;

	// 三つの面積の合計がポリゴンの面積と同じ場合はポリゴンの内部にいる
	if (!(fabsf(fDIffArea) <= 0.1f))
	{
		// 入っていない
		return false;
	}

	// 入っている
	return true;
}

//**************************************************
// 球と線分の当たり判定
//**************************************************
bool hit::CollisionSpherAndLine(D3DXVECTOR3* pOut, D3DXVECTOR3 posSpher, float fRadius, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd)
{
	D3DXVECTOR3 vec = posEnd - posStart;
	D3DXVECTOR3 posT0 = InitValu::VEC3_NULL;
	D3DXVECTOR3 posT1 = InitValu::VEC3_NULL;

	// 二次方程式に使うabcを算出
	float fxa = posStart.x - posSpher.x;
	float fya = posStart.y - posSpher.y;
	float fza = posStart.z - posSpher.z;

	float fa = (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	float fb = 2.0f * ((vec.x * fxa) + (vec.y * fya) + (vec.z * fza));
	float fc = (fxa * fxa) + (fya * fya) + (fza * fza) - (fRadius * fRadius);

	// 二次方程式が解けない場合は衝突していないとみなす
	float fd = fb * fb - 4.0f * fa * fc;
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
// メッシュでの当たり判定処理
//**************************************************
bool hit::CollisionMeshAndSpher(D3DXVECTOR3* pOutHitPos, D3DXVECTOR3* pOutNor, LPD3DXMESH pMesh,D3DXMATRIX* pMtx, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius)
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
	*pOutNor = { 0.0f,1.0f,0.0f };

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
			if(pMtx != nullptr)
			{
				aPos[nCntTriAngle] =  my_vector::Vec3MulMtx(*pPosVtx, *pMtx);
			}
			else
			{
				// 位置ポインター設定
				apPos[nCntTriAngle] = pPosVtx;
				aPos[nCntTriAngle] = *pPosVtx;
			}
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

		if (!(fDotLengthPos <= fRadius && fDotLengthPosOld >= -fRadius * 0.5f))
		{
			continue;
		}

		bool bIn = false;
		// 表面で半径以下だったら
		/*if (fDotLengthPos <= fRadius && fabsf(fDotLengthPosOld) <= fRadius)
		{
			bIn = true;
		}*/

		/*if (fDotLengthPos <= fRadius + 30.0f && fDotLengthPosOld >= -fRadius - 30.0f)
		{
			bIn = true;
		}*/

		/*if (bIn == false)
		{
			continue;
		}*/

		// 交差点を計算する
		float fPalmDotLength = fDotLengthPos / (fDotLengthPos - fDotLengthPosOld);
		D3DXVECTOR3 posHit = pos - nor * fDotLengthPos;
		D3DXVECTOR3  posHitLine;

		// 当たっている場所の判別変数
		bool bCollEdge = false;
		bool bCollArea = false;

		// ポリゴンの辺に当たっているか調べる
		if (CollisionSpherAndLine(&posHitLine, posHit, fRadius, aPos[0], aPos[1]) == true)
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

		float fDiff = 0.0f;
		// ポリゴンの中か外にいるかを調べる
		if (PolygonInsideOutside(&fDiff,posHit, aPos[0], aPos[1], aPos[2]) == true)
		{
			bCollArea = true;
		}

		// どれにも当たっていなかった
		if (bCollArea == false && bCollEdge == false)
		{
			continue;
		}

		// 深さを図る
		float fDips = fDotLengthPos - fRadius;

		// より深ければ当たっている
		if (fDipsNow >= fDips)
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

//**************************************************************************
// モーションクラスメッシュの当たり判定(トリガー)
//**************************************************************************
bool hit::CollisionMeshAndSpherTrigger(D3DXVECTOR3* pOutHitPos, LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius)
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
		if (!(fDotLengthPos <= fRadius && fDotLengthPosOld >= -fRadius))
		{
			continue;
		}

		//// 表面で半径以下だったら
		//if (!(fDotLengthPos < fRadius && fDotLengthPosOld >= fRadius * 0.4f))
		//{
		//	continue;
		//}

		// 交差点を計算する
		float fPalmDotLength = fDotLengthPos / (fDotLengthPos - fDotLengthPosOld);
		D3DXVECTOR3 posHit = pos - nor * fDotLengthPos;
		D3DXVECTOR3  posHitLine;

		// 当たっている場所の判別変数
		bool bCollEdge = false;
		bool bCollArea = false;

		// ポリゴンの辺に当たっているか調べる
		if (CollisionSpherAndLine(&posHitLine, posHit, fRadius, aPos[0], aPos[1]) == true)
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

		float fDiff = 0.0f;
		// ポリゴンの中か外にいるかを調べる
		if (PolygonInsideOutside(&fDiff,posHit, aPos[0], aPos[1], aPos[2]) == true)
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

//**************************************************************************
// モーションクラスメッシュの当たり判定
//**************************************************************************
CHitResultMeshAndSpher hit::CollisionMeshAndSpher(const LPD3DXMESH& pMesh,D3DXMATRIX* pMtx, const D3DXVECTOR3& pos,const D3DXVECTOR3& posOld, float fRadius, float fMovement, float fAngleNorLimit)
{
	
	// 出力情報
	CHitResultMeshAndSpher hitresult;

	// 計算時に使う変数
	D3DXVECTOR3 posNew;
	D3DXVECTOR3 posHit;
	D3DXVECTOR3 nor;

	// 位置を代入
	hitresult.m_pos = pos;

	/*for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		bool bCollision = false;*/

		// メッシュとの当たり判定を取る
		if (CollisionMeshAndSpher(&posHit, &nor, pMesh, pMtx, hitresult.m_pos, posOld, fRadius) == true)
		{
			//bCollision = true;

			// 法線の角度を図る
			float fAngleNor = VectorAngleToRadian(D3DXVECTOR3(0.0f, -1.0f, 0.0f), nor);

			// 一定の角度を越したら滑る押し出し
			if (fAngleNor <= fAngleNorLimit)
			{
				// 法線方向へ半径分伸ばした位置
				posNew = hitresult.m_pos - nor * fRadius;

				// 押し出した分を取る
				D3DXVECTOR3 vecPush = ExtrusionPolygonSlide(&posNew, posHit, nor, posNew);

				// 元の位置に足す
				hitresult.m_pos += vecPush;
				hitresult.m_pos += nor * 0.1f;

				// 当たったことにする
				hitresult.m_bCollision = true;
			}
			// 滑らない押し出し
			else
			{
				// 法線方向へ半径分伸ばした位置
				posNew = hitresult.m_pos - nor * fRadius;

				// 押し出した分を取る
				float fPush = ExtrusionPolygonNoSlip(&posNew, posHit, nor, posNew);

				// 元の位置に足す
				hitresult.m_pos.y += fPush;
				hitresult.m_pos.y += 0.1f;

				// 当たったことにする
				hitresult.m_bCollision = true;

				// 乗ったことにする
				hitresult.m_bRanding = true;
			}

			// 移動させた方向と距離を計算
			D3DXVECTOR3 vecNH = posNew - posHit;
			float fLengthNNN = D3DXVec3Length(&vecNH);
			D3DXVec3Normalize(&vecNH, &vecNH);

			// 移動量を越して移動した抑える
			if (fLengthNNN >= fMovement)
			{
				hitresult.m_pos -= vecNH * (fLengthNNN - fMovement);
			}

			// 法線の設定
			hitresult.m_nor = nor;
		}
		else
		{
			int a = 0;
		}

	return hitresult;
}

//**************************************************
// 球とカプセルの当たり判定
//**************************************************
bool hit::CollisionSpherAndCapsuleTrigger(D3DXVECTOR3* pOut, D3DXVECTOR3 posSpher, float fRadius, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd)
{
	D3DXVECTOR3 vec = posEnd - posStart;
	D3DXVECTOR3 posT0 = InitValu::VEC3_NULL;
	D3DXVECTOR3 posT1 = InitValu::VEC3_NULL;

	// 二次方程式に使うabcを算出
	float fxa = posStart.x - posSpher.x;
	float fya = posStart.y - posSpher.y;
	float fza = posStart.z - posSpher.z;

	float fa = (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	float fb = 2.0f * ((vec.x * fxa) + (vec.y * fya) + (vec.z * fza));
	float fc = (fxa * fxa) + (fya * fya) + (fza * fza) - (fRadius * fRadius);

	// 二次方程式が解けない場合は衝突していないとみなす
	float fd = fb * fb - 4.0f * fa * fc;
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
// カプセルとカプセルの当たり判定
//**************************************************
bool hit::CollisionCapsuleAndCapsuleTrigger(D3DXVECTOR3& posHit,D3DXVECTOR3& vecHit, D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2,float fRadiusA, float fRadiusB)
{
	float fLength = 0.0f;
	float s = 0.0f;
	float t = 0.0f;
	D3DXVECTOR3 c1, c2;

	// カプセルとカプセルの距離を取る
	fLength = ClosestPtSegmentSegment(posA1, posA2, posB1, posB2, s, t, c1, c2);

	// 二つの半径を足した長さ
	float fRadiusAB = fRadiusA + fRadiusB;

	// 長さが半径以下だったら当たっている
	if (fLength <= fRadiusAB)
	{
		// 当たった位置と向き特定
		vecHit = c2 - c1;
		vecHit = Vec3Normalize(vecHit);
		posHit = vecHit * fRadiusA;
		posHit += c1;
		return true;
	}

	return false;
}