//=============================================================================
//
//	Locus of Memory [main.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MAIN_H_			// このマクロ定義がされていなかったら
#define _MAIN_H_			// 2重インクルード防止のマクロを定義する

//*****************************************************************************
// モードの種類
//*****************************************************************************
typedef enum
{
	MODE_NONE = -1,			// なにもない
	MODE_TITLE,				// タイトル画面
	MODE_TUTORIAL,			// チュートリアル画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_RANKING,			// ランキング画面
	MODE_PAUSE,				// ポーズ画面
	MODE_MAX				// 総数
}MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
float AngleNormalize(float fAngle);
bool CrossCollision(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, bool bRetrun, bool bIn);
void ToggleFullscreen(HWND hWnd);

#endif