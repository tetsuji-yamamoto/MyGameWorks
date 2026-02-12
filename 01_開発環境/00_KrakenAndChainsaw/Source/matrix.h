//****************************************************
// 
// 行列計算処理
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _MATRIX_H_
#define _MATRIX_H_

// ベクトル処理名前空間
namespace matrix
{
	
	D3DXMATRIX MatrixMultiply(const D3DXMATRIX mtxA, const D3DXMATRIX mtxB);
	D3DXMATRIX MatrixMulQuat(const D3DXMATRIX mtxA, const D3DXQUATERNION quat);
	D3DXMATRIX MatrixInverse(const D3DXMATRIX mtx);
	D3DXMATRIX MatrixQuatAxis(const D3DXVECTOR3 axis, const float fAngle);
	D3DXMATRIX MatrixRotationQuat(const D3DXQUATERNION quat);
	D3DXMATRIX MatrixYawPitchRoll(const D3DXVECTOR3 rot);
	D3DXMATRIX MatrixTranslation(const D3DXVECTOR3 pos);
	D3DXMATRIX MatrixWorld(const D3DXVECTOR3 pos, const D3DXQUATERNION quat);
	D3DXMATRIX MatrixWorld(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	D3DXMATRIX MatrixWorld(const D3DXVECTOR3 pos, const D3DXVECTOR3 axis, const float fAngle);
}

#endif