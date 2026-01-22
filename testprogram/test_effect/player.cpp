//========================================================================
// 
// 3Dポリゴンの描画[player.cpp]
//
//========================================================================
#include "main.h"
#include "player.h"
#include "shadow.h"
#include "camera.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define WIDTH		(100.0f)						// 幅
#define DEPTH		(100.0f)						// 奥行
#define HEIGHT		(100.0f)						// 高さ
#define MOVE		(0.05f)							// 移動量
#define ROTATE		(0.05f)							// 回転量
#define PI			(D3DX_PI)						// 円周率
#define RIGHT		(D3DX_PI / 2)					// 右を向く
#define LEFT		(-(D3DX_PI / 2))				// 左を向く
#define BACK		(D3DX_PI)						// 後ろを向く
#define FRONT		(0.0f)							// 正面を向く
#define RIGHT_BACK	(D3DX_PI / 4 + D3DX_PI / 2)		// 右後ろ
#define RIGHT_FRONT	(D3DX_PI / 4)					// 右手前
#define LEFT_BACK	(-(D3DX_PI / 4 + D3DX_PI / 2))	// 左後ろ
#define LEFT_FRONT	(-(D3DX_PI / 4))				// 左手前
#define POS			(D3DXVECTOR3(0.0f, 10.0f, 0.0f))	// プレイヤーの位置
#define DEFALT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// xyzが0.0fの場合
#define NORMAL		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 基本の法線

// グローバル変数
Player g_player;	// プレイヤーの情報を格納

//========================================================================
// プレイヤーの初期化処理
//========================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;

	// 初期化
	g_player.pMesh = NULL;
	g_player.pBuffMat = NULL;
	g_player.dwNumMat = RESET_DATA;
	for (int nCntMat = RESET_DATA; nCntMat < MAX_MAT; nCntMat++)
	{
		g_player.apTexture[nCntMat] = NULL;
	}
	g_player.pos = POS;
	g_player.rot = DEFALT;
	g_player.move = DEFALT;

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\airplane000.x",
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_player.pBuffMat,
						NULL,
						&g_player.dwNumMat,
						&g_player.pMesh);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_player.pBuffMat->GetBufferPointer();

	for (int nCntMat = RESET_DATA; nCntMat < (int)g_player.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_player.apTexture[nCntMat]);
		}
	}

	// 影のIDを設定
	g_player.nIdxShadow = SetShadow();
}

//========================================================================
// プレイヤーの終了処理
//========================================================================
void UninitPlayer(void)
{
	// メッシュの破棄
	if (g_player.pMesh != NULL)
	{
		g_player.pMesh->Release();
		g_player.pMesh = NULL;
	}

	// テクスチャの破棄
	for (int nCntMat = RESET_DATA; nCntMat < (int)g_player.dwNumMat; nCntMat++)
	{
		if (g_player.apTexture[nCntMat] != NULL)
		{
			g_player.apTexture[nCntMat]->Release();
			g_player.apTexture[nCntMat] = NULL;
		}
	}

	// マテリアルの破棄
	if (g_player.pBuffMat != NULL)
	{
		g_player.pBuffMat->Release();
		g_player.pBuffMat = NULL;
	}
}

