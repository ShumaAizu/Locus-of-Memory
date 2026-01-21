//========================================================
// 
// 3Dアクションゲーム_MASTER KEY[light.h]
// 
//========================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "main.h"

// マクロ定義
#define LIGHT_BRIGHTEST			(D3DXCOLOR(1.000f,	1.000f,	1.000f,	1.000f))	// 1番目に明るい光源
#define LIGHT_BRIGHTER			(D3DXCOLOR(0.800f,	0.800f,	0.800f,	1.000f))	// 2番目に明るい光源
#define LIGHT_BRIGHT			(D3DXCOLOR(0.600f,	0.600f,	0.600f,	1.000f))	// 3番目に明るい光源


// プロトタイプ宣言
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
void SetLightColors(D3DXCOLOR Brightest, D3DXCOLOR Brighter, D3DXCOLOR Bright);
#endif
