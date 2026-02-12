//**************************************************
// 
// 編集
// Author Tesuji Yamamoto
// 
//**************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

#include "object.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "bone.h"
#include "component.h"
#include <vector>

// 前方宣言
class CRange2D;				// 範囲2D
class CFunctionBase;		// 機能基底クラス
class CFunctionEditBase;	// 機能基底クラス
class CWeapon;				// 武器クラス
class CCollisionModel;		// 当たり判定用モデルクラス

// 編集クラス
class CEdit : CObject
{
public:
	static constexpr int IDX_BONE_NUM = 2;				// ボーンインデックスの最大数
	static constexpr float MARK_WIDTH = 40.0f;			// 頂点マークの幅
	static constexpr float WAGTH_INCDEC_AMOUNT = 0.1f;	// 重さの増減量
	static constexpr float WAGTH_MAX = 1.0f;			// 重さの最大値
	static constexpr float WAGTH_MIN = 0.0f;			// 重さの最小値
	static constexpr float ADDMOVEMENT = 0.1f;			// 移動量
	static constexpr float ADDROTATE = D3DX_PI * 0.01f;	// 移動量

	// 編集タイプ
	typedef enum
	{
		EDITTYPE_NONE = 0,
		EDITTYPE_BONE_RIGGING,
		EDITTYPE_BONE_SKINNING,
		EDITTYPE_BONE_PREVIEW,
		EDITTYPE_MAX
	}EDITTYPE;

	// 頂点編集用構造体
	typedef struct
	{
		D3DXVECTOR3 posOffSet;			// 頂点のオフセット
		D3DXVECTOR3 norOffSet;			// 法線ののオフセット
		float aVtxWaght[IDX_BONE_NUM];	// 重さ
		int aIdxBone[IDX_BONE_NUM];		// ボーンへのインデックス
	}VERTEXEDIT;

	// 頂点グループ
	typedef struct
	{
		std::vector<int> vVtxIdx;		// 頂点番号インデックス
		D3DXVECTOR3 pos;				// 位置
		int nID;						// 自身の番号
		float aVtxWaght[IDX_BONE_NUM];	// 重さ
		int aIdxBone[IDX_BONE_NUM];		// ボーンへのインデックス
	}VERTEXGROUP;

	// キー
	typedef struct
	{
		D3DXVECTOR3 rot;	// 向き
	}KEY;

	// キーセット
	typedef struct
	{
		int nFrame;					// フレーム数
		std::vector<KEY> vectorKey;	// キーリスト
	}KEYSET;

	// モデル情報
	typedef struct
	{
		std::vector<std::string> vsTexturePath;		// テクスチャへのインデックスポインター
		LPD3DXMESH pMesh;		// メッシュ
		LPD3DXBUFFER pBuffMat;	// マテリアル
		DWORD dwNumMat;			// マテリアルの数
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	}MODEL;

	CEdit();
	~CEdit();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const char* pEditModelFilePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Debug(void);
	void DebugNone(void);
	void DebugRigging(void);
	void DebugSknning(void);
	void DebugWeapon(void);
	void DebugCollModel(void);
	void DebugPreview(void);

	static CEdit* Create(const char* pEditModelFilePass);
	VERTEXGROUP* touchVertex(void);
	CBone::BONE* touchBone(D3DXVECTOR3 posScreen);
	void SelectBone(int nAdd);
	void editVtx_BoneIdxReset(int nIdxBone);
	void editNone(void);
	void editVtx_Waght(void);
	void editVtx_BoneIdx(int nIdxBone);
	void editBoneRigging(void);
	void editBoneSkinning(void);
	void editBonePreview(void);
	void CreateBone(void);
	void DeleteBone(void);
	void BoneModelSettingPosition(void);
	void BoneModelOllSetColor(D3DXCOLOR colBone, D3DXCOLOR colBoneParent, D3DXCOLOR colBoneSelect);
	void SelectBonePerrent(int nAddSelect);
	void editSelectVertexColor(void);
	void editVertexWaghtColor(void);
	void SwitchisDisp(void) { m_bDisp = !m_bDisp; }
	void CreateRange(void);
	void DeleteRange(void);
	void ResetVtx(void);
	void ResetBone(void);
	void SwitchIsZTest(void) { m_bZTest = !m_bZTest; }
	void AddFunction(CFunctionBase* function);
	void ChangeFunctionEdit(CFunctionEditBase* function);
	void BoneScaling(float fPlam);
	void BoneMovementScaling(float fPlam);

	void WeaponSelectOperate(int nAddIdx);
	void WeaponSelectFollowBone(int nAddIdx);
	void WeaponAddPosition(D3DXVECTOR3 add);
	void WeaponResetPosition(void);
	void WeaponAddRotate(D3DXVECTOR3 axis);
	void WeaponResetRotate(void);
	void WeaponHighlightsOperate(void);
	void WeaponHighlightsParentBone(void);

	void CollModelSelectOperate(int nAddIdx);
	void CollModelSelectFollowBone(int nAddIdx);
	void CollModelAddPosition(D3DXVECTOR3 add);
	void CollModelResetPosition(void);
	void CollModelAddRotate(D3DXVECTOR3 axis);
	void CollModelResetRotate(void);
	void CollModelHighlightsOperate(void);
	void CollModelHighlightsParentBone(void);

	bool SkinInfoWrite(void);
	bool SkinInfoWriteBinary(void);
	bool SkinInfoRead(void);
	bool SkinInfoReadBinary(void);
	bool ReadModel(void);

private:
	CEntity* m_pPlayer;								// プレイヤー
	std::vector<CBone::BONE> m_vBone;				// ボーン
	std::vector<CBone*> m_vpBoneModel;				// ボーンのモデル
	std::vector<VERTEXEDIT> m_vVtxEdit;				// 頂点編集用
	std::vector<VERTEXGROUP> m_vVtxGroup;			// 頂点グループ
	std::vector<CObjectBillboard*> m_vpVtxMark;		// 印ポインター
	std::vector<CFunctionBase*> m_vpFunctionBase;	// 機能クラスポインター
	std::string m_sXFilePass;						// ファイルパス
	CFunctionEditBase* m_pFunctionEdtBase;			// 機能クラスポインター
	MODEL m_model;									// モデル情報
	int m_nNumVtx;									// 頂点数

	int m_nEditType;								// 編集タイプ
	int m_nNumBone;									// ボーンの総数
	int m_nSelectBone;								// 今選択しているボーン
	bool m_bDisp;									// 映すか否か
	float m_fSetWaght;								// 設定する重さ
	CRange2D* m_pRange2D;							// 範囲選択用ポリゴン
	VERTEXGROUP* m_pTouchVtxGroup;					// 触れいている頂点グループ
	CBone::BONE* m_pTouchBone;						// 触れているボーン
	D3DXVECTOR3 m_posRangeA;						// 範囲位置A
	D3DXVECTOR3 m_posRangeB;						// 範囲位置B
	std::vector<VERTEXGROUP*> m_vpTouchVtxGroup;	// 触れいている頂点グループ

	int m_nSelectWeapon;								// 今選択している武器
	CWeapon* m_pWeaponOperate;							// 操る武器
	std::vector <CWeapon*> m_vectorWeapon;				// 武器リスト

	int m_nSelectCollModel;								// 今選択している当たり判定用モデル
	CCollisionModel* m_pCollModelOperate;				// 操る当たり判定用モデル
	std::vector <CCollisionModel*> m_vectorCollModel;	// 当たり判定用モデルリスト

	float m_fMovementBone;	// ボーンの移動量
	bool m_bZTest;			// Zテストの有無
};
#endif