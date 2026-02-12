//******************************************************
// 
// モーション(スキンメッシュ)
// Author Tetsuji Yamamoto
// 
//******************************************************
#ifndef _MOTIONSKIN_H_
#define _MOTIONSKIN_H_

#include "bone.h"
#include "component.h"
#include <vector>

// モーションクラス
class CMotionSkin
{
public:
	static constexpr float ADDMOVEMENT = 0.1f;
	static constexpr int IDX_BONE_NUM = 2;			// ボーンインデックスの最大数
	static constexpr int MMOTION_BLENDTIME = 10;	// モーションのブレンドする時間

	// キー情報
	typedef struct KEY
	{
		KEY()
		{
			fPosX = 0.0f;
			fPosY = 0.0f;
			fPosZ = 0.0f;
			quat = InitValu::QUAT_NULL;
		}
		float fPosX;			// 位置X
		float fPosY;			// 位置Y
		float fPosZ;			// 位置Z
		D3DXQUATERNION quat;	// クォータニオン
	}KEY;

	// キーのセット情報
	typedef struct KEYSET
	{
		KEYSET() 
		{
			nFlame = 10;
			nNumkey = 0;
		}
		int nFlame;				// フレーム数
		int nNumkey;			// キーの数
		std::vector<KEY> vKey;	// キー
	}KEYSET;

	// モーションの情報
	typedef struct MOTION
	{
		MOTION() 
		{
			bLoop = false;
			nNumKeyInfo = 0;
		};

		bool bLoop;						// ループするかどうか
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
		int* apIdxTexture;		// テクスチャへのインデックスポインター
		LPD3DXMESH pMesh;		// メッシュ
		LPD3DXBUFFER pBuffMat;	// マテリアル
		DWORD dwNumMat;			// マテリアルの数
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	}MODEL;

	// モーションで使う情報
	typedef struct
	{
		int nKeySetNow;						// 現在のキーセット
		int nKeySetNext;					// 次のキーセット
		int nKeyNow;						// 今のキー
		int nCounterMotion;					// モーションカウンター
		int nTypeMotionNow;					// 今のモーションタイプ
		bool bEndMotion;					// モーションが終わったかどうか
	}MOTIONINFO;

	CMotionSkin();
	~CMotionSkin();

	void Init(std::string motionFilePath);
	void InitBinary(std::string motionFilePathBinary);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMotionSkin* Create(std::string motionFilePath);
	static CMotionSkin* CreateBinary(std::string motionFilePathBinary);

	void UpdateMotionGetKey(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutRot,D3DXQUATERNION* pOutQuat ,MOTIONINFO* pMotionInfo);
	void UpdateMotion(CMotionSkin* pBlendMotion = NULL);
	void ReSetPositionBone(int nSelectBone);
	void AddMotion(void);
	void PullMotion(void);
	void AddKeySet(int nMotion);
	void PullKeySet(int nMotion);
	bool ApplicableKeySetBoneToKey(int nMotion, int nKeySet);
	bool ApplicableKeySetKeyToBone(int nMotion, int nKeySet);
	void ReSetBone(void);

	// セッター
	void SetMotion(int nTypeMotion, int nBlendTime = MMOTION_BLENDTIME);
	void SetCounterMotion(int nCounterMotion) { m_motionInfo.nCounterMotion = nCounterMotion; };
	void SetRotateBone(int nSelectBone, D3DXQUATERNION quat);
	void SetRotateBoneNotParent(int nSelectBone, D3DXQUATERNION quat);
	void ReSetRotateBone(int nSelectBone);
	void SetPositionBone(int nSelectBone, D3DXVECTOR3 pos);
	void SetPositionBoneChild(int nSelectBone, D3DXVECTOR3 pos);
	void SetPositionBoneX(int nSelectBone, float fX);
	void SetPositionBoneY(int nSelectBone, float fY);
	void SetPositionBoneZ(int nSelectBone, float fZ);
	void SetKeySetNow(int nkeySet) { m_motionInfo.nKeyNow = nkeySet; }
	void SetNumMotion(int nNumMotion) { m_nNumMotion = nNumMotion; }
	void SetIsEndoMotion(bool bEndMotion) { m_motionInfo.bEndMotion = bEndMotion;}

	// ゲッター
	int GetNumBone(void) { return m_nNumBone; }
	int GetNumMotion(void) { return m_nNumMotion; }
	CBone::BONE* GetBone(int nBone);
	CEntity* GetEntity(void) { return m_pPlayer; }
	D3DXQUATERNION 	GetRotateBone(int nSelectBone) { auto iterBone = m_vBone.begin() + nSelectBone; return (*iterBone).quat; }
	D3DXVECTOR3 GetPositionBone(int nSelectBone);
	D3DXVECTOR3 GetPositionBoneParent(int nSelectBone);
	D3DXVECTOR3 GetBoneVec3(int nSelectBone);

	std::vector<MOTION>& ReferenceVectorMotion(void) { return m_vMotion; }
	std::vector<CBone::BONE> GetVectorBoneReset(void) { return m_vBoneReset; }
	std::vector<CBone::BONE> GetVectorBone(void) { return m_vBone; }
	std::vector<VERTEXSKIN> GetVectorVtxSkin(void) { return m_vVtxSkin; }
	std::string GetXFilePass(void) { return m_sXFilePass; }
	int GetNumVtx(void) { return m_nNumVtx; }

	void BoneModelSettingPosition(void);
	void BoneModelOllSetColor(int nSelectBoneIdx, D3DXCOLOR colBone, D3DXCOLOR colBoneParent, D3DXCOLOR colBoneSelect);
	void RotateBone(int nSelectBone, float fAngle, D3DXVECTOR3 vecAxis);
	void PositionBone(int nSelectBone, D3DXVECTOR3 addPos);
	void BoneScaling(void);

private:
	CEntity* m_pPlayer;						// プレイヤー
	int m_nNumBone;							// ボーン数
	int m_nNumVtx;							// 頂点数
	int m_nNumMotion;						// モーションの総数

	MOTIONINFO m_motionInfo;				// モーションで使う情報
	MOTIONINFO m_motionInfoBlend;			// モーションで使う情報ブレンド用

	bool m_bBlend;							// モーションブレンドの有無
	int m_nBlendTime;						// ブレンド時間
	int m_nCntBlend;						// ブレンドカウンター
	int m_nTypeBlendMotion;					// ブレンドするモーションタイプ
	float m_fMovementBone;								// ボーンの移動量

	std::vector<MOTION> m_vMotion;			// モーション
	std::vector<CBone::BONE> m_vBone;		// ボーン
	std::vector<CBone::BONE> m_vBoneReset;	// ボーンリセット用
	std::vector<CBone*> m_vpBoneModel;		// ボーンのモデル
	std::vector<VERTEXSKIN> m_vVtxSkin;		// 頂点スキンメッシュ用
	MODEL m_model;							// モデル情報
	std::string m_sXFilePass;				// ファイルパス
};

#endif // !_MOTION_H_