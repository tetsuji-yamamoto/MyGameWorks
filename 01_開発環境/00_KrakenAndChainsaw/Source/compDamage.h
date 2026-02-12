//****************************************************
// 
// ダメージコンポーネント
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _COMPONENT_DAMAGE_H_
#define _COMPONENT_DAMAGE_H_

#include "component.h"

// 武器クラス
class CDamage : public CComponent
{
public:

	CDamage();
	~CDamage();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}

	// セッター
	void SetIsDamage(bool bDamage) { m_bDamage = bDamage; }

	// ゲッター
	bool GetIsDamage(void) { return m_bDamage; }

private:
	bool m_bDamage;	// ダメージか否か
};

#endif