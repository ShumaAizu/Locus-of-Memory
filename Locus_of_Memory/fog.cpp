//=============================================================================
//
//	霧の処理 [fog.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "fog.h"

//=============================================================================
//	霧の有効無効切り替え
//=============================================================================
void SetFogEnable(bool isFog)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	if (isFog == true)
	{// オンにする
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}
	else if (isFog == false)
	{// オフにする
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
}

//=============================================================================
//	霧の設定処理 [色 / 開始位置 / 終了位置 ]
//=============================================================================
void SetFog(D3DXCOLOR col, float fStart, float fEnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 霧の設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, col);
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	pDevice->SetRenderState(D3DRS_FOGSTART, *(LPDWORD)(&fStart));
	pDevice->SetRenderState(D3DRS_FOGEND, *(LPDWORD)(&fEnd));
}