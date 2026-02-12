//****************************************************
// 
// 武器クラス
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_WEAPON_H_
#define _COMPONENT_WEAPON_H_

#include "component.h"
#include "bone.h"
#include "compInfo.h"
#include "compCollisionShapeSpherTrigger.h"

// 武器クラス
class CWeapon : public CComponent
{
public:

	CWeapon();
	CWeapon(const int nIdxBone,const D3DXVECTOR3 pos, const D3DXQUATERNION quat, const std::string& XFilePath);
	~CWeapon();

	void Init(void);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void AddPosition(D3DXVECTOR3 add) { m_pos += add; }
	void AddRotate(D3DXVECTOR3 axis, float fAngle);
	void AddCollSpherTrigger(D3DXVECTOR3 posOffset, float fRadius);

	void DeleteSpherColliderTrgger(void);

	// セッター
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetQuat(const D3DXQUATERNION quat) { m_quat = quat; }
	void SetIdxBone(const int nIdx) { m_nIdxBone = nIdx; }
	void SetXFilePath(const std::string XFilePath) { m_XFilePath = XFilePath; }
	void SetParentBone(CBone::BONE* pBone) { m_pParentBone = pBone; }
	void SetIsHilights(bool bHilights) { m_bHilights = bHilights; }

	// ゲッター
	std::string& GetXFilePath(void) { return m_XFilePath; }
	D3DXQUATERNION GetQuat(void) { return m_quat; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetHitPosition(void) { return m_posHit; }
	D3DXMATRIX* GetPointerMatrix(void) { return &m_mtxWorld; }
	bool GetIsCollision(void) { return m_bCollision; }
	int GetIdxBone(void) { return m_nIdxBone; }

private:
	std::vector < CCollisionShapeSpherTrigger*> m_vCompCollSpherTrigger;	// 当たり判定球配列
	std::vector<std::string> m_vsTexturePath;								// テクスチャへのインデックスポインター
	std::string m_XFilePath;												// Xファイルパス
	D3DXQUATERNION m_quat;													// 向き
	LPD3DXBUFFER m_pBuffMat;												// マテリアル
	CBone::BONE* m_pParentBone;												// 親ボーン
	D3DXVECTOR3 m_pos;														// 位置
	D3DXVECTOR3 m_posHit;													// 当たった位置
	LPD3DXMESH m_pMesh;														// メッシュ
	D3DXMATRIX m_mtxWorld;													// ワールドマトリックス
	DWORD m_dwNumMat;														// マテリアルの数
	bool m_bHilights;														// 強調表示の有無
	bool m_bCollision;														// 当たったか否か
	int m_nIdxBone;															// ボーンインデックス
};

#endif