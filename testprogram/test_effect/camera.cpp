//========================================================================
// 
// 3Dポリゴンの描画[camera.cpp]
//
//========================================================================
#include "camera.h"
#include "input.h"

// マクロ定義
#define CAMERA_CLOSEST	(10.0f)		// カメラとの距離がこれ以上近い場合、描画を行わない
#define CAMERA_FARTHEST	(1000.0f)	// カメラとの距離がこれ以上遠い場合、描画を行わない
#define MOVE			(1.5f)		// 移動量
#define ROTATE			(0.005f)	// 回転量
#define DISTANCE		(250.0f)	// 視点と注視点の距離
#define CAMERA_POS		(D3DXVECTOR3(0.0f, 150.0f, -DISTANCE))	// カメラの位置

// グローバル変数
Camera g_camera;	// カメラの情報

//========================================================================
// カメラの初期化処理
//========================================================================
void InitCamera(void)
{
	g_camera.posV = CAMERA_POS;
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================================================
// カメラの終了処理
//========================================================================
void UninitCamera(void)
{

}

//========================================================================
// カメラの更新処理
//========================================================================
void UpdateCamera(void)
{
	// 平行移動
	if (GetKeyboardPress(DIK_A) == true)	// 右に移動
	{
		g_camera.posV.x -= cosf(g_camera.rot.y) * MOVE;
		g_camera.posV.z += sinf(g_camera.rot.y) * MOVE;

		g_camera.posR.x -= cosf(g_camera.rot.y) * MOVE;
		g_camera.posR.z += sinf(g_camera.rot.y) * MOVE;
	}
	else if (GetKeyboardPress(DIK_D) == true)	// 左に移動
	{
		g_camera.posV.x += cosf(g_camera.rot.y) * MOVE;
		g_camera.posV.z -= sinf(g_camera.rot.y) * MOVE;

		g_camera.posR.x += cosf(g_camera.rot.y) * MOVE;
		g_camera.posR.z -= sinf(g_camera.rot.y) * MOVE;
	}
	if (GetKeyboardPress(DIK_W) == true)	// 奥に移動
	{
		g_camera.posV.x += sinf(g_camera.rot.y) * MOVE;
		g_camera.posV.z += cosf(g_camera.rot.y) * MOVE;

		g_camera.posR.x += sinf(g_camera.rot.y) * MOVE;
		g_camera.posR.z += cosf(g_camera.rot.y) * MOVE;
	}
	else if (GetKeyboardPress(DIK_S) == true)	// 手前に移動
	{
		g_camera.posV.x -= sinf(g_camera.rot.y) * MOVE;
		g_camera.posV.z -= cosf(g_camera.rot.y) * MOVE;

		g_camera.posR.x -= sinf(g_camera.rot.y) * MOVE;
		g_camera.posR.z -= cosf(g_camera.rot.y) * MOVE;
	}

	// 注視点を中心としたカメラ移動
	if (GetKeyboardPress(DIK_Z) == true)	// 時計回り
	{
		g_camera.rot.y += ROTATE;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2;
		}
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * DISTANCE;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * DISTANCE;
	}
	else if (GetKeyboardPress(DIK_C) == true)	// 反時計回り
	{
		g_camera.rot.y -= ROTATE;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2;
		}
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * DISTANCE;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * DISTANCE;
	}

	// 視点の起点としたカメラ移動(首振り)
	if (GetKeyboardPress(DIK_E) == true)
	{
		g_camera.rot.y += ROTATE;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2;
		}
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * DISTANCE;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * DISTANCE;
	}
	else if (GetKeyboardPress(DIK_Q) == true)
	{
		g_camera.rot.y -= ROTATE;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2;
		}
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * DISTANCE;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * DISTANCE;
	}

	// spaceキーでリセット
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		g_camera.posV = CAMERA_POS;
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//========================================================================
// カメラの設定
//========================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(FOV_DEFAULT),					// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		CAMERA_CLOSEST,								// カメラから描画できる距離(カメラに近い場合)
		CAMERA_FARTHEST);							// カメラから描画できる距離(カメラから離れている場合)

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,		// 視点
						&g_camera.posR,		// 注視点
						&g_camera.vecU);	// 上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//========================================================================
// カメラ情報を渡す
//========================================================================
Camera* GetCamera(void)
{
	return &g_camera;
}