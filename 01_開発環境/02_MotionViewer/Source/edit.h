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
#include "motionSkin.h"
#include "component.h"
#include <vector>

class CXYZAxis;				// XYZ軸
class CFunctionBase;		// 機能基底クラス
class CFunctionEditBase;	// 機能基底クラス

// 編集クラス
class CEdit : CObject
{
public:

	static constexpr float ADDROTATE = D3DX_PI * 0.001f;	// ボーンに追加する角度の値
	static constexpr float ADDPOSITION = 0.1f;				// ボーンに追加する位置の値
	static constexpr int SAVETIME = 60 * 30;				// オートセーブ時間

	// 編集タイプ
	typedef enum
	{
		EDITTYPE_MOTION = 0,
		EDITTYPE_PREVIEW,
		EDITTYPE_MAX
	}EDITTYPE;

	CEdit();
	~CEdit();

	HRESULT Init(void) { return S_OK; }
	HRESULT InitBinary(std:: string motionFilePathBinary);
	HRESULT Init(std:: string motionFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void editMotion(void);
	void editPreview(void);
	
	void SelectBone(void);
	void SelectMotionEditMotion(int nSelectNum);
	void SelectMotionEditPreview(int nSelectNum);
	bool WriteMotion(void);
	bool WriteMotionBinary(void);
	bool ReadMotion(void);
	bool ReadMotionBinary(void);

	static CEdit* Create(std::string motionFilePathBinary);
	void Debug(void);
	void AddFunction(CFunctionEditBase* pFunction) { m_vpFunction.push_back(pFunction); }
	void SwitchIsOutSave(void) { m_bOutSave = !m_bOutSave; }

private:
	std::vector<CBone*> m_vpBoneModel;	// ボーンのモデル
	CMotionSkin* m_pMotionSkin;			// モーション情報

	int m_nEditType;									// 編集タイプ
	int m_nNumBone;										// ボーンの総数
	int m_nNumMotion;									// モーションの総数
	int m_nSelectBoneIdx;								// 今選択しているボーンインデックス
	int m_nSelectmotion;								// 今選択しているモーション
	int m_nSelectKeyInfo;								// 今選択してるキーセット番号
	int m_nCntTime;										// タイムカウンター
	bool m_bOutSave;									// オートセーブ
	CBone::BONE* m_pTouchBone;							// 触れているボーン
	std::vector<CMotionSkin::MOTION> m_vMotionEdit;		// 編集用モーション
	std::vector<CFunctionEditBase*> m_vpFunction;		// 機能リスト
	CMotionSkin::KEYSET m_keySetCpy;					// コピー用キーセット
	CMotionSkin::MOTION m_MotionCpy;					// コピー用モーション
	D3DXQUATERNION m_quatCpy;							// コピー用クォータニオン
	D3DXVECTOR3 m_vecCpy;								// コピー用ベクトル

	CXYZAxis* m_pXYZAxis;	// XYZ軸
};
#endif