//=============================================================================
//
//	カメラ処理 [camera.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "camera.h"
#include "input.h"
#include "title.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Camera g_acamera[MAX_CAMERA];	// カメラの情報
int g_nNumCamera = 0;			// カメラの数

// カメラの情報格納用
CameraInfo g_acameraInfo[] =
{
	{INIT_1PCAMERAPOSV, INIT_1PCAMERAPOSR, INIT_VECU, INIT_1PCAMERAROT, INIT_1PVEIWPORT },
	{INIT_2PCAMERAPOSV, INIT_2PCAMERAPOSR, INIT_VECU, INIT_2PCAMERAROT, INIT_2PVEIWPORT },
	{INIT_1PCAMERAPOSV, INIT_1PCAMERAPOSR, INIT_VECU, INIT_1PCAMERAROT, INIT_3PVEIWPORT },
	{INIT_1PCAMERAPOSV, INIT_1PCAMERAPOSR, INIT_VECU, INIT_1PCAMERAROT, INIT_4PVEIWPORT },
};

//=============================================================================
//	カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	Camera* pCamera = &g_acamera[0];
	CameraInfo* pCameraInfo = &g_acameraInfo[0];

	memset(pCamera, NULL, sizeof(Camera) * MAX_CAMERA);

	// 初期化
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCamera++, pCameraInfo++)
	{// カメラに予め用意した初期値を代入
		// カメラの位置設定
		pCamera->posV = pCameraInfo->posV;
		pCamera->posR = pCameraInfo->posR;
		pCamera->vecU = pCameraInfo->vecU;
		pCamera->rot = pCameraInfo->rot;
		pCamera->fRadiusVertical = INIT_RADIUS;

		pCamera->viewport = pCameraInfo->viewport;		// ビューポート設定

		// 目的位置
		pCamera->posVDest = pCameraInfo->posV;
		pCamera->posRDest = pCameraInfo->posR;

		// 距離を離す
		pCamera->posVDest.z = pCamera->posRDest.z + cosf(pCamera->rot.y) * CAMERAPOSR_DIS;
		pCamera->posVDest.x = pCamera->posRDest.x + sinf(pCamera->rot.y) * CAMERAPOSR_DIS;
	}

	g_nNumCamera = INIT_NUMCAMERA;
}

