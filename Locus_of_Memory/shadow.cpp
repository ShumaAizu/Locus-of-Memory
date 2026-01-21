//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[shadow.cpp]
//
//======================================================================================
#include "main.h"
#include "shadow.h"
#include "input.h"
#include "debugproc.h"

// マクロ定義
#define NUM_SHADOW	(SHADOWTYPE_MAX)				// 影の種類の最大数
#define WIDTH		(10.0f)							// 幅
#define DEPTH		(10.0f)							// 奥行
#define HEIGHT		(100.0f)						// 高さ
#define NORMAL		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 基本の法線
#define DEFAULT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 基本の座標
#define MOVE		(1.5f)							// 移動量
#define ROTATE		(0.05f)							// 回転量
#define SIZE		(10.0f)							// 大きさ
#define COLOR_05fSHADOW		(D3DXCOLOR(0.500f,	0.500f,	0.500f,	1.000f))	// 薄い影
#define COLOR_03fSHADOW		(D3DXCOLOR(0.300f,	0.300f,	0.300f,	1.000f))	// 濃い影

// テクスチャの読み込み
const char* c_apFilenameShadow[NUM_SHADOW] =
{
	"data\\TEXTURE\\shadow000.jpg",	// [TSHADOWTYPE_CIRCLE]
	"data\\TEXTURE\\shadow001.jpg",	// [TSHADOWTYPE_SQUARE]
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureShadow[NUM_SHADOW] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];

//======================================================================================
// 影の初期化処理
//======================================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntShadow = 0; nCntShadow < NUM_SHADOW; nCntShadow++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameShadow[nCntShadow], &g_apTextureShadow[nCntShadow]);
	}

	// 初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = DEFAULT;
		g_aShadow[nCntShadow].rot = DEFAULT;
		g_aShadow[nCntShadow].fWidth = 0.0f;
		g_aShadow[nCntShadow].fDepth = 0.0f;
		g_aShadow[nCntShadow].col = COLOR_03fSHADOW;
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

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, +g_aShadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidth, 0.0f, +g_aShadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = g_aShadow[nCntShadow].col;
		pVtx[1].col = g_aShadow[nCntShadow].col;
		pVtx[2].col = g_aShadow[nCntShadow].col;
		pVtx[3].col = g_aShadow[nCntShadow].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//======================================================================================
// 影の終了処理
//======================================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	for (int nCntShadow = 0; nCntShadow < NUM_SHADOW; nCntShadow++)
	{
		if (g_apTextureShadow[nCntShadow] != NULL)
		{
			g_apTextureShadow[nCntShadow]->Release();
			g_apTextureShadow[nCntShadow] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//======================================================================================
// 影の更新処理
//======================================================================================
void UpdateShadow(void)
{
	Shadow* pShadow = GetShadow();
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++, pShadow++)
	{
		if (pShadow->bUse == true)
		{
			for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
			{
				if (pShadow->pos.y == g_aShadow[nCntShadow].pos.y && nCnt != nCntShadow && g_aShadow[nCntShadow].bUse == true)
				{ // 重なりを軽減
					pShadow->pos.y += 0.01f;
				}
			}
		}
	}
}

//======================================================================================
// 影の描画処理
//======================================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP,		D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,	D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
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
			pDevice->SetTexture(0, g_apTextureShadow[g_aShadow[nCntShadow].type]);

			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// 減算合成を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//======================================================================================
// 影の設定
//======================================================================================
int SetShadow(SHADOWTYPE type, float fWidth, float fDepth)
{
	int nCntShadow = -1;

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].fWidth = fWidth;
			g_aShadow[nCntShadow].fDepth = fDepth;
			g_aShadow[nCntShadow].type = type;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(- g_aShadow[nCntShadow].fWidth, 0.0f, + g_aShadow[nCntShadow].fDepth);
			pVtx[1].pos = D3DXVECTOR3(+ g_aShadow[nCntShadow].fWidth, 0.0f, + g_aShadow[nCntShadow].fDepth);
			pVtx[2].pos = D3DXVECTOR3(- g_aShadow[nCntShadow].fWidth, 0.0f, - g_aShadow[nCntShadow].fDepth);
			pVtx[3].pos = D3DXVECTOR3(+ g_aShadow[nCntShadow].fWidth, 0.0f, - g_aShadow[nCntShadow].fDepth);
			
			// 頂点カラーの設定
			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;

			g_aShadow[nCntShadow].bUse = true;
			break;
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
	return nCntShadow;
}

//======================================================================================
// 影の位置を更新
//======================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].bUse = true;
	g_aShadow[nIdxShadow].pos = pos;
	while (1)
	{
		g_aShadow[nIdxShadow].pos.y -= 0.5f;	// 全く同じ高度で描画を行うとちらつくので、少し浮かせることでこの問題を解決する
	
		if (g_aShadow[nIdxShadow].pos.y <= 0.0f)
		{
			g_aShadow[nIdxShadow].pos.y = 0.0f;
			break;
		}
	}
}

//======================================================================================
// 影を使用していない状態にする
//======================================================================================
void ResetShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
	g_aShadow[nIdxShadow].pos = DEFAULT;

}

//======================================================================================
// 影の情報を渡す
//======================================================================================
Shadow* GetShadow(void)
{
	return &g_aShadow[0];
}

//======================================================================================
// 高度によって影の色を変更
//======================================================================================
void SetShadowColorData(int nIdxShadow, float fPosYOld, float fPosY)
{
	float fWidth = g_aShadow[nIdxShadow].fWidth + 5.0f;
	float fDepth = g_aShadow[nIdxShadow].fDepth + 5.0f;

	if (fPosYOld - fPosY <= 10.0f)
	{
		g_aShadow[nIdxShadow].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
	}
	else if (fPosYOld - fPosY >= 750.0f)
	{
		g_aShadow[nIdxShadow].col = D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f);
	}
	else if (fPosYOld - fPosY >= 30.0f)
	{
		g_aShadow[nIdxShadow].col = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);
	}
	else
	{
		g_aShadow[nIdxShadow].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}
	
	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdxShadow * 4;

	// 頂点カラーの設定
	pVtx[0].col = g_aShadow[nIdxShadow].col;
	pVtx[1].col = g_aShadow[nIdxShadow].col;
	pVtx[2].col = g_aShadow[nIdxShadow].col;
	pVtx[3].col = g_aShadow[nIdxShadow].col;

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}
