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
#include <stdlib.h>
#include <float.h>
#include <vector>
#include <list>
#include <string>
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
#define FVF_VERTEX_LINE (D3DFVF_XYZ | D3DFVF_DIFFUSE )									// 座標.法線.カラー.テクスチャ

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

// 頂点情報[3D]構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DCOLOR col;		// 頂点カラー
}VERTEX_LINE;

// 初期化値
namespace InitValu
{
	const D3DXCOLOR RGBA_FULL = { 1.0f,1.0f,1.0f,1.0f };
	const D3DXCOLOR COLOR_NULL = { 0.0f,0.0f,0.0f,0.0f };
	const D3DXCOLOR COLOR_GRAY = { 0.5f , 0.5f , 0.5f, 1.0f };
	const D3DXCOLOR COLOR_DARKCYAN = { 0.0f,0.54f,0.54f,1.0f };
	const D3DXCOLOR COLOR_BLUE = { 0.0f,0.0f,1.0f,1.0f };
	const D3DXCOLOR COLOR_CYAN = { 0.0f,1.0f,1.0f,1.0f };
	const D3DXCOLOR COLOR_MAGENTA = { 1.0f,0.0f,1.0f,1.0f };
	const D3DXCOLOR COLOR_RED = { 1.0f,0.0f,0.0f,1.0f };
	const D3DXCOLOR COLOR_YELLOW = { 1.0f,1.0f,0.0f,1.0f };
	const D3DXCOLOR COLOR_GREEN = { 0.0f,1.0f,0.0f,1.0f };
	const D3DXCOLOR COLOR_AQUAMARINE = { 0.498f,1.0f,0.831f,1.0f };
	const D3DXCOLOR COLOR_BONE = { 0.5f,0.8f,0.8f,1.0f };
	const D3DXCOLOR COLOR_BONE_PERENT = { 0.0f,0.0f,1.0f,1.0f };
	const D3DXCOLOR COLOR_BONE_SELECT = { 1.0f,0.6f,0.0f,1.0f };
	const D3DXVECTOR3 VEC3_NULL = { 0.0f,0.0f ,0.0f };
	const D3DXQUATERNION QUAT_NULL = { 0.0f,0.0f ,0.0f,1.0f };
	const D3DXMATRIX MTX_NULL = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f };
}
#endif // !_PCH_H_
