//========================================================================
// 
// player処理[player.cpp]
// Author : Kajino Hiyori
//
//========================================================================
#include "main.h"
#include "player.h"
#include "shadow.h"
#include "camera.h"
#include "title.h"
#include "input.h"

// マクロ定義
#define MAX_PLAYER		(2)									// プレイヤーの最大数
#define MAX_MODEL		(1)									// モデルの最大数
#define MOVE			(0.25f)								// 移動量
#define ROTATE			(0.9f)								// 回転量
#define GRAVITY			(0.75f)				// 重力
#define JUMP			(17.0f)				// ジャンプ
#define SHADOｗ			(10.0f)								// 影の大きさ
#define CORRECTION_ROT	(0.075f)							// 回転の減衰係数
#define PI				(D3DX_PI)							// 円周率
#define RIGHT			(D3DX_PI / 2)						// 右を向く
#define LEFT			(-(D3DX_PI / 2))					// 左を向く
#define BACK			(D3DX_PI)							// 後ろを向く
#define FRONT			(0.0f)								// 正面を向く
#define LENGTH			(50.0f)								// フィールドの範囲
#define POS				(D3DXVECTOR3(0.0f, 20.0f, 0.0f))	// プレイヤーの位置
#define DEFALT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// xyzが0.0fの場合
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 基本の法線

// グローバル変数
LPD3DXMESH			g_pMeshPlayer[MAX_PLAYER] = {};				// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER		g_pBuffMatPlayer[MAX_PLAYER] = {};			// マテリアルへのポインタ
DWORD				g_dwNumMatPlayer[MAX_PLAYER] = {0, 0};		// マテリアルの数
LPDIRECT3DTEXTURE9	g_apTexturePlayer[MAX_PLAYER][MAX_MAT];		// テクスチャへの	ポインタ
Player				g_aPlayer[MAX_PLAYER];						// プレイヤーの情報を格納

// 1Pのモデル
const char* c_apFilename1PModel[MAX_MODEL] =
{
	"data\\MODEL\\player\\airplane000.x",	// 胴体[0]
};

// 2Pのモデル
const char* c_apFilename2PModel[MAX_MODEL] =
{
	"data\\MODEL\\player\\tank000.x",	// 胴体[0]
};

//========================================================================
// プレイヤーの初期化処理
//========================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = DEFALT;
		g_aPlayer[nCntPlayer].rot = DEFALT;
		g_aPlayer[nCntPlayer].rotDest = DEFALT;
		g_aPlayer[nCntPlayer].move = DEFALT;
		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = false;
		g_aPlayer[nCntPlayer].bJump = false;

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			switch (nCntPlayer)
			{
			case 0:	// 1P
				// Xファイルの読み込み
				D3DXLoadMeshFromX(c_apFilename1PModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer[nCntPlayer], NULL, &g_dwNumMatPlayer[nCntPlayer], &g_pMeshPlayer[nCntPlayer]);
				break;

			case 1:	// 2P
				D3DXLoadMeshFromX(c_apFilename2PModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer[nCntPlayer], NULL, &g_dwNumMatPlayer[nCntPlayer], &g_pMeshPlayer[nCntPlayer]);
				break;
			}

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatPlayer[nCntPlayer]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer[nCntPlayer]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTexturePlayer[nCntPlayer][nCntMat]);
				}
			}
		}
	}

	OPERATIONTYPE operationtyoe = GetOperationType();
	SetPlayer(0, POS, DEFALT);
	if (operationtyoe == OPERATIONTYPE_2P)
	{
		SetPlayer(1, DEFALT, DEFALT);
	}
}

//========================================================================
// プレイヤーの終了処理
//========================================================================
void UninitPlayer(void)
{
	// メッシュの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_pMeshPlayer[nCntPlayer]!= NULL)
		{
			g_pMeshPlayer[nCntPlayer]->Release();
			g_pMeshPlayer[nCntPlayer] = NULL;
		}
	}
	// テクスチャの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer[nCntPlayer]; nCntMat++)
		{
			if (g_apTexturePlayer[nCntPlayer][nCntMat] != NULL)
			{
				g_apTexturePlayer[nCntPlayer][nCntMat]->Release();
				g_apTexturePlayer[nCntPlayer][nCntMat] = NULL;
			}
		}
	}

	// マテリアルの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_pBuffMatPlayer[nCntPlayer] != NULL)
		{
			g_pBuffMatPlayer[nCntPlayer]->Release();
			g_pBuffMatPlayer[nCntPlayer] = NULL;
		}
	}
}

