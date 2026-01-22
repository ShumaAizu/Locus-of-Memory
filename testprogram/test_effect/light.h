//==========================================
// 
// 3Dポリゴンの描画[light.h]
// 
//==========================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

// マクロ定義
#define LIGHT_BRIGHTEST			(D3DXCOLOR(1.000f,	1.000f,	1.000f,	1.000f))	// 1番目に明るい光源
#define LIGHT_BRIGHTER			(D3DXCOLOR(0.700f,	0.700f,	0.700f,	1.000f))	// 2番目に明るい光源
#define COLOR_BRIGHT			(D3DXCOLOR(0.300f,	0.300f,	0.300f,	1.000f))	// 3番目に明るい光源

// プロトタイプ宣言
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
#endif
