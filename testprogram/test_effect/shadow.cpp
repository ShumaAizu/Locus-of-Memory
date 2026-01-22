//========================================================================
// 
// 3D影の描画[shadow.cpp]
//
//========================================================================
#include "main.h"
#include "shadow.h"
#include "field.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define MAX_SHADOW	(256)							// 影の最大数
#define WIDTH		(20.0f)						// 幅
#define DEPTH		(20.0f)						// 奥行
#define HEIGHT		(100.0f)						// 高さ
#define NORMAL		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 基本の法線
#define DEFAULT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 基本の座標
#define MOVE		(1.5f)							// 移動量
#define ROTATE		(0.05f)							// 回転量

// 影の構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	bool bUse;				// 使用状態
}Shadow;

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];

//========================================================================
// 影の初期化処理
//========================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\shadow000.jpg",&g_pTextureShadow);

	// 初期化
	for (int nCntShadow = RESET_DATA; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = DEFAULT;
		g_aShadow[nCntShadow].rot = DEFAULT;
		g_aShadow[nCntShadow].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = RESET_DATA; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - WIDTH, 0.0f, g_aShadow[nCntShadow].pos.z + DEPTH);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + WIDTH, 0.0f, g_aShadow[nCntShadow].pos.z + DEPTH);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - WIDTH, 0.0f, g_aShadow[nCntShadow].pos.z - DEPTH);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + WIDTH, 0.0f, g_aShadow[nCntShadow].pos.z - DEPTH);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_SHADOW;
		pVtx[1].col = COLOR_SHADOW;
		pVtx[2].col = COLOR_SHADOW;
		pVtx[3].col = COLOR_SHADOW;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//========================================================================
// 影の終了処理
//========================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//========================================================================
// 影の更新処理
//========================================================================
void UpdateShadow(void)
{
	
}

//========================================================================
// 影の描画処理
//========================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP,		D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,	D3DBLEND_ONE);

	for (int nCntShadow = RESET_DATA; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// 減算合成を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================================================
// 影の設定
//========================================================================
int SetShadow(void)
{
	int nCntShadow = -1;
	for (nCntShadow = RESET_DATA; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].bUse = true;
			break;
		}
	}
	return nCntShadow;
}

//========================================================================
// 影の位置を更新
//========================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 fieldPos = GetFieldPos();

	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].pos.y = fieldPos.y + 0.01f;	// 全く同じ高度で描画を行うとちらつくので、少し浮かせることでこの問題を解決する
}
