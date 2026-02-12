//**********************************************
// 
// タコ触手状態待機コンポーネントクラス
// Author Tetsuji Yamamoto
//
//**********************************************
#ifndef _COMPONENT_STATE_NEWTRAL_TENTACLE_H_
#define _COMPONENT_STATE_NEWTRAL_TENTACLE_H_

#include "component.h"

// 前方宣言
class CPartMotion;
class CCounter;
class CPlayer;

// 待機クラス
class CCompStateTentacleNewtral : public CComponent
{
public:
	
	// 引数情報
	typedef struct Argument
	{
		CPartMotion*	pPartMotion			= nullptr;	// 特定部位のモーション制御ポインタ
		CCounter*		pCounter			= nullptr;	// カウンターポインタ
		std::string		TentacleNumber		= "00";		// 触手番号
		std::string		Name;							// 触手番号
		int				nTimeMotionBlend	= 0;		// モーションをブレンドする時間
		float			fSensingHeight		= 0.0f;		// 検知高さ
		float			fSensingAngleRange	= 0.0f;		// 検知範囲
		float			fSensingAngle		= 0.0f;		// 検知向き
	}Argument;

	CCompStateTentacleNewtral(Argument arg);
	~CCompStateTentacleNewtral();

	void Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void) {}
private:
	Argument m_Arg;	// 引数情報
};
#endif