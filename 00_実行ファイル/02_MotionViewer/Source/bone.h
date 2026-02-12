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
class CBone : CObject
{
public:
	static constexpr float BONE_MODEL_HEIGHT = 0.5f;
	static constexpr float BONE_MODEL_RADIUS = 0.5f;

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

	CBone();
	~CBone();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBone* Create(void);

	void SetHeight(float fHeight);
	void SetRadius(float fRadius);
	void SetBack(D3DXVECTOR3 pos);
	void ResetVertex(void);
	void SetColor(D3DXCOLOR col);
	void SetMatrixRotate(D3DXMATRIX mtxRotate) { m_mtxRotate = mtxRotate; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	int GetID(void) { return m_nID; }
	D3DXCOLOR GetColor(void) { return m_col; }
	D3DXMATRIX GetMatrixRotate(void) { return m_mtxRotate; }
	static void SwitchIsDisp(void) { m_bDisp = !m_bDisp; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXMATRIX m_mtxRotate;				// 回転マトリックス
	D3DXCOLOR m_col;					// 色
	D3DXVECTOR3 m_pos;					// 位置
	int m_nID;							// 自身のID
	static int m_nNumOll;				// 全体数
	static bool m_bDisp;				// 映すか否か
};

#endif