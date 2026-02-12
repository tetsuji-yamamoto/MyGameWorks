//************************************************************
// 
//	スキンメッシュ用オブジェクト
// Author Tetsuji Yamamoto
// 
//************************************************************
#ifndef _OBJECTSKIN_H_
#define _OBJECTSKIN_H_

#include "main.h"
#include "object.h"
#include <vector>

class CModel;

// オブジェクトスキンクラス
class CObjectSkin : public CObject // CObject継承
{
public:
	static constexpr int IDX_BONE_NUM = 2;	// ボーンインデックスの最大数

	// 頂点情報
	typedef struct
	{
		float fWeight;	// 重さ
		int nIdxBone;	// ボーンへのインデックス
	}VERTEX;

	// 頂点情報セット
	typedef struct
	{
		VERTEX vtx[IDX_BONE_NUM];	// ボーンのインデックス情報
		D3DXVECTOR3 posOffset;		// 頂点のオフセット位置
	}VERTEXSET;

	// ボーン構造体
	typedef struct BONE
	{
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		BONE* pBonePerent;		// 親ボーン
		int m_nIdxModel;		// モデルのインデックス
		int m_nIdxParent;		// 親モデルへのインデックス
	}BONE;

	// キー
	typedef struct
	{
		D3DXVECTOR3 rot;	// 向き
	}KEY;

	// キーセット
	typedef struct
	{
		int nFrame;						// フレーム数
		std::vector<KEY> vectorKey;	// キーリスト
	}KEYSET;

	// モーション
	typedef struct
	{
		bool bLoop;							// ループの有無
		std::vector<KEYSET> vectorKeySet;	// キーセットリスト
	}MOTION;


	CObjectSkin();
	~CObjectSkin();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const char* pLoadFIlePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CObjectSkin* Create(const char* pLoadFIlePass);
	void Load(const char*pLoadFIlePass);

	// セッター

	// ゲッター


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	std::vector<CModel*>m_pVectorBoneModel;	// ボーンのモデル
	std::vector<BONE>m_vectorBone;			// ボーンのリスト
	std::vector<VERTEXSET>m_vectorVtxSet;	// 頂点の情報
	int m_nNumPolygon;						// ポリゴン数
	int m_nNumVtx;							// 頂点数
	int m_nNumIdx;							// インデックス数
	int m_nDivisionX;						// Xの分割数
	int m_nDivisionY;						// Yの分割数

	//std::vector<MOTION*> m_pVectorMotion;	// モーション情報
};

#endif // !_OBJECTSKIN_H_