//=============================================================================
//	カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
//	カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	Camera* pCamera = &g_acamera[0];	// 先頭アドレス
	Player* pPlayer = GetPlayer();		// プレイヤーの先頭アドレス

	for (int nCntCamera = 0; nCntCamera < g_nNumCamera; nCntCamera++, pCamera++, pPlayer++)
	{
		if (GetKeyboardPress(DIK_Z) == true)
		{
			pCamera->rot.y += 0.05f;
		}

		if (GetKeyboardPress(DIK_C) == true)
		{
			pCamera->rot.y += -0.05f;
		}

		PrintDebugProc("[%d] 視点 = { %.2f %.2f %.2f }\n", nCntCamera, pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
		PrintDebugProc("[%d] 注視点 = { %.2f %.2f %.2f }\n", nCntCamera, pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

		if (GetKeyboardPress(DIK_Z) == true || GetJoypadStickPressR(JOYSTICK_RIGHT, nCntCamera) == true)
		{
			pCamera->rot.y += 0.05f;
		}

		if (GetKeyboardPress(DIK_C) == true || GetJoypadStickPressR(JOYSTICK_LEFT, nCntCamera) == true)
		{
			pCamera->rot.y += -0.05f;
		}

		if (GetKeyboardPress(DIK_E) == true || GetJoypadStickPressR(JOYSTICK_UP, nCntCamera) == true)
		{
			if (pCamera->rot.x < D3DX_PI * 0.25f)
			{
				pCamera->rot.x += 0.05f;
			}
		}

		if (GetKeyboardPress(DIK_Q) == true || GetJoypadStickPressR(JOYSTICK_DOWN, nCntCamera) == true)
		{
			if (pCamera->rot.x > -D3DX_PI * 0.25f)
			{
				pCamera->rot.x += -0.05f;
			}
		}

		// 角度を補正
		pCamera->rot.y = AngleNormalize(pCamera->rot.y);

		D3DXVECTOR3 diff;

		diff = D3DXVECTOR3(sinf(0.0f) * pCamera->fRadiusVertical, cosf(pCamera->rot.x) * pCamera->fRadiusVertical, cosf(0.0f) * pCamera->fRadiusVertical);

		// 球の半径を計算
		pCamera->fRadiusHorizonttal = SQRTF(diff.x, diff.y);

		// 前方表示
		pCamera->posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * AHEADVEIW;
		pCamera->posRDest.y = pPlayer->pos.y + AHEADVEIW;
		pCamera->posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * AHEADVEIW;

		// 目的視点を目的注視点から離す
		pCamera->posVDest.x = pCamera->posRDest.x + sinf(pCamera->rot.y) * pCamera->fRadiusHorizonttal;
		pCamera->posVDest.y = pCamera->posRDest.y + sinf(pCamera->rot.x) * pCamera->fRadiusVertical;
		pCamera->posVDest.z = pCamera->posRDest.z + cosf(pCamera->rot.y) * pCamera->fRadiusHorizonttal;

		// 各移動処理
		if (GetKeyboardPress(DIK_Q) == true)
		{
			pCamera->posRDest.y += 1.0f;
		}

		if (GetKeyboardPress(DIK_B) == true)
		{
			pCamera->posRDest.y += -1.0f;
		}

		if (GetKeyboardPress(DIK_E) == true)
		{
			pCamera->posVDest.y += 1.0f;
		}

		if (GetKeyboardPress(DIK_N) == true)
		{
			pCamera->posVDest.y += -1.0f;
		}

#if 0
		if (GetKeyboardPress(DIK_W) == true)
		{
			pCamera->posRDest.x += sinf(pCamera->rot.y) * -5.0f;
			pCamera->posRDest.z += cosf(pCamera->rot.y) * -5.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{
			pCamera->posRDest.x += sinf(pCamera->rot.y + (D3DX_PI * -0.5f)) * -5.0f;
			pCamera->posRDest.z += cosf(pCamera->rot.y + (D3DX_PI * -0.5f)) * -5.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{
			pCamera->posRDest.x += sinf(pCamera->rot.y + (D3DX_PI)) * -5.0f;
			pCamera->posRDest.z += cosf(pCamera->rot.y + (D3DX_PI)) * -5.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{
			pCamera->posRDest.x += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * -5.0f;
			pCamera->posRDest.z += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * -5.0f;
		}
#endif
		
		// 注視点を目的注視点に移動
		pCamera->posR.x += (pCamera->posRDest.x - pCamera->posR.x) * CAMERA_INERTIA;
		pCamera->posR.y += (pCamera->posRDest.y - pCamera->posR.y) * CAMERA_INERTIA;
		pCamera->posR.z += (pCamera->posRDest.z - pCamera->posR.z) * CAMERA_INERTIA;

		// 視点を目的視点に移動
		pCamera->posV.x += (pCamera->posVDest.x - pCamera->posV.x) * CAMERA_INERTIA;
		pCamera->posV.y += (pCamera->posVDest.y - pCamera->posV.y) * CAMERA_INERTIA;
		pCamera->posV.z += (pCamera->posVDest.z - pCamera->posV.z) * CAMERA_INERTIA;
	}
}

//=============================================================================
//	カメラの設定処理 [カメラの番号]
//=============================================================================
void SetCamera(int nIdx)
{
	Camera* pCamera = &g_acamera[nIdx];		// アドレス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&pCamera->viewport);

	// プロジェクションマトリックスを初期化
	D3DXMatrixIdentity(&pCamera->mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&pCamera->mtxProjection,
							D3DXToRadian(45.0f),
							(float)pCamera->viewport.Width / (float)pCamera->viewport.Height,
							10.0f,
							10000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &pCamera->mtxProjection);

	// ビューマトリックスを初期化
	D3DXMatrixIdentity(&pCamera->mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&pCamera->mtxView,
					   &pCamera->posV,
					   &pCamera->posR,
					   &pCamera->vecU);
						
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &pCamera->mtxView);
}

//=============================================================================
//	カメラの取得処理
//=============================================================================
Camera *GetCamera(void)
{
	return &g_acamera[0];
}

//=============================================================================
//	カメラの数設定処理
//=============================================================================
void SetNumCamera(MODE mode)
{
	Camera* pCamera = &g_acamera[0];
	CameraInfo* pCameraInfo = &g_acameraInfo[0];

	g_nNumCamera = INIT_NUMCAMERA;
	pCamera->viewport = DEFAULT_VEIWPORT;

	if (mode == MODE_GAME || mode == MODE_TUTORIAL)
	{
		OPERATIONTYPE operationtype = GetOperationType();		// 今の操作タイプ
		if(operationtype == OPERATIONTYPE_2P)
		{// 2Pプレイだったら
			g_nNumCamera = CAMERA_2PPLAY;
			for (int nCntCamera = 0; nCntCamera < g_nNumCamera; nCntCamera++, pCamera++, pCameraInfo++)
			{// カメラに予め用意した初期値を代入
				// カメラの位置設定
				pCamera->posV = pCameraInfo->posV;
				pCamera->posR = pCameraInfo->posR;
				pCamera->vecU = pCameraInfo->vecU;
				pCamera->rot = pCameraInfo->rot;

				pCamera->viewport = pCameraInfo->viewport;		// ビューポート設定

				// 目的位置
				pCamera->posVDest = pCameraInfo->posV;
				pCamera->posRDest = pCameraInfo->posR;

				// 距離を離す
				pCamera->posVDest.z = pCamera->posRDest.z + cosf(pCamera->rot.y) * CAMERAPOSR_DIS;
				pCamera->posVDest.x = pCamera->posRDest.x + sinf(pCamera->rot.y) * CAMERAPOSR_DIS;
			}
		}
	}
}

//=============================================================================
//	カメラの数取得処理
//=============================================================================
int GetNumCamera(void)
{
	return g_nNumCamera;
}

//=============================================================================
//	ビューポート生成処理
//=============================================================================
D3DVIEWPORT9 SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	D3DVIEWPORT9 ViwePort = {};		// 返り値用変数

	ViwePort.X = X;					// 左上X座標
	ViwePort.Y = Y;					// 左上Y座標
	ViwePort.Width = Width;			// 幅
	ViwePort.Height = Height;		// 高さ
	ViwePort.MinZ = 0.0f;			// 固定
	ViwePort.MaxZ = 1.0f;			// 固定

	return ViwePort;
}