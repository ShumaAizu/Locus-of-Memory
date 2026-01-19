//=============================================================================
//
//	Locus of Memory [fade.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _Fade_H        // このマクロ定義がされなかったら
#define _Fade_H        // 2重インクルード防止のマクロを定義する

#include"main.h"

#define SCREEN_ZERO	(0)

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

// プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext);
FADE* GetFade(void);

#endif