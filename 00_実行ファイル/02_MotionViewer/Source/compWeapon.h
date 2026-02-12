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

	// セッター
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetQuat(const D3DXQUATERNION quat) { m_quat = quat; }
	void SetIdxBone(const int nIdx) { m_nIdxBone = nIdx; }
	void SetXFilePath(const std::string XFilePath) { m_XFilePath = XFilePath; }
	void SetParentBone(CBone::BONE* pBone) { m_pParentBone = pBone; }
	void SetIsHilights(bool bHilights) { m_bHilights = bHilights; }

	// ゲッター
	std::string& GetXFilePath(void) { return m_XFilePath; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	int GetIdxBone(void) { return m_nIdxBone; }
	D3DXQUATERNION GetQuat(void) { return m_quat; }

private:
	CBone::BONE* m_pParentBone;	// 親ボーン
	LPD3DXMESH m_pMesh;			// メッシュ
	LPD3DXBUFFER m_pBuffMat;	// マテリアル
	DWORD m_dwNumMat;			// マテリアルの数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXQUATERNION m_quat;		// 向き
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	std::string m_XFilePath;	// Xファイルパス
	int* m_apIdxTexture;		// テクスチャへのインデックスポインター
	int m_nIdxBone;				// ボーンインデックス
	bool m_bHilights;			// 強調表示の有無
};

#endif