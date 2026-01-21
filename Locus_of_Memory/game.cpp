//=============================================================================
//
//	Locus of Memory [game.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"game.h"
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
#include"shadow.h"
//#include"billboard.h"
//#include"wall.h"
//#include"field.h"
//#include"meshfield.h"
//#include"mashwall.h"
//#include"meshsky.h"
//#include"block.h"
//#include"fence.h"
//#include"coin.h"
//#include"half_fence.h"
//#include"score.h"
//#include"timer.h"


bool g_bPauseMenu = true;					// ポーズメニュー状態のON/OFF
bool g_bPause = false;
GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
int g_nCounterGameState = 0;				// 状態管理カウンター

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitGame(void)
{
	// プレイヤーの初期化設定
	//InitBG();

	//InitPause();

	InitLight();

	InitCamera();

	//InitField();

	//InitMeshsky();

	//InitMeshfield();

	//InitWall();

	//InitMashwall();

	//InitBullet();

	//InitBillboard();

	InitShadow();

	InitPlayer();

	//InitModel();

	//InitBlock();

	//InitScore();

	//InitTimer();
	
	//PlaySound(SOUND_LABEL_GAME);

}
//=======================================================
// ゲームの終了処理
//=======================================================
void UninitGame(void)
{
	// プレイヤーの終了処理
	//UninitPause();

	UninitLight();

	UninitCamera();

	//UninitField();

	//UninitMeshfield();

	//UninitWall();

	//UninitMashwall();

	//UninitBullet();

	UninitShadow();

	//UninitBillboard();

	UninitPlayer();

	//UninitModel();

	//UninitBlock();

	//UninitScore();

	//UninitTimer();

	//StopSound();

}
//=======================================================
// ゲートの更新処理
//=======================================================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();
	FADE* pFade = GetFade();
	//Timer* pTimer = GetTimer();


	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = (g_bPause) ? false : true;
	}
	
	
	if (g_bPause == true)
	{// ポーズ中

		//if (GetKeyboardTrigger(DIK_F4) == true)
		//{// ポーズ画面ON/OFF切り替え

		//	g_bPauseMenu = g_bPauseMenu ? false : true;
		//}

		//if (g_bPauseMenu == true)
		//{// ポーズ画面ON

		//	// ポーズの更新処理
		//	UpdatePause();
		//}
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

		//UpdateWall();

		//UpdateMashwall();

		//UpdateBullet();

		UpdateShadow();

		//UpdateBillboard();

		UpdatePlayer();

		//UpdateMotion();

		//UpdateModel();

		//UpdateBlock();

		//UpdateScore();

		//UpdateTimer();
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:		// 通常状態

		if (GetKeyboardTrigger(DIK_P) == true && *pFade == FADE_NONE || GetJoypadPress(JOYKEY_B,1) == true && *pFade == FADE_NONE)
		{// ポーズON/OFF切り替え

			g_bPause = g_bPause ? false : true;
		}

		break;

	case GAMESTATE_END:			// 終了状態

		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 || GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
		{// 0以下になった

			g_gameState = GAMESTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);

			// サウンド停止
			//StopSound(SOUND_LABEL_BGM000);
		}
	}


	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった

		g_gameState = GAMESTATE_NONE;

		// フェード設定(リザルト画面に移行)
		SetFade(MODE_RESULT);

		// サウンド停止
		//StopSound(SOUND_LABEL_BGM000);
	}

	//else if (/*GetKeyboardTrigger(DIK_L) == true && *pFade == FADE_NONE ||*/ pTimer->Time <= 0 && *pFade == FADE_NONE)
	//{// 0以下になった

	//	g_gameState = GAMESTATE_NONE;

	//	// フェード設定(リザルト画面に移行)
	//	SetFade(MODE_LOSE);

	//	// サウンド停止
	//	StopSound(SOUND_LABEL_BGM000);
	//}

}
//=======================================================
// ゲームの描画処理
//=======================================================
void DrawGame(void)
{
	// プレイヤーの描画処理
	//DrawBG();

	//DrawField();

	//DrawMeshsky();

	//DrawMeshfield();

	//DrawWall();

	//DrawMashwall();

	//DrawBullet();

	DrawShadow();

	//DrawBillboard();

	DrawPlayer();

	//DrawModel();

	//DrawBlock();

	//DrawScore();

	//DrawTimer();

	if (g_bPause == true)
	{// ポーズ中

		if (g_bPauseMenu == true)
		{// ポーズ画面ON

			// ポーズの描画処理
			//DrawPause();
		}
	}

}
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;				// ゲーム状態設定
	g_nCounterGameState = nCounter;		// 状態管理カウンター設定
}
GAMESTATE GetGameState(void)
{
	return g_gameState;
}