//========================================================================
// プレイヤーの更新処理
//========================================================================
void UpdatePlayer(void)
{
	Camera *pCamera = GetCamera();

	// 移動を管理
	if (GetKeyboardPress(DIK_LEFTARROW) == true)	// 右に移動
	{
		g_player.move.x -= cosf(pCamera->rot.y) * MOVE;
		g_player.move.z += sinf(pCamera->rot.y) * MOVE;

		g_player.move.x -= cosf(pCamera->rot.y) * MOVE;
		g_player.move.z += sinf(pCamera->rot.y) * MOVE;
	}
	else if (GetKeyboardPress(DIK_RIGHTARROW) == true)	// 左に移動
	{
		g_player.move.x += cosf(pCamera->rot.y) * MOVE;
		g_player.move.z -= sinf(pCamera->rot.y) * MOVE;

		g_player.move.x += cosf(pCamera->rot.y) * MOVE;
		g_player.move.z -= sinf(pCamera->rot.y) * MOVE;

		g_player.rot.y = pCamera->rot.y - D3DX_PI / 2;
	}

	if (GetKeyboardPress(DIK_UPARROW) == true)	// 奥に移動
	{
		g_player.move.x += sinf(pCamera->rot.y) * MOVE;
		g_player.move.z += cosf(pCamera->rot.y) * MOVE;

		g_player.move.x += sinf(pCamera->rot.y) * MOVE;
		g_player.move.z += cosf(pCamera->rot.y) * MOVE;	
	}
	else if (GetKeyboardPress(DIK_DOWNARROW) == true)	// 手前に移動
	{
		g_player.move.x -= sinf(pCamera->rot.y) * MOVE;
		g_player.move.z -= cosf(pCamera->rot.y) * MOVE;

		g_player.move.x -= sinf(pCamera->rot.y) * MOVE;
		g_player.move.z -= cosf(pCamera->rot.y) * MOVE;
	}

	// プレイヤーの方向を設定
	if (GetKeyboardPress(DIK_LEFTARROW) == true)	// 右に移動
	{
		if (GetKeyboardPress(DIK_UPARROW) == true)	// 奥に移動
		{
			g_player.rot.y = pCamera->rot.y + D3DX_PI / 2 + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(DIK_DOWNARROW) == true)	// 手前に移動
		{
			g_player.rot.y = pCamera->rot.y + D3DX_PI / 2 - D3DX_PI / 4;
		}
		else
		{
			g_player.rot.y = pCamera->rot.y + D3DX_PI / 2;
		}
	}
	else if (GetKeyboardPress(DIK_RIGHTARROW) == true)	// 左に移動
	{
		if (GetKeyboardPress(DIK_UPARROW) == true)	// 奥に移動
		{
			g_player.rot.y = pCamera->rot.y - D3DX_PI / 2 - D3DX_PI / 4;
		}
		else if (GetKeyboardPress(DIK_DOWNARROW) == true)	// 手前に移動
		{
			g_player.rot.y = pCamera->rot.y - D3DX_PI / 2 + D3DX_PI / 4;
		}
		else
		{
			g_player.rot.y = pCamera->rot.y - D3DX_PI / 2;
		}
	}
	else if (GetKeyboardPress(DIK_UPARROW) == true)	// 奥に移動
	{
		g_player.rot.y = pCamera->rot.y + D3DX_PI;
	}
	else if (GetKeyboardPress(DIK_DOWNARROW) == true)	// 手前に移動
	{
		g_player.rot.y = pCamera->rot.y;
	}



#if 0
	if (GetKeyboardPress(DIK_I) == true)	// 上に移動
	{
		g_player.move.y += MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)	// 下に移動
	{
		g_player.move.y -= MOVE;
	}

	// 回転を管理
	if (GetKeyboardPress(DIK_UPARROW) == true)	// x軸回転
	{
		g_player.rot.x += ROTATE;
	}
	else if (GetKeyboardPress(DIK_DOWNARROW) == true)	// x軸回転
	{
		g_player.rot.x -= ROTATE;
	}
	if (GetKeyboardPress(DIK_LSHIFT) == true)	// y軸回転
	{
		g_player.rot.y += ROTATE;
	}
	else if (GetKeyboardPress(DIK_RSHIFT) == true)	// y軸回転
	{
		g_player.rot.y -= ROTATE;
	}
	if (GetKeyboardPress(DIK_LEFTARROW) == true)	// z軸回転
	{
		g_player.rot.z += ROTATE;
	}
	else if (GetKeyboardPress(DIK_RIGHTARROW) == true)	// z軸回転
	{
		g_player.rot.z -= ROTATE;
	}
#endif

	// 位置の更新
	g_player.pos += g_player.move;

	// 移動量の更新
	g_player.move.x += (0.0f - g_player.move.x) * 0.1f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.1f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.1f;


	// Enterキーでリセット
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_player.pos = POS;
		g_player.rot = DEFALT;
		g_player.move = DEFALT;
	}

	// 影の位置を更新
	SetPositionShadow(g_player.nIdxShadow, g_player.pos);
}

//========================================================================
// プレイヤーの描画処理
//========================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルを保存
	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_player.pBuffMat->GetBufferPointer();

	// マテリアルごとにプレイヤーを描画
	for (int nCntMat = RESET_DATA; nCntMat < (int)g_player.dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_player.apTexture[nCntMat]);

		// プレイヤーパーツの描画
		g_player.pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルに戻す
	pDevice->SetMaterial(&matDef);
}

//========================================================================
// プレイヤーの情報を渡す
//========================================================================
Player* GetPlayer(void)
{
	return &g_player;
}