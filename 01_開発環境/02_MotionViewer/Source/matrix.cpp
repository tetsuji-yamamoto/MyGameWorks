//****************************************************
// 
// 行列計算処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#include "matrix.h"

//**************************************************
// 行列同士を掛ける計算
//**************************************************ssssss
D3DXMATRIX matrix::MatrixMultiply(const D3DXMATRIX mtxA, const D3DXMATRIX mtxB)
{
	D3DXMATRIX mtx;
	D3DXMatrixMultiply(&mtx, &mtxA, &mtxB);
	return mtx;
}

//**************************************************
// 行列同士を掛ける計算
//**************************************************
D3DXMATRIX matrix::MatrixMulQuat(const D3DXMATRIX mtxA, const D3DXQUATERNION quat)
{
	D3DXMATRIX mtx,mtxRot;
	D3DXMatrixRotationQuaternion(&mtxRot, &quat);
	D3DXMatrixMultiply(&mtx, &mtxA, &mtxRot);
	return mtx;
}

//**************************************************
// 逆行列を作る処理
//**************************************************
D3DXMATRIX matrix::MatrixInverse(const D3DXMATRIX mtx)
{
	D3DXMATRIX mtxorigin = mtx;
	D3DXMATRIX mtxInverse;
	D3DXMatrixInverse(&mtxInverse,nullptr, &mtxorigin);
	return mtxInverse;
}

//**************************************************
// クォータニオンから回転行列を作る処理
//**************************************************
D3DXMATRIX  matrix::MatrixQuatAxis(const D3DXVECTOR3 axis, const float fAngle)
{
	D3DXMATRIX mtxRot;
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, fAngle);
	D3DXMatrixRotationQuaternion(&mtxRot, &quat);
	return mtxRot;
}

//**************************************************
// クォータニオンから回転行列を作る処理
//**************************************************
D3DXMATRIX  matrix::MatrixRotationQuat(const D3DXQUATERNION quat)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationQuaternion(&mtxRot, &quat);
	return mtxRot;
}

//**************************************************
// 回転行列を作る処理
//**************************************************
D3DXMATRIX matrix::MatrixYawPitchRoll(const D3DXVECTOR3 rot)
{
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot,rot.y, rot.x, rot.z);
	return mtxRot;
}

//**************************************************
// 位置行列を作る処理
//**************************************************
D3DXMATRIX matrix::MatrixTranslation(const D3DXVECTOR3 pos)
{
	D3DXMATRIX mtxTrans;
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	return mtxTrans;
}

//**************************************************
// ワールド行列を作る処理
//**************************************************
D3DXMATRIX matrix::MatrixWorld(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	D3DXMATRIX mtxWorld = InitValu::MTX_NULL;
	D3DXMATRIX mtxRot, mtxTrans;
	mtxRot = MatrixYawPitchRoll(rot);
	mtxTrans = MatrixTranslation(pos);
	mtxWorld = MatrixMultiply(mtxWorld, mtxRot);
	mtxWorld = MatrixMultiply(mtxWorld, mtxTrans);
	return mtxWorld;
}

//**************************************************
// ワールド行列を作る処理
//**************************************************
D3DXMATRIX matrix::MatrixWorld(const D3DXVECTOR3 pos, const D3DXQUATERNION quat)
{
	D3DXMATRIX mtxWorld = InitValu::MTX_NULL;
	D3DXMATRIX mtxTrans,mtxRot;
	mtxTrans = MatrixTranslation(pos);
	mtxRot = MatrixRotationQuat(quat);
	mtxWorld = MatrixMultiply(mtxWorld,mtxRot);
	mtxWorld = MatrixMultiply(mtxWorld,mtxTrans);
	return mtxWorld;
}

//**************************************************
// ワールド行列を作る処理
//**************************************************
D3DXMATRIX matrix::MatrixWorld(const D3DXVECTOR3 pos, const D3DXVECTOR3 axis,const float fAngle)
{
	D3DXMATRIX mtxWorld = InitValu::MTX_NULL;
	D3DXMATRIX mtxRot, mtxTrans;
	mtxRot = MatrixQuatAxis(axis, fAngle);
	mtxTrans = MatrixTranslation(pos);
	mtxWorld = MatrixMultiply(mtxWorld, mtxRot);
	mtxWorld = MatrixMultiply(mtxWorld, mtxTrans);
	return mtxWorld;
}