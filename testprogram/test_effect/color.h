//==========================================
// 
// 3Dポリゴンの描画[color.h]
// 
//==========================================
#ifndef _COLOR_H_
#define	_COLOR_H_

// マクロ定義
#define COLOR_WHITE			(D3DXCOLOR(1.000f,	1.000f,	1.000f,	1.000f))	// 白
#define COLOR_WHITE_ALPHA	(D3DXCOLOR(1.000f,	1.000f,	1.000f,	0.500f))	// アルファ値半減白
#define COLOR_BLACK			(D3DXCOLOR(0.000f,	0.000f,	0.000f,	1.000f))	// 黒
#define COLOR_RED			(D3DXCOLOR(1.000f,	0.000f,	0.000f,	1.000f))	// 赤
#define COLOR_GREEN			(D3DXCOLOR(0.000f,	1.000f,	0.000f,	1.000f))	// 緑
#define COLOR_BLUE			(D3DXCOLOR(0.000f,	0.000f,	1.000f,	1.000f))	// 青
#define COLOR_YELLOW		(D3DXCOLOR(1.000f,	1.000f,	0.000f,	1.000f))	// 黄
#define COLOR_CYAN			(D3DXCOLOR(0.000f,	1.000f,	1.000f,	1.000f))	// シアン
#define COLOR_MAGENTA		(D3DXCOLOR(1.000f,	0.000f,	1.000f,	1.000f))	// マゼンタ
#define COLOR_SHADOW		(D3DXCOLOR(1.000f,	1.000f,	1.000f,	0.500f))	// 影
#endif