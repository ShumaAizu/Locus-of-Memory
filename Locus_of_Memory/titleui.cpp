//======================================================================================
// 
// タイトルの2DUI処理[titleui.h]
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "titleui.h"
#include "title.h"
#include "input.h"

// タイトルUIの種類
typedef enum
{
	TITLEUITYPE_LOGO = 0,	// タイトルロゴ
	TITLEUITYPE_1PPLAY,		// 1PPALY
	TITLEUITYPE_2PPLAY,		// 2PPALY
	TITLEUITYPE_KEYBOARD,	// KEYBOARD
	TITLEUITYPE_MAX
}TITLEUITYPE;

// タイトルUI演出の管理
typedef enum
{
	TITLETEXT_MODE_NONE = 0,	// 通常
	TITLETEXT_MODE_WIDTH,		// 横長
	TITLETEXT_MODE_HEIGHT,		// 縦長
	TITLETEXT_MODE_BLINKING,	// 点滅
	TITLETEXT_MODE_BLINKING1,	// 高速点滅
	TITLETEXT_MODE_MAX
}TITLETEXT_MODE;

// タイトルUIの状態
typedef enum
{
	TITLETEXTSTATE_APPEAR = 0,	// ロゴ出現状態
	TITLETEXTSTATE_NORMAL,		// 通常状態
	TITLETEXTSTATE_START,		// チュートリアルへの遷移待機
	TITLETEXTSTATE_MAX
}TITLETEXTSTATE;

// タイトルUIの構造体
typedef struct
{
	TITLEUITYPE		type;	// 種類
	TITLETEXT_MODE	mode;	// 演出
	D3DXVECTOR3		pos;	// 位置
	int nCounterState;		// 拡縮を管理
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	bool	bDisp;			// 表示状態
}TitleText;

// マクロ定義
#define NUM_TITLEUI			(TITLEUITYPE_MAX)	// タイトルUIのレイヤー数
#define SELECT_NUM			(3)			// 操作方法選択数
#define LOGO_POS			(D3DXVECTOR3(350.0f, 200.0f, 0.0f))	// ロゴの位置
#define LOGO_WIDTH			(300.0f)	// ロゴの横幅
#define LOGO_HEIGHT			(150.0f)	// ロゴの縦幅
#define SELECT_POS			(D3DXVECTOR3(1000.0f, 400.0f, 0.0f))	// セレクトボタンの開始位置
#define SELECT_WIDTH		(150.0f)	// セレクトボタンの横幅
#define SELECT_HEIGHT		(75.0f)		// セレクトボタンの縦幅
#define SELECT_MARGIN		(80.0f)		// セレクトボタンの余白

// テクスチャの読み込み
const char* c_apFilenameTitleUI[TITLEUITYPE_MAX] =
{
	"data\\TEXTURE\\title_000.png",	// [TITLETEXT_TYPE_BG]
	"data\\TEXTURE\\title_001.png",	// [TITLETEXT_TYPE_ENTER]
	"data\\TEXTURE\\title_001.png",	// [TITLETEXT_TYPE_ENTER]
	"data\\TEXTURE\\title_001.png",	// [TITLETEXT_TYPE_ENTER]
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTitleUI[NUM_TITLEUI] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleUI = NULL; // 頂点バッファへのポインタ
TitleText g_aTitleUI[NUM_TITLEUI];	// 構造体
int g_nSelectOperation;	// 選択されている操作方法

//========================================================================
// タイトルUIの初期化処理
//========================================================================
void InitTitleUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < NUM_TITLEUI; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameTitleUI[nCntUI], &g_apTextureTitleUI[nCntUI]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLEUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitleUI, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLEUI; nCntTitle++, pVtx += 4)
	{
		switch (nCntTitle)
		{
		case TITLEUITYPE_LOGO:	// タイトルロゴ
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(LOGO_POS.x - LOGO_WIDTH, LOGO_POS.y - LOGO_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(LOGO_POS.x + LOGO_WIDTH, LOGO_POS.y - LOGO_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(LOGO_POS.x - LOGO_WIDTH, LOGO_POS.y + LOGO_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(LOGO_POS.x + LOGO_WIDTH, LOGO_POS.y + LOGO_HEIGHT, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case TITLEUITYPE_1PPLAY:	// 1PPLAY
		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case TITLEUITYPE_2PPLAY:	// 2PPLAY
		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			break;

		case TITLEUITYPE_KEYBOARD:	// KEYBOARD
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntTitle - 1) * SELECT_MARGIN, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			break;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_aTitleUI[nCntTitle].bDisp = true;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffTitleUI->Unlock();

	g_nSelectOperation = TITLEUITYPE_1PPLAY;	// 1PPALYが選択されている
}

//========================================================================
// タイトルUIの終了処理
//========================================================================
void UninitTitleUI(void)
{
	// テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < NUM_TITLEUI; nCntTitle++)
	{
		if (g_apTextureTitleUI[nCntTitle] != NULL)
		{
			g_apTextureTitleUI[nCntTitle]->Release();
			g_apTextureTitleUI[nCntTitle] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleUI != NULL)
	{
		g_pVtxBuffTitleUI->Release();
		g_pVtxBuffTitleUI = NULL;
	}
}

//========================================================================
// タイトルUIの更新処理
//========================================================================
void UpdateTitleUI(void)
{
	// 現在のフェードの状態を管理
	FADE *pfade = GetFade();

	// 選択状態の変更(Repeat)
	if (GetKeyboardRepeat(DIK_W) == true || GetJoypadRepeat(JOYKEY_UP, 0) == true)
	{
		g_nSelectOperation--;
		if (g_nSelectOperation < TITLEUITYPE_1PPLAY)
		{
			g_nSelectOperation = TITLEUITYPE_KEYBOARD;
		}
	}
	else if (GetKeyboardRepeat(DIK_S) == true || GetJoypadRepeat(JOYKEY_DOWN, 0) == true)
	{
		g_nSelectOperation++;
		if (g_nSelectOperation > TITLEUITYPE_KEYBOARD)
		{
			g_nSelectOperation = TITLEUITYPE_1PPLAY;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, 0) == true || GetJoypadTrigger(JOYKEY_START, 0) == true)
	{
		switch (g_nSelectOperation)
		{
		case TITLEUITYPE_1PPLAY:	// 1PPLAY
			SetOperationType(OPERATIONTYPE_1P);
			break;

		case TITLEUITYPE_2PPLAY:	// 2PPLAY
			SetOperationType(OPERATIONTYPE_2P);
			break;

		case TITLEUITYPE_KEYBOARD:	// KEYBOARD
			SetOperationType(OPERATIONTYPE_KEYBOARD);
			break;
		}
	}

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4;	// ロゴの分だけポインタを進める

	for (int nCntUI = 0; nCntUI < SELECT_NUM; nCntUI++, pVtx += 4)
	{
		if (nCntUI + 1 == g_nSelectOperation)	// 選択部だけ明るく表示
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
	}
	
	// 頂点バッファをアンロック
	g_pVtxBuffTitleUI->Unlock();
}

//========================================================================
// タイトルUIの描画処理
//========================================================================
void DrawTitleUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLEUI; nCntTitle++)
	{
		if (g_aTitleUI[nCntTitle].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitleUI[nCntTitle]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
		}
	}
}