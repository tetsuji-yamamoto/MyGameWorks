//****************************************
// 
// プリコンパイルヘッダー
// Author Tetsuji Yamamoto
// 
//****************************************
#ifndef _PCH_H_
#define _PCH_H_

#include<windows.h>
#include <stdio.h>
#include "d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"						// 入力処理に必要
#include "Xinput.h"						// ジョイパッド処理に必要
#include "xaudio2.h"					// サウンド処理に必要
#include <float.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include "json.hpp"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9,lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント(部品)に必要
#pragma comment(lib,"winmm.lib")	// D
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要
#pragma comment(lib,"xinput.lib")	// ジョイパッド処理に必要

#define SCREEN_WIDTH (1280)			// ウィンドウの幅
#define SCREEN_HEIGHT (720)			// ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					// 座標.カラー.テクスチャ
#define FVF_VERTEX_2D_MULTI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1| D3DFVF_TEX2)	// 座標.カラー.テクスチャ1.テクスチャ2
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 座標.法線.カラー.テクスチャ

// 頂点情報[2D]構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_2D;

// 頂点情報[2D]構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex1;	// テクスチャ座標
	D3DXVECTOR2 tex2;	// テクスチャ座標
}VERTEX_2D_MULTI;

// 頂点情報[3D]構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D;

// 頂点情報[3Dモデル]構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D_MODEL;

// 初期化値
namespace InitValu
{
	const int INTMAX = 2147483647;
	const D3DXVECTOR2 VEC2_NULL = { 0.0f,0.0f };
	const D3DXVECTOR2 VEC2_LEFTTOP = { 0.0f,0.0f };
	const D3DXVECTOR2 VEC2_LEFTBOTTOM = { 0.0f,1.0f };
	const D3DXVECTOR2 VEC2_RIGHTTOP = { 1.0f,0.0f };
	const D3DXVECTOR2 VEC2_RIGHTBOTTOM = { 1.0f,1.0f };
	const D3DXVECTOR3 VEC3_NULL = { 0.0f,0.0f ,0.0f };
	const D3DXVECTOR3 VEC3_Y = { 0.0f,1.0f ,0.0f };
	const D3DXVECTOR3 VEC3_CENTER = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
	const D3DXQUATERNION QUAT_NULL = { 0.0f,0.0f ,0.0f,1.0f };
	const D3DXMATRIX MTX_NULL = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f };
}

// パラメータ値parameters
namespace Parameters
{
	const float HALF = 0.5f;	// 半分の値
}


// 初期化値
namespace Color
{
	static D3DXCOLOR Color255To1(int r, int g, int b, int a)
	{
		D3DXCOLOR col;
		col.r = (float)r / 255.0f;
		col.g = (float)g / 255.0f;
		col.b = (float)b / 255.0f;
		col.a = (float)a / 255.0f;
		return col;
	}

	const D3DXCOLOR _FULL			= { 1.0f,1.0f,1.0f,1.0f };
	const D3DXCOLOR _WHITE			= { 1.0f,1.0f,1.0f,1.0f };
	const D3DXCOLOR _NULL			= { 0.0f,0.0f,0.0f,0.0f };
	const D3DXCOLOR _BLACK			= { 0.0f , 0.0f , 0.0f,1.0f };
	const D3DXCOLOR _GRAY			= { 0.5f , 0.5f , 0.5f, 1.0f };
	const D3DXCOLOR _DARKCYAN		= { 0.0f,0.54f,0.54f,1.0f };
	const D3DXCOLOR _BLUE			= { 0.0f,0.0f,1.0f,1.0f };
	const D3DXCOLOR _LIGHT_BLUE		= Color255To1(173,216,230,255);		// 明るい灰みの青系の色
	const D3DXCOLOR _MAGENTA		= { 1.0f,0.0f,1.0f,1.0f };
	const D3DXCOLOR _MEDIUMNIOLETED	= Color255To1(199, 21, 133, 255);	// あざやかな赤紫系の色
	const D3DXCOLOR _RED			= { 1.0f,0.0f,0.0f,1.0f };
	const D3DXCOLOR _GREEN			= { 0.0f,1.0f,0.0f,1.0f };
	const D3DXCOLOR _LIGHTGREEN		= { 0.564f,0.933f,0.447f,1.0f };	// 明るい灰みの緑系の色
	const D3DXCOLOR _YELLOW			= { 1.0f,1.0f,0.0f,1.0f };
	const D3DXCOLOR _DARKGREEN		= { 0.0f,0.3921f,0.152f,1.0f };
	const D3DXCOLOR _AQUAMARINE		= { 0.498f,1.0f,0.831f,1.0f };
	const D3DXCOLOR _CORAL			= { 1.0f,0.498f,0.313f,1.0f };		// 明るい赤系の色
	const D3DXCOLOR _BLUEVIOLET		= { 0.541f,0.168f,0.886f,1.0f };	// 鮮やかな紫系の色
	const D3DXCOLOR _BONE			= { 0.5f,0.8f,0.8f,1.0f };
	const D3DXCOLOR _BONE_PERENT	= { 0.0f,0.0f,1.0f,1.0f };
	const D3DXCOLOR _BONE_SELECT	= { 1.0f,0.6f,0.0f,1.0f };
}

#endif // !_PCH_H_