//========================================================================
// プレイヤーの更新処理
//========================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();
	// スティックの角度を取得
	float fRotDiffKey = 0.0f;	// キー入力時の角度補正計算用
	float fRotDiffPad = 0.0f;	// ジョイパッド入力時の角度補正計算用
	
	// キーボード操作[1P] / WASD
	if (g_aPlayer[0].bUse == true)
	{
		// プレイヤーの移動を管理
		if (GetKeyboardPress(DIK_A) == true)	// 右に移動
		{
			if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
			{
				g_aPlayer[0].move.x -= sinf(pCamera[0].rot.y - D3DX_PI / 4) * MOVE;
				g_aPlayer[0].move.z -= cosf(pCamera[0].rot.y - D3DX_PI / 4) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
			{
				g_aPlayer[0].move.x += sinf(pCamera[0].rot.y + D3DX_PI / 4) * MOVE;
				g_aPlayer[0].move.z += cosf(pCamera[0].rot.y + D3DX_PI / 4) * MOVE;
			}
			else
			{
				g_aPlayer[0].move.x += cosf(pCamera[0].rot.y) * MOVE;
				g_aPlayer[0].move.z -= sinf(pCamera[0].rot.y) * MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)	// 左に移動
		{
			if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
			{
				g_aPlayer[0].move.x -= cosf(pCamera[0].rot.y - D3DX_PI / 4) * MOVE;
				g_aPlayer[0].move.z += sinf(pCamera[0].rot.y - D3DX_PI / 4) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
			{
				g_aPlayer[0].move.x -= cosf(pCamera[0].rot.y + D3DX_PI / 4) * MOVE;
				g_aPlayer[0].move.z += sinf(pCamera[0].rot.y + D3DX_PI / 4) * MOVE;
			}
			else
			{
				g_aPlayer[0].move.x -= cosf(pCamera[0].rot.y) * MOVE;
				g_aPlayer[0].move.z += sinf(pCamera[0].rot.y) * MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
		{
			g_aPlayer[0].move.x -= sinf(pCamera[0].rot.y) * MOVE;
			g_aPlayer[0].move.z -= cosf(pCamera[0].rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
		{
			g_aPlayer[0].move.x += sinf(pCamera[0].rot.y) * MOVE;
			g_aPlayer[0].move.z += cosf(pCamera[0].rot.y) * MOVE;
		}

		// プレイヤーの向きを設定
		if (GetKeyboardPress(DIK_A) == true)	// 右に移動
		{
			if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
			{
				g_aPlayer[0].rotDest.y = pCamera[0].rot.y - D3DX_PI / 2 + D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
			{
				g_aPlayer[0].rotDest.y = pCamera[0].rot.y - D3DX_PI / 2 - D3DX_PI / 4;
			}
			else
			{
				g_aPlayer[0].rotDest.y = pCamera[0].rot.y - D3DX_PI / 2;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)	// 左に移動
		{
			if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
			{
				g_aPlayer[0].rotDest.y = pCamera[0].rot.y + D3DX_PI / 2 - D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
			{
				g_aPlayer[0].rotDest.y = pCamera[0].rot.y + D3DX_PI / 2 + D3DX_PI / 4;
			}
			else
			{
				g_aPlayer[0].rotDest.y = pCamera[0].rot.y + D3DX_PI / 2;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
		{
			g_aPlayer[0].rotDest.y = pCamera[0].rot.y;
		}
		else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
		{
			g_aPlayer[0].rotDest.y = pCamera[0].rot.y + D3DX_PI;
		}
	
		// ジャンプ処理
		if (GetKeyboardTrigger(DIK_SPACE) == true && g_aPlayer[0].bJump == false)
		{
			g_aPlayer[0].move.y = JUMP;
			g_aPlayer[0].bJump = true;
		}
	}

	// キーボード操作[2P] / IJKL
#ifdef _DEBUG
	if (g_aPlayer[1].bUse == true)
	{
		// プレイヤーの移動を管理
		if (GetKeyboardPress(DIK_J) == true)	// 右に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].move.x -= sinf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z -= cosf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].move.x += sinf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z += cosf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
			}
			else
			{
				g_aPlayer[1].move.x += cosf(pCamera[1].rot.y) * MOVE;
				g_aPlayer[1].move.z -= sinf(pCamera[1].rot.y) * MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_L) == true)	// 左に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].move.x -= cosf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z += sinf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].move.x -= cosf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z += sinf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
			}
			else
			{
				g_aPlayer[1].move.x -= cosf(pCamera[1].rot.y) * MOVE;
				g_aPlayer[1].move.z += sinf(pCamera[1].rot.y) * MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
		{
			g_aPlayer[1].move.x -= sinf(pCamera[1].rot.y) * MOVE;
			g_aPlayer[1].move.z -= cosf(pCamera[1].rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
		{
			g_aPlayer[1].move.x += sinf(pCamera[1].rot.y) * MOVE;
			g_aPlayer[1].move.z += cosf(pCamera[1].rot.y) * MOVE;
		}

		// プレイヤーの向きを設定
		if (GetKeyboardPress(DIK_J) == true)	// 右に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y - D3DX_PI / 2 + D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y - D3DX_PI / 2 - D3DX_PI / 4;
			}
			else
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y - D3DX_PI / 2;
			}
		}
		else if (GetKeyboardPress(DIK_L) == true)	// 左に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI / 2 - D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI / 2 + D3DX_PI / 4;
			}
			else
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI / 2;
			}
		}
		else if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
		{
			g_aPlayer[1].rotDest.y = pCamera[1].rot.y;
		}
		else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
		{
			g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI;
		}

		// ジャンプ処理
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_aPlayer[1].bJump == false)
		{
			g_aPlayer[1].move.y = JUMP;
			g_aPlayer[1].bJump = true;
		}
	}
#endif	
	
	// ジョイパッド[十字キー]操作
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			// 移動を管理
			if (GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true)	// 右に移動
			{
				if (GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
				{
					g_aPlayer[nCntPlayer].move.x -= sinf(pCamera[nCntPlayer].rot.y - D3DX_PI / 4) * MOVE;
					g_aPlayer[nCntPlayer].move.z -= cosf(pCamera[nCntPlayer].rot.y - D3DX_PI / 4) * MOVE;
				}
				else if (GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
				{
					g_aPlayer[nCntPlayer].move.x += sinf(pCamera[nCntPlayer].rot.y + D3DX_PI / 4) * MOVE;
					g_aPlayer[nCntPlayer].move.z += cosf(pCamera[nCntPlayer].rot.y + D3DX_PI / 4) * MOVE;
				}
				else
				{
					g_aPlayer[nCntPlayer].move.x += cosf(pCamera[nCntPlayer].rot.y) * MOVE;
					g_aPlayer[nCntPlayer].move.z -= sinf(pCamera[nCntPlayer].rot.y) * MOVE;
				}
			}
			else if (GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true)	// 左に移動
			{
				if (GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
				{
					g_aPlayer[nCntPlayer].move.x -= cosf(pCamera[nCntPlayer].rot.y - D3DX_PI / 4) * MOVE;
					g_aPlayer[nCntPlayer].move.z += sinf(pCamera[nCntPlayer].rot.y - D3DX_PI / 4) * MOVE;
				}
				else if (GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
				{
					g_aPlayer[nCntPlayer].move.x -= cosf(pCamera[nCntPlayer].rot.y + D3DX_PI / 4) * MOVE;
					g_aPlayer[nCntPlayer].move.z += sinf(pCamera[nCntPlayer].rot.y + D3DX_PI / 4) * MOVE;
				}
				else
				{
					g_aPlayer[nCntPlayer].move.x -= cosf(pCamera[nCntPlayer].rot.y) * MOVE;
					g_aPlayer[nCntPlayer].move.z += sinf(pCamera[nCntPlayer].rot.y) * MOVE;
				}
			}
			else if (GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
			{
				g_aPlayer[nCntPlayer].move.x -= sinf(pCamera[nCntPlayer].rot.y) * MOVE;
				g_aPlayer[nCntPlayer].move.z -= cosf(pCamera[nCntPlayer].rot.y) * MOVE;
			}
			else if (GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
			{
				g_aPlayer[nCntPlayer].move.x += sinf(pCamera[nCntPlayer].rot.y) * MOVE;
				g_aPlayer[nCntPlayer].move.z += cosf(pCamera[nCntPlayer].rot.y) * MOVE;
			}

			// プレイヤーの方向を設定
			if (GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true)	// 右に移動
			{
				if (GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
				{
					g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y - D3DX_PI / 2 + D3DX_PI / 4;
				}
				else if (GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
				{
					g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y - D3DX_PI / 2 - D3DX_PI / 4;
				}
				else
				{
					g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y - D3DX_PI / 2;
				}
			}
			else if (GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true)	// 左に移動
			{
				if (GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
				{
					g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y + D3DX_PI / 2 - D3DX_PI / 4;
				}
				else if (GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
				{
					g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y + D3DX_PI / 2 + D3DX_PI / 4;
				}
				else
				{
					g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y + D3DX_PI / 2;
				}
			}
			else if (GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
			{
				g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y;
			}
			else if (GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
			{
				g_aPlayer[nCntPlayer].rotDest.y = pCamera[nCntPlayer].rot.y + D3DX_PI;
			}

			if (GetJoypadTrigger(JOYKEY_A, nCntPlayer) == true && g_aPlayer[nCntPlayer].bJump == false)
			{
				g_aPlayer[nCntPlayer].move.y = JUMP;
				g_aPlayer[nCntPlayer].bJump = true;
			}
		}
	}
	
	// プレイヤーの方向を補正
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			g_aPlayer[nCntPlayer].rotDest.y = AngleNormalize(g_aPlayer[nCntPlayer].rotDest.y);
			g_aPlayer[nCntPlayer].rot.y = AngleNormalize(g_aPlayer[nCntPlayer].rot.y);

			fRotDiffKey = g_aPlayer[nCntPlayer].rotDest.y - g_aPlayer[nCntPlayer].rot.y;	// 差分を計算
			fRotDiffKey = AngleNormalize(fRotDiffKey);
			
			g_aPlayer[nCntPlayer].rot.y += (fRotDiffKey)*CORRECTION_ROT;
			g_aPlayer[nCntPlayer].rot.y = AngleNormalize(g_aPlayer[nCntPlayer].rot.y);

			// 重力
			g_aPlayer[nCntPlayer].move.y -= GRAVITY;

			// 位置の更新
			g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;

			// プレイヤーの行動範囲を制限
			if (g_aPlayer[nCntPlayer].pos.x < -LENGTH)	// 左の壁にぶつかったとき
			{
				g_aPlayer[nCntPlayer].pos.x = -LENGTH;
			}
			else if (g_aPlayer[nCntPlayer].pos.x > LENGTH)	// 右の壁にぶつかったとき
			{
				g_aPlayer[nCntPlayer].pos.x = LENGTH;
			}
			if (g_aPlayer[nCntPlayer].pos.z < -LENGTH)	// 手前の壁にぶつかったとき
			{
				g_aPlayer[nCntPlayer].pos.z = -LENGTH;
			}
			else if (g_aPlayer[nCntPlayer].pos.z > LENGTH)	// 奥の壁にぶつかったとき
			{
				g_aPlayer[nCntPlayer].pos.z = LENGTH;
			}
			if (g_aPlayer[nCntPlayer].pos.y < 0.0f)	// 最低高度に到達したとき
			{
				g_aPlayer[nCntPlayer].pos.y = 0.0f;
				g_aPlayer[nCntPlayer].bJump = false;
			}
			else if (g_aPlayer[nCntPlayer].pos.y > 100.0f)	// 最高高度に到達したとき
			{
				g_aPlayer[nCntPlayer].pos.y = 100.0f;
			}

			SetPositionShadow(g_aPlayer[nCntPlayer].nIdxShadow, g_aPlayer[nCntPlayer].pos);

			// 移動量の更新
			g_aPlayer[nCntPlayer].move.x += (0.0f - g_aPlayer[nCntPlayer].move.x) * 0.1f;
			g_aPlayer[nCntPlayer].move.y += (0.0f - g_aPlayer[nCntPlayer].move.y) * 0.1f;
			g_aPlayer[nCntPlayer].move.z += (0.0f - g_aPlayer[nCntPlayer].move.z) * 0.1f;
		}
#if 0
		// Enterキーでリセット
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			g_aPlayer[nCntPlayer].pos = POS;
			g_aPlayer[nCntPlayer].rot = DEFALT;
			g_aPlayer[nCntPlayer].move = DEFALT;
		}
#endif
	}
}

//========================================================================
// プレイヤーの描画処理
//========================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
		D3DMATERIAL9 matDef;	// 現在のマテリアルを保存
		D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatPlayer[nCntPlayer]->GetBufferPointer();

			// マテリアルごとにプレイヤーを描画
			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer[nCntPlayer]; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTexturePlayer[nCntPlayer][nCntMat]);

				// プレイヤーパーツの描画
				g_pMeshPlayer[nCntPlayer]->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//========================================================================
// プレイヤーの情報を渡す
//========================================================================
Player* GetPlayer(void)
{
	return &g_aPlayer[0];
}

//========================================================================
// プレイヤーを設置する
//========================================================================
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_aPlayer[nIdx].bUse = true;
	g_aPlayer[nIdx].pos = pos;
	g_aPlayer[nIdx].rot = rot;
	g_aPlayer[nIdx].nIdxShadow = SetShadow(SHADOWTYPE_CIRCLE, SHADOｗ, SHADOｗ);
}
