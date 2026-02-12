//***********************************************************************
//
// イージング処理
// Aouther Tetsuji Yamamoto
// 
//***********************************************************************
#include "easing.h"

float easing::Liner(float fp) { return (fp); }
float easing::EaseInExpo(float fp) { return 0 ? 0 : (float)pow(2, 10 * (double)fp - 10); }
float easing::OutExpo(float fp) { return fp == 1.0f ? 1.0f : 1.0f - (float)pow(2, -10 * (double)fp); }
float easing::EaseInOutCirc(float fp) { return fp < 0.5 ? (1.0f - sqrtf(1.0f - (float)pow((double)2.0f * (double)fp, 2.0))) / 2.0f : (sqrtf(1.0f - (float)pow((double)-2.0f * (double)fp + 2.0f, 2.0)) + 1.0f) / 2.0f; }
float easing::EaseInOutSine(float fp) { return -(cosf(D3DX_PI * fp) - 1) / 2; }
