//=============================================================================
//
//	Locus of Memory [title.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _Title_H        // このマクロ定義がされなかったら
#define _Title_H        // 2重インクルード防止のマクロを定義する

// タイトルの状態
typedef enum
{
	TITLESTATE_MOVETITLE = 0,	// タイトルが出てる状態
	TITLESTATE_STOPTITLE,		// タイトルが止まってる状態
	TITLESTATE_MAX

}TITLESTATE;

// スタートの状態
typedef enum
{
	STARTSTATE_NORMAL = 0,	// 通常状態
	STARTSTATE_MOVE,		// 点滅状態
	STARTSTATE_MAX

}STARTSTATE;

 // タイトルの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動
	TITLESTATE state;		// 状態

}TITLE;

 // スタートの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	STARTSTATE state;		// 状態

}START;

// プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif