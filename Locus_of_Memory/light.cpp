//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[light.cpp]
//
//======================================================================================
#include "light.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

// マクロ定義
#define NUM_LIGHT	(3)		// ライトの最大数
#define DIRECTOPN0	(D3DXVECTOR3(0.2f, -0.8f, 0.4f))		// ライトの方向
#define DIRECTOPN1	(D3DXVECTOR3(-0.2f, 0.9f, -0.4f))		// ライトの方向
#define DIRECTOPN2	(D3DXVECTOR3(0.9f, 0.1f, 0.4f))			// ライトの方向

// グローバル変数
D3DLIGHT9 g_aLight[NUM_LIGHT];	// ライトの情報

D3DXVECTOR3 g_LightDis;

//======================================================================================
// ライトの初期化処理
//======================================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

	// ライトの情報をクリア
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9) * NUM_LIGHT);

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		// ライトの種類を設定
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:	// 0番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = LIGHT_BRIGHTEST;
			// ライトの方向を設定
			vecDir = DIRECTOPN0;
			break;

		case 1:	// 1番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = LIGHT_BRIGHTER;
			// ライトの方向を設定
			vecDir = DIRECTOPN1;
			break;

		case 2:	// 2番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = LIGHT_BRIGHT;
			// ライトの方向を設定
			vecDir = DIRECTOPN2;
			break;
		}
	
		D3DXVec3Normalize(&vecDir, &vecDir);	// ベクトルを正規化
		g_aLight[nCntLight].Direction = vecDir;
		// ライトを設定
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);
		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//======================================================================================
// ライトの終了処理
//======================================================================================
void UninitLight(void)
{

}

//======================================================================================
// ライトの更新処理
//======================================================================================
void UpdateLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

	// ライトの情報をクリア
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9) * NUM_LIGHT);



	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		// ライトの種類を設定
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{

		case 0:	// 0番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = LIGHT_BRIGHTEST;
			if (GetKeyboardPress(DIK_UP) == true)
			{
				// ライトの方向を設定
				g_LightDis.x += 0.005f;

			}
			if (GetKeyboardPress(DIK_DOWN) == true)
			{
				// ライトの方向を設定
				g_LightDis.x -= 0.005f;

			}
			if (GetKeyboardPress(DIK_RIGHT) == true)
			{
				// ライトの方向を設定
				g_LightDis.y += 0.005f;

			}
			if (GetKeyboardPress(DIK_LEFT) == true)
			{
				// ライトの方向を設定
				g_LightDis.y -= 0.005f;

			}
			if (GetKeyboardPress(DIK_U) == true)
			{
				// ライトの方向を設定
				g_LightDis.z += 0.005f;

			}
			if (GetKeyboardPress(DIK_M) == true)
			{
				// ライトの方向を設定
				g_LightDis.z -= 0.005f;

			}

			vecDir = g_LightDis;
			break;

		case 1:	// 1番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = LIGHT_BRIGHTER;
			// ライトの方向を設定
			vecDir = DIRECTOPN1;
			break;

		case 2:	// 2番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = LIGHT_BRIGHT;
			// ライトの方向を設定
			vecDir = DIRECTOPN2;
			break;

		}

		D3DXVec3Normalize(&vecDir, &vecDir);	// ベクトルを正規化
		g_aLight[nCntLight].Direction = vecDir;
		// ライトを設定
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);
		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);

	}

	PrintDebugProc("ライト[%d]の位置 : (%.3f, %.3f, %.3f)\n", 0, vecDir.x, vecDir.y, vecDir.z);

}



//======================================================================================
// ライトの色を変える
//======================================================================================
void SetLightColors(D3DXCOLOR Brightest, D3DXCOLOR Brighter, D3DXCOLOR Blight)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

	// ライトの情報をクリア
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9) * NUM_LIGHT);

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		// ライトの種類を設定
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:	// 0番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = Brightest;
			// ライトの方向を設定
			vecDir = DIRECTOPN0;
			break;

		case 1:	// 1番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = Brighter;
			// ライトの方向を設定
			vecDir = DIRECTOPN1;
			break;

		case 2:	// 2番目のライト
			// ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = Blight;
			// ライトの方向を設定
			vecDir = DIRECTOPN2;
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);	// ベクトルを正規化
		g_aLight[nCntLight].Direction = vecDir;
		// ライトを設定
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);
		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}
