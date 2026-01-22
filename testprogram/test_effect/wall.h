//==========================================
// 
// 3Dポリゴンの描画[wall.h]
// 
//==========================================
#ifndef _WALL_H_
#define	_WALL_H_

#include "main.h"

// 壁の構造体
typedef struct
{
	D3DXMATRIX	mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	bool bUse;				// 使用状態
}Wall;

// プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 rot);
D3DXVECTOR3 GetWallPos(void);

#endif
