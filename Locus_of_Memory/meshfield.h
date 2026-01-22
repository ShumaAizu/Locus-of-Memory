//=============================================================================
//
//	メッシュフィールド処理 [meshfield.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_WIDTH				(100.0f)			// メッシュフィールドの幅
#define MESHFIELD_DEPTH				(100.0f)			// メッシュフィールドの奥行き
#define ICE_INERTIA					(0.0275f)			// 氷面歩行時の慣性
#define SNOW_INERTIA				(0.0525f)			// 雪面歩行時の慣性
#define MAX_VTX						(1000)				// 頂点数の最大

//*****************************************************************************
// メッシュフィールドの種類
//*****************************************************************************
typedef enum MESHFIELDTYPE
{// 仮置きで数値
	MESHFIELDTYPE_000 = 0,
	MESHFIELDTYPE_001,
	MESHFIELDTYPE_002,
	MESHFIELDTYPE_003,
	MESHFIELDTYPE_MAX
}MESHFIELDTYPE;

//*****************************************************************************
// メッシュフィールドの構造体定義
//*****************************************************************************
typedef struct MeshField
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR2 tex;							// テクスチャ座標
	MESHFIELDTYPE type;							// 種類
	int nSplitWidth;							// 幅の分割数
	int nSplitDepth;							// 奥行きの分割数
	float fWidth;								// 幅
	float fDepth;								// 奥行き
	bool bUse;									// 使用状態
	bool bMesh;									// メッシュかどうか
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXMATRIX *mtxParent;						// 親マトリックス
}MeshField;

typedef struct MeshField* PMESHFIELD;

//*****************************************************************************
// メッシュ情報構造体定義
//*****************************************************************************
typedef struct MeshInfo
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	MESHFIELDTYPE type;							// 種類
	D3DXVECTOR3 VtxPos[MAX_VTX];				// 頂点の位置
	int nVtx;									// 頂点数
	int nSplitWidth;							// 幅の分割数
	int nSplitDepth;							// 奥行きの分割数
}MeshInfo;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth, int nSplitWidth, int nSplitDepth, MESHFIELDTYPE type);
PMESHFIELD CollisionMeshField(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove);
void SetMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nVtx, D3DXVECTOR3* VtxPos, int nSplitWidth, int nSplitDepth, MESHFIELDTYPE type, D3DXMATRIX* pmtxParent);
void LoadMesh(const char* pFileName, MeshInfo* MeshInfo);

#endif