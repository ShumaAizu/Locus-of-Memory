//===================================================================
//
// magic処理[magic.cpp]
// Author : 眞鍋快陸
// 
//===================================================================
#include "magic.h"
#include "debugproc.h"
#include "input.h"
#include "shadow.h"

//マクロ定義
#define MAX_MAGIC		(128)		//魔法の最大数
#define MAX_COMMAND		(3)			//コマンドの最大数

//グローバル変数宣言
Magic g_aMagic[MAX_MAGIC];					//魔法の情報
COMMANDTYPE g_aCommand[MAX_COMMAND];		//コマンドの情報
int nCntCommand = 0;

//魔法の初期化処理=============================
void InitMagic(void)
{
	for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		g_aMagic[nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].mType = MAGICTYPE_NONE;
		g_aMagic[nCntMagic].bUse = false;
		g_aMagic[nCntMagic].nLife = 0;
		g_aMagic[nCntMagic].nIdxShadow = -1;
	}
}

//魔法の終了処理==============================
void UninitMagic(void)
{
}

//魔法の更新処理==============================
void UpdateMagic(void)
{
	
}

//魔法の描画処理==============================
void DrawMagic(void)
{

}

//コマンド入力情報=============================
MAGICTYPE PressCommand(void)
{

	if (g_aCommand[nCntCommand] == COMMANDTYPE_NONE)
	{//コマンドが何も入力されていないとき
		if (GetJoypadTrigger(JOYKEY_B, 0) == true)
		{//B(赤)が入力された
			g_aCommand[nCntCommand] = COMMANDTYPE_R;
		}
		else if (GetJoypadTrigger(JOYKEY_A, 0) == true)
		{//A(緑)が入力された
			g_aCommand[nCntCommand] = COMMANDTYPE_G;
		}
		else if (GetJoypadTrigger(JOYKEY_X, 0) == true)
		{//X(青)が入力された
			g_aCommand[nCntCommand] = COMMANDTYPE_B;
		}
		else if (GetJoypadTrigger(JOYKEY_Y, 0) == true)
		{//Y(黄)が入力された
			g_aCommand[nCntCommand] = COMMANDTYPE_Y;
		}
	}
	if (g_aCommand[nCntCommand] != COMMANDTYPE_NONE)
	{//コマンドが何かしら入力されているとき
		nCntCommand++;
	}

	if (nCntCommand == MAX_COMMAND)
	{
		//緑緑緑
		if (g_aCommand[0] == COMMANDTYPE_G && g_aCommand[1] == COMMANDTYPE_G && g_aCommand[2] == COMMANDTYPE_G)
		{

		}

		//赤赤赤
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}

		//青青青
		if (g_aCommand[0] == COMMANDTYPE_B && g_aCommand[1] == COMMANDTYPE_B && g_aCommand[2] == COMMANDTYPE_B)
		{

		}

		//黄黄黄
		if (g_aCommand[0] == COMMANDTYPE_Y && g_aCommand[1] == COMMANDTYPE_Y && g_aCommand[2] == COMMANDTYPE_Y)
		{

		}

		//赤赤緑
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_G)
		{

		}

		//赤黄黄
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}

		//青青緑
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}

		//青緑緑
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}

		//青黄黄
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}

		//緑緑黄
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}

		//赤緑青
		if (g_aCommand[0] == COMMANDTYPE_R && g_aCommand[1] == COMMANDTYPE_R && g_aCommand[2] == COMMANDTYPE_R)
		{

		}
	}
	else if (nCntCommand < MAX_COMMAND)
	{
		return MAGICTYPE_NONE;
	}
}

//魔法の設定処理==============================
void SetMagic(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move)
{

}

//魔法の設定処理==============================
void SetMagicPosition(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

}