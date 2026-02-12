//***********************************************************************
//
// イージング処理
// Aouther Tetsuji Yamamoto
// 
//***********************************************************************
#ifndef _EASING_H_
#define _EASING_H_

#include "main.h"

// イージング処理名前空間
namespace easing
{
	// 特に変化がない直線の動き
	float Liner(float fp);

	// 前半はほとんど変化がなく,中盤に値が大きくなり,後半に急激に大きくなる
	float EaseInExpo(float fp);
	
	// 前半は急激に大きくなる,中盤に値が穏やかになり,後半にほとんど変化がない
	float OutExpo(float fp);
	
	// 前半は穏やかに増えるる,中盤に急激に増える,後半は穏やかになる
	float EaseInOutCirc(float fp);
	
	// 前半は穏やかに増え,中盤は一定の速度で,後半は穏やかになる
	float EaseInOutSine(float fp);	
}

#endif // !_EASING_H_
