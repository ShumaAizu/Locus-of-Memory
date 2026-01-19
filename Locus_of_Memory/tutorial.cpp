//=============================================================================
//
//	Locus of Memory [tutorial.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"tutorial.h"
#include"player.h"
#include"input.h"
//#include"sound.h"
#include"fade.h"
//#include"pause.h"
#include"camera.h"
#include"light.h"
//#include"polygon.h"
//#include"model.h"
#include"player.h"
//#include"shadow.h"
//#include"billboard.h"
//#include"wall.h"
//#include"field.h"
//#include"meshfield.h"
//#include"mashwall.h"
//#include"meshsky.h"
//#include"block.h"
//#include"score.h"
//#include"timer.h"


bool g_bPauseMenu1 = true;					// ポーズメニュー状態のON/OFF
bool g_bPause1 = false;
TUTORIALSTATE g_TutorialState = TUTORIALSTATE_NONE;		// ゲームの状態
int g_nCounterTutorialState = 0;				// 状態管理カウンター
int g_nCntFade = 0;

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitTutorial(void)
{
	// プレイヤーの初期化設定
	//InitBG();

	//InitPause();

	InitLight();

	InitCamera();

	//InitField();

	//InitMeshsky();

	//InitMeshfield();



	//InitMashwall();

	//InitBullet();

	//InitBillboard();

	//InitShadow();

	InitPlayer();

	//InitModel();

	//InitBlock();

	//InitWall();

	//InitScore();
	//
	//PlaySound(SOUND_LABEL_GAME);

}
//=======================================================
// ゲームの終了処理
//=======================================================
void UninitTutorial(void)
{
	// プレイヤーの終了処理
	//UninitPause();

	UninitLight();

	UninitCamera();

	//UninitField();

	//UninitMeshsky();

	//UninitMeshfield();

	//UninitMashwall();

	//UninitBullet();

	//UninitShadow();

	//UninitBillboard();

	UninitPlayer();

	//UninitModel();

	//UninitBlock();

	//UninitWall();

	//UninitScore();

	//StopSound();

}
//=======================================================
// ゲートの更新処理
//=======================================================
void UpdateTutorial(void)
{
	Player* pPlayer = GetPlayer();
	FADE* pFade = GetFade();
	//Timer* pTimer = GetTimer();


	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause1 = (g_bPause1) ? false : true;
	}
	
	
	if (g_bPause1 == true)
	{// ポーズ中

		if (GetKeyboardTrigger(DIK_F4) == true)
		{// ポーズ画面ON/OFF切り替え

			g_bPauseMenu1 = g_bPauseMenu1 ? false : true;
		}

		if (g_bPauseMenu1 == true)
		{// ポーズ画面ON

			// ポーズの更新処理
			//UpdatePause();
		}
	}
	else
	{
		// プレイヤーの更新処理
		//UpdateBG();

		UpdateCamera();

		UpdateLight();

		//UpdateField();

		//UpdateMeshsky();

		//UpdateMeshfield();

		//UpdateMashwall();

		//UpdateBullet();

		//UpdateShadow();

		//UpdateBillboard();

		UpdatePlayer();

		//UpdateMotion();

		//UpdateModel();

		//UpdateBlock();

		//UpdateWall();

		//UpdateScore();


	}

	switch (g_TutorialState)
	{
	case TUTORIALSTATE_NORMAL:		// 通常状態

		if (GetKeyboardTrigger(DIK_P) == true && *pFade == FADE_NONE || GetJoypadPress(JOYKEY_X,1) == true && *pFade == FADE_NONE)
		{// ポーズON/OFF切り替え

			g_bPause1 = g_bPause1 ? false : true;
		}

		break;

	case TUTORIALSTATE_END:			// 終了状態

		g_nCounterTutorialState--;		// 状態管理カウンター減少

		if (g_nCounterTutorialState <= 0 || GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
		{// 0以下になった

			g_TutorialState = TUTORIALSTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_GAME);

			// サウンド停止
			//StopSound(SOUND_LABEL_BGM000);
		}
	}

	

	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった


			g_TutorialState = TUTORIALSTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_GAME);

			// サウンド停止
			//StopSound(SOUND_LABEL_BGM000);
		

	}


}
//=======================================================
// ゲームの描画処理
//=======================================================
void DrawTutorial(void)
{
	// プレイヤーの描画処理
	//DrawBG();

	//DrawField();

	//DrawMeshsky();

	//DrawMeshfield();



	//DrawMashwall();

	//DrawBullet();

	//DrawShadow();

	//DrawBillboard();

	DrawPlayer();

	//DrawModel();

	//DrawBlock();

	//DrawWall();

	//DrawScore();


	if (g_bPause1 == true)
	{// ポーズ中

		if (g_bPauseMenu1 == true)
		{// ポーズ画面ON

			// ポーズの描画処理
			//DrawPause();
		}
	}

}
void SetTutorialePause(bool bPause1)
{
	g_bPause1 = bPause1;
}
void SetTutorialState(TUTORIALSTATE state, int nCounter)
{
	g_TutorialState = state;				// ゲーム状態設定
	g_nCounterTutorialState = nCounter;		// 状態管理カウンター設定
}
TUTORIALSTATE GetTutorialState(void)
{
	return g_TutorialState;
}