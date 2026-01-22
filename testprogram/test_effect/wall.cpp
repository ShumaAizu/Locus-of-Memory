//========================================================================
// 
// 3Dフィールドの描画[wall.cpp]
//
//========================================================================
#include "wall.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define MAX_WALL	(4)										// 壁の最大数
#define WIDTH		(100.0f)								// 幅
#define DEPTH		(100.0f)								// 奥行
#define HEIGHT		(200.0f)								// 高さ
#define NORMAL		(D3DXVECTOR3(0.0f, 0.0f, -1.0f))		// 基本の法線
#define ROT_BACK	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))			// 奥の壁
#define ROT_FRONT	(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f))		// 手前の壁
#define ROT_LEFT	(D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f))	// 左の壁
#define ROT_RIGHT	(D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f))	// 右の壁

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	// 頂点バッファへのポインタ
Wall g_aWall[MAX_WALL];

//========================================================================
// 壁の初期化処理
//========================================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\wall000.jpg",&g_pTextureWall);

	// 初期化
	for (int nCntWall = RESET_DATA; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].bUse = false;
	}
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntWall = RESET_DATA; nCntWall < MAX_WALL; nCntWall++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f - WIDTH, 0.0f + HEIGHT, 100.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f + WIDTH, 0.0f + HEIGHT, 100.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f - WIDTH, 0.0f, 100.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f + WIDTH, 0.0f, 100.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();

	// 壁の設置
	SetWall(ROT_BACK);	// 奥の壁
	SetWall(ROT_FRONT);	// 手前の壁
	SetWall(ROT_LEFT);	// 左の壁
	SetWall(ROT_RIGHT);	// 右の壁
}

//========================================================================
// 壁の終了処理
//========================================================================
void UninitWall(void)
{
	// テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//========================================================================
// 壁の更新処理
//========================================================================
void UpdateWall(void)
{
	
}

//========================================================================
// 壁の描画処理
//========================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int nCntWall = RESET_DATA; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			// フィールドの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}

//========================================================================
// 壁のセット
//========================================================================
void SetWall(D3DXVECTOR3 rot)
{
	for (int nCntWall = RESET_DATA; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].bUse = true;
			break;
		}
	}
}

//========================================================================
// フィールドの座標を渡す
//========================================================================
D3DXVECTOR3 GetWallPos(void)
{
	return g_aWall[0].pos;
}