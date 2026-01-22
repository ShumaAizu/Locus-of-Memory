//==========================================
// 
// 3Dポリゴンの描画[Player.h]
// 
//==========================================
#ifndef _Player_H_
#define	_Player_H_

// マクロ定義
#define MAX_MAT	(16)	// マテリアルの最大数

// モデルの構造体
typedef struct
{
	LPD3DXMESH			pMesh;					// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアルへのポインタ
	DWORD				dwNumMat;				// マテリアルの数
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス
	LPDIRECT3DTEXTURE9	apTexture[MAX_MAT];		// テクスチャへの	ポインタ
	D3DXVECTOR3			pos;					// プレイヤーの位置
	D3DXVECTOR3			move;					// プレイヤーの位置
	D3DXVECTOR3			rot;					// プレイヤーの向き
	int					nIdxShadow;				// 影のインデックス
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
#endif
