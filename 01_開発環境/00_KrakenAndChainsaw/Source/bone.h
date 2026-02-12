//*********************************************
// 
// ボーン
// Author Tetsuji Yamamoto
// 
//*********************************************
#ifndef _BONE_H_
#define _BONE_H_

#include "main.h"
#include "object.h"
#include <vector>

// ボーン
class CBone
{
public:
	// ボーン構造体
	typedef struct BONE
	{
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		D3DXMATRIX mtxRot;		// 回転マトリックス
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 posOffset;	// オフセット位置
		D3DXQUATERNION quat;	// クォータニオン
		BONE* pBoneParent;		// 親ボーン
		BONE* pBoneChild;		// 子ボーン
		int nIdx;				// ボーンのインデックス
		int nIdxParent;			// 親ボーンへのインデックス
		int nIdxChild;			// 子ボーンへのインデックス
	}BONE;

	CBone() {}
	~CBone() {}

private:
};

#endif