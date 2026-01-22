//========================================================================
// 
// 3Dフィールドの描画[field.cpp]
//
//========================================================================
#include "field.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define WIDTH	(100.0f)						// 幅
#define DEPTH	(100.0f)						// 奥行
#define HEIGHT	(100.0f)						// 高さ
#define NORMAL	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 基本の法線
#define MOVE	(1.5f)							// 移動量
#define ROTATE	(0.05f)							// 回転量

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX g_mtxWorldField;						// ワールドマトリックス
D3DXVECTOR3 g_posField;							// フィールドの位置
D3DXVECTOR3 g_rotField;							// フィールドの向き

//========================================================================
// フィールドの初期化処理
//========================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\field001.jpg",&g_pTextureField);

	// 初期化
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posField.x - WIDTH, 0.0f, g_posField.z + DEPTH);
	pVtx[1].pos = D3DXVECTOR3(g_posField.x + WIDTH, 0.0f, g_posField.z + DEPTH);
	pVtx[2].pos = D3DXVECTOR3(g_posField.x - WIDTH, 0.0f, g_posField.z - DEPTH);
	pVtx[3].pos = D3DXVECTOR3(g_posField.x + WIDTH, 0.0f, g_posField.z - DEPTH);

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

	// 頂点バッファをアンロック
	g_pVtxBuffField->Unlock();
}

//========================================================================
// フィールドの終了処理
//========================================================================
void UninitField(void)
{
	// テクスチャの破棄
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//========================================================================
// フィールドの更新処理
//========================================================================
void UpdateField(void)
{
	
}

//========================================================================
// フィールドの描画処理
//========================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 向きを反映
	// 向きから反映しないと回転軸がずれる
	// YawはY軸、PitchはX軸(高度)、RollはZ軸を表すため、変数の指定に注意
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureField);

	// フィールドの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================================
// フィールドの座標を渡す
//========================================================================
D3DXVECTOR3 GetFieldPos(void)
{
	return g_posField;
}