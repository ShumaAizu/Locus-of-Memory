//=============================================================================
//
//	プリコンパイルヘッダー [ pch.h ]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PCH_H_
#define _PCH_H_

//*****************************************************************************
// ヘッダーインクルード
//*****************************************************************************

#include <windows.h>
#include <cstdio>
#include <crtdbg.h>
#include "d3dx9.h"								// 描画処理に必要
#include "dinput.h"								// 入力処理に必要
#include "xaudio2.h"							// サウンド処理に必要
#include "xinput.h"								// ジョイパッド処理に必要

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")				// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")				// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")				// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")				// システムの時刻取得に必要
#pragma comment(lib, "dinput8.lib")				// 入力処理に必要
#pragma comment(lib, "xinput.lib")				// ジョイパッド処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIRECTINPUT_VERSION		(0x0800)																		// ビルド時の警告対処用マクロ
#define SCREEN_WIDTH			(1280.0f)																		// ウインドウの幅
#define SCREEN_HEIGHT			(720.0f)																		// ウインドウの高さ
#define DEFAULT_FADESPEED		(0.025f)																		// フェードの速度
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)									// 頂点フォーマット(2D)
#define FVF_VERTEX_3D			(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)						// 頂点フォーマット(3D)
#define SQRTF(X, Y)				(sqrtf(X * X + Y * Y))															// 対角線の長さ算出
#define INIT_D3DXVEC3			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))													// D3DXVECTOR3型初期化
#define RANDAM_VEC3(X, Y, Z)	(D3DXVECTOR3((float)(rand() % X), (float)(rand() % Y), (float)(rand() % Z)))	// ランダムVECTOR3
#define VEC3TOROT(vec3)			(vec3 * D3DX_PI)

#define COLOR_BLACK				(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
#define COLOR_WHITE				(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))

//*****************************************************************************
// 頂点情報の構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標(x, y, z)
	float rhw;				// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;			// 頂点カラー(r, g, b, a)
	D3DXVECTOR2 tex;		// テクスチャ座標(x, y)
}VERTEX_2D;

//*****************************************************************************
// 頂点情報(3D)の構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標(x, y, z)
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR col;			// 頂点カラー(r, g, b, a)
	D3DXVECTOR2 tex;		// テクスチャ座標(x, y)
}VERTEX_3D;

#endif	// _PCH_H_