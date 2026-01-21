//==========================================
// 
// player処理[player.h]
// Author : Kajino Hiyori
// 
//==========================================
#ifndef _Player_H_
#define	_Player_H_

// マクロ定義
#define MAX_MAT	(16)	// マテリアルの最大数

// モデルの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// プレイヤーの位置
	D3DXVECTOR3		move;		// プレイヤーの移動量
	D3DXVECTOR3		rot;		// プレイヤーの向き
	D3DXVECTOR3		rotDest;	// プレイヤーの目的の向き
	int				nIdxShadow;	// 影のインデックス
	bool			bJump;		// ジャンプ状態
	bool			bUse;		// 使用状態
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
#endif
