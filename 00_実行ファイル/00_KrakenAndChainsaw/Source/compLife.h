//********************************************************
// 
// 体力コンポーネントクラス
// Author Tetsuji Yamamoto
// 
//********************************************************
#ifndef _COMPONENT_LIFE_H_
#define _COMPONENT_LIFE_H_

#include "component.h"

// 体力コンポーネントクラス
class CLife : public CComponent
{
public:
	static constexpr int VERTEX_NUM = 4;// 頂点数

	// 引数構造体
	typedef struct LifeArgument
	{
		// コンストラクタ
		LifeArgument()
		{
			nLife = 10;	// 体力
			nMax = 10;	// 最大値
			nMin = 0;	// 最小値
		}

		// 変数
		int nLife;	// 体力
		int nMax;	// 最大値
		int nMin;	// 最小値
	}LifeArgument;

	CLife(LifeArgument argment);
	~CLife();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void AddLife(int nAddLife);

	// ゲッター
	int GetLife(void) { return m_argument.nLife; }
	int GetMax(void) { return m_argument.nMax; }
	bool GetIsMax(void);
	bool GetIsMin(void);

private:
	LifeArgument m_argument;	// 各種値
};
#endif