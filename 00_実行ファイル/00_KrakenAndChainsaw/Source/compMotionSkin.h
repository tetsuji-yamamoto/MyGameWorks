//********************************************************
// 
// モーションスキンコンポーネント
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENTMOTIONSKIN_PARENT_H_
#define _COMPONENTMOTIONSKIN_PARENT_H_

#include "component.h"
#include "bone.h"
#include "myMath.h"

// 前方宣言
class CObject;
class CEntity;
class CCollisionModel;
class CWeapon;
class CShader;

// コンポーネントクラス
class CMotionSkin : public CComponent
{
public:
	static constexpr int IDX_BONE_NUM = 2;			// ボーンインデックスの最大数
	static constexpr int MMOTION_BLENDTIME = 10;	// モーションのブレンドする時間

	// キー情報
	typedef struct
	{
		float fPosX;			// 位置X
		float fPosY;			// 位置Y
		float fPosZ;			// 位置Z
		D3DXQUATERNION quat;	// クォータニオン
	}KEY;

	// キーのセット情報
	typedef struct
	{
		int nFlame;				// フレーム数
		int nNumkey;			// キーの数
		std::vector<KEY> vKey;	// キー
	}KEYSET;

	// モーションの情報
	typedef struct
	{
		bool bLoop;					// ループするかどうか
		int nNumKeyInfo;				// キー情報の数
		std::vector<KEYSET> vKeySet;	// キーセット
	}MOTION;

	// スキンメッシュ用頂点情報
	typedef struct
	{
		D3DXVECTOR3 posOffSet;			// 頂点のオフセット
		D3DXVECTOR3 norOffSet;			// 法線ののオフセット
		float aVtxWaght[IDX_BONE_NUM];	// 重さ
		int aIdxBone[IDX_BONE_NUM];		// ボーンへのインデックス
	}VERTEXSKIN;

	// モデル情報
	typedef struct
	{
		std::vector<std::string> vsTexturePath;		// テクスチャへのインデックスポインター
		LPD3DXMESH pMesh;							// メッシュ
		LPD3DXBUFFER pBuffMat;						// マテリアル
		DWORD dwNumMat;								// マテリアルの数
		D3DXMATRIX mtxWorld;						// ワールドマトリックス
	}MODEL;

	// モーションで使う情報
	typedef struct MOTIONINFO
	{
		MOTIONINFO()
		{
			nKeySetNow = 0;			// 現在のキーセット
			nKeySetNext = 0;		// 次のキーセット
			nKeyNow = 0;			// 今のキー
			nCounterMotion = 0;		// モーションカウンター
			nTypeMotionNow = -1;	// 今のモーションタイプ
			bEndMotion = false;		// モーションが終わったかどうか
		}

		int nKeySetNow;						// 現在のキーセット
		int nKeySetNext;					// 次のキーセット
		int nKeyNow;						// 今のキー
		int nCounterMotion;					// モーションカウンター
		int nTypeMotionNow;					// 今のモーションタイプ
		bool bEndMotion;					// モーションが終わったかどうか
	}MOTIONINFO;

	CMotionSkin(std::string motionFilePath);
	~CMotionSkin();

	void InitText(std::string motionFilePath);
	void InitBinary(std::string motionFilePathBinary);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateMotionGetKey(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutRot, D3DXQUATERNION* pOutQuat, MOTIONINFO* pMotionInfo);
	void UpdateMotion(CMotionSkin* pBlendMotion = nullptr);
	void UpdateModel(void);

	void SwitchIsStopMotion(void) { m_bStopMotion = !m_bStopMotion; }

	// セッター
	void SetMotion(int nTypeMotion, int nBlendTime = MMOTION_BLENDTIME);
	void SetRotationModel(D3DXQUATERNION quat) { m_Quat = quat; }
	void SetBoneRotationMatrix(int nBoneIdx, D3DXMATRIX mtxRot);
	void SetBoneMatrix(int nBoneIdx, D3DXMATRIX mtx);
	void SetBonePos(int nBoneIdx, D3DXVECTOR3 pos);
	void SetBoneRotate(int nBoneIdx, D3DXQUATERNION quat);
	void SetMatrixModel(D3DXMATRIX mtx) { m_model.mtxWorld = mtx; }
	void SetQuaternionModel(D3DXQUATERNION quat) { m_Quat = quat; }
	void SetIsStopMotion(bool bStopMotion) { m_bStopMotion = bStopMotion; }

	// ゲッター
	std::vector<CCollisionModel*>& GetVectorCollModel(void) { return m_vCollModel; }
	MOTIONINFO GetMotionInfo(void) { return m_motionInfo; }
	MOTION GetMotion(const int nTypeMotion);
	D3DXQUATERNION GetRotationModel(void) { return m_Quat; }
	D3DXQUATERNION GetQuaternionModel(void) { return m_Quat; }
	D3DXVECTOR3 GetBonePos(int nBoneIdx);
	D3DXVECTOR3 GetBonePosOffSet(int nBoneIdx);
	D3DXVECTOR3 GetVec3BoneToBone(int nBoneIdxA, int nBoneIdxB);
	D3DXMATRIX GetBoneMatrix(int nBoneIdx);
	D3DXMATRIX* GetBoneMatrixPointer(int nBoneIdx);
	D3DXMATRIX GetMatrixModel(void) { return m_model.mtxWorld; }
	LPD3DXMESH GetMesh(void) { return m_model.pMesh; }
	CEntity* GetEntity(void) { return m_pEntity; }
	bool GetIsMotionEnd(void) { return m_motionInfo.bEndMotion; }
	bool GetIsStopMotion(void) {return  m_bStopMotion; }
	KEY GetKey(const int nTypeMotion, const int nKeySet, const int nBone, const int nFrame);
	KEY GetKeyBoneNow(const int nBone);
	KEYSET GetKeySetEnd(const int nTypeMotion);

	bool CollisionMesh(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutNor, D3DXMATRIX* pOutMtx, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius, float fMovement, float fAngleNorLimit);
	bool CollisionModel(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutNor, D3DXMATRIX* pOutMtx, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fRadius, float fMovement, float fAngleNorLimit);

private:
	CShader* m_pShader;
	CEntity* m_pEntity;							// エンティティ
	D3DXQUATERNION m_Quat;						// クォータニオン
	MODEL m_model;								// モデル情報
	MOTIONINFO m_motionInfo;					// モーションで使う情報
	MOTIONINFO m_motionInfoBlend;				// モーションで使う情報ブレンド用
	std::vector<MOTION> m_vMotion;				// モーション
	std::vector<CBone::BONE> m_vBone;			// ボーン
	std::vector<VERTEXSKIN> m_vVtxSkin;			// 頂点スキンメッシュ用
	std::vector<CCollisionModel*> m_vCollModel;	// 当たり判定用モデルリスト
	std::vector<CWeapon*> m_vWeapon;			// 武器モデルリスト
	std::string m_sXFilePass;					// ファイルパス
	std::string m_motionFIlePath;				// モーションファイルパス
	int m_nNumBone;								// ボーン数
	int m_nNumVtx;								// 頂点数
	int m_nNumMotion;							// モーションの総数
	int m_nBlendTime;							// ブレンド時間
	int m_nCntBlend;							// ブレンドカウンター
	int m_nTypeBlendMotion;						// ブレンドするモーションタイプ
	bool m_bBlend;								// モーションブレンドの有無
	bool m_bStopMotion;							// モーションストップの有無
};
#endif