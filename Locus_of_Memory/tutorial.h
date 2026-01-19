//=============================================================================
//
//	Locus of Memory [tutorial.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _TUTORIAL_H_        // このマクロ定義がされなかったら
#define _TUTORIAL_H_        // 2重インクルード防止のマクロを定義する

#include"main.h"

// ゲームの状態列挙型の定義
typedef enum
{
	TUTORIALSTATE_NONE = 0,		// 何もしていない状態
	TUTORIALSTATE_NORMAL,		// 通常状態
	TUTORIALSTATE_END,			// 終了状態
	TUTORIALSTATE_MAX
}TUTORIALSTATE;

// プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void SetTutorialePause(bool bPause1);
void SetTutorialState(TUTORIALSTATE state, int nCounter);
TUTORIALSTATE GetTutorialState(void);

#endif