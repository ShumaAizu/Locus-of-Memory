//========================================================
// 
// 影の管理処理[shadow.h]
// 
//========================================================
#ifndef _SHADOW_H_
#define	_SHADOW_H_

// マクロ定義
#define MAX_SHADOW	(1024)	// 影の最大数

// 影の種類
typedef enum
{
	SHADOWTYPE_CIRCLE = 0,	// 円形の影
	SHADOWTYPE_SQUARE,		// 四角の影
	SHADOWTYPE_MAX
}SHADOWTYPE;

// 影の構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	SHADOWTYPE type;		// 影の種類
	float fWidth;			// 大きさ[幅]
	float fDepth;			// 大きさ[奥行き]
	D3DXCOLOR col;			// 色
	bool bUse;				// 使用状態
}Shadow;

// プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(SHADOWTYPE type, float fWidth, float fDepth);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void ResetShadow(int nIdxShadow);
Shadow* GetShadow(void);
void SetShadowColorData(int nIdxShadow, float fPosYOld, float fPosY);

#endif
