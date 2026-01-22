//===================================================================
//
// magic処理[magic.h]
// Author : 眞鍋快陸
// 
//===================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//魔法の種類
typedef enum
{
	MAGICTYPE_NONE = 0,
	MAGICTYPE_GGG,			//緑緑緑
	MAGICTYPE_RRR,			//赤赤赤
	MAGICTYPE_BBB,			//青青青
	MAGICTYPE_YYY,			//黄黄黄
	MAGICTYPE_RRG,			//赤赤緑
	MAGICTYPE_RYY,			//赤黄黄
	MAGICTYPE_BBG,			//青青緑
	MAGICTYPE_BGG,			//青緑緑
	MAGICTYPE_BYY,			//青黄黄
	MAGICTYPE_GGY,			//緑緑黄
	MAGICTYPE_RGB,			//赤緑青
	MAGICTYPE_MAX
}MAGICTYPE;

//コマンドの並び順
typedef enum
{
	//３つ同じ並び
	COMMANDOREDER_GGG,			//緑緑緑
	COMMANDOREDER_RRR,			//赤赤赤
	COMMANDOREDER_BBB,			//青青青
	COMMANDOREDER_YYY,			//黄黄黄
	//赤赤緑 | 赤緑赤 | 緑赤赤
	COMMANDOREDER_RRG,
	COMMANDOREDER_RGR,
	COMMANDOREDER_GRR,
	//赤黄黄 | 黄赤黄 | 黄黄赤
	COMMANDOREDER_RYY,
	COMMANDOREDER_YRY,
	COMMANDOREDER_TTR,
	//青青緑 | 青緑青 | 緑青青
	COMMANDOREDER_BBG,
	COMMANDOREDER_BGB,
	COMMANDOREDER_GBB,
	//青緑緑 | 緑青緑 | 緑緑青
	COMMANDOREDER_BGG,
	COMMANDOREDER_GBG,
	COMMANDOREDER_GGB,
	//青黄黄 | 黄青黄 | 黄黄青
	COMMANDOREDER_BYY,
	COMMANDOREDER_YBY,
	COMMANDOREDER_YYB,
	//緑緑黄 | 緑黄緑 | 黄緑緑
	COMMANDOREDER_GGY,
	COMMANDOREDER_GYG,
	COMMANDOREDER_YGG,
	//赤緑青
	COMMANDOREDER_RGB,

	COMMANDOREDER_MAX
}CommandOrder;

//コマンドの種類
typedef enum
{
	COMMANDTYPE_NONE = 0,	//何も入力されていないとき
	COMMANDTYPE_R,			//赤のコマンド
	COMMANDTYPE_G,			//緑のコマンド
	COMMANDTYPE_B,			//青のコマンド
	COMMANDTYPE_Y,			//黄のコマンド
	COMMANDTYPE_MAX
}COMMANDTYPE;

typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	MAGICTYPE mType;
	int nLife;
	int nIdxShadow;
	bool bUse;
}Magic;

void InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);

//入力されたコマンドを配列に格納(playerのMAGICBOOKから魔導書を持っているか確認してセット)
MAGICTYPE PressCommand(void);

//魔法の発動(コマンドの入力回数をカウント)
void SetMagic(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);

//フィール都内に魔法を設置
void SetMagicPosition(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
