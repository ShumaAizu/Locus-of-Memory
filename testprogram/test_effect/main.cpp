//========================================================================
// 
// 3Dポリゴンの描画[main.cpp]
// 
//========================================================================
#include "main.h"
#include "input.h"
#include "field.h"
#include "wall.h"
#include "player.h"
#include "shadow.h"
#include "camera.h"
#include "light.h"
//#include "sound.h"
//#include "title.h"
//#include "tutorial.h"
//#include "game.h"
//#include "ranking.h"
//#include "result.h"
//#include "fade.h"
//#include "time.h"

//// デバッグ用インクルード
//#include "player.h"
//#include "block.h"
//#include "item.h"

// マクロ定義
#define CLASS_NAME	"WindowClass"	// ウィンドウクラスの名前
#define WINDOW_NAME	"ポリゴンの描画"	// ウィンドウの名前(キャプションに表示)

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

// グローバル変数
LPDIRECT3D9	 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS = RESET_DATA;			// 最後にFPSを計測した時間
MODE g_mode = MODE_TITLE;	// 現在のモード
bool g_bDebug = true;

// デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//========================================================================
// メイン関数
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // 画面サイズの構造体

	// ↓ ウィンドウクラスの構造体
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 0にする(通常は使用しない)
		0,									// 0にする(通常は使用しない)
		hInstance,							// インスタントハンドル
		LoadIcon(NULL, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// クライアント領域の背景色を白にする
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION),	// ファイルのアイコン
	};

	HWND hWnd;	// ウィンドウハンドル(識別子)
	MSG msg;	// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時間

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに指定
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);	

	// ウィンドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウの左上X座標
		CW_USEDEFAULT,				// ウィンドウの左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウのID
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))// false でフルスクリーン
	{ // 初期化処理に失敗した場合
		return -1;
	}

	// 分解能の設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;	// 初期化
	dwExecLastTime = timeGetTime();	// 現在時刻を取得して保存
	dwFrameCount = RESET_DATA;		// 初期化
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);			// クライアント領域を設定

	// メッセージループ
	// メッセージキューからメッセージを取得→メッセージを取得しなかった場合0を返す
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // Windowsの処理
			if (msg.message == WM_QUIT)
			{ // WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{ // DirectXの処理
			dwCurrentTime = timeGetTime(); // 現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// FPSを測定した時刻を保存
				dwFrameCount = RESET_DATA;		// フレームレートのクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻(現在時刻)を保存
				// 更新処理
				Update();
				// 描画処理
				Draw();
				dwFrameCount ++;
			}
		}
	}
	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}

//========================================================================
// ウィンドウプロシージャ
//========================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY: // ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: // キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE: // ESCが押された場合
			nID = MessageBox(hWnd, "QUIT THE GAME?", "ポリゴンの描画", MB_YESNO);
			if (nID == IDYES)
			{
				// ウィンドウを破棄する→WM_DESTROYメッセージを送る
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE: // ×ボタンを押された場合
		nID = MessageBox(hWnd, "QUIT THE GAME?", "ポリゴンの描画",  MB_YESNO);
		if (nID == IDYES)
		{
			// ウィンドウを破棄する→WM_DESTROYメッセージを送る
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を返す
}

//========================================================================
// 初期化処理
//========================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;	// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// DirectX3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファのとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	// アルファブレンドの設定1
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // アルファブレンドの設定2
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// アルファブレンドの設定3

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャの繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャの繰り返し設定
	
	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// テクスチャのアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// テクスチャのアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// テクスチャのアルファブレンドの設定

	// デバッグ表示用のフォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

#if 0
	// サウンドの初期化処理
	InitSound(hWnd);

	// フェードの初期化
	InitFade(MODE_TITLE);

	// 乱数の種を設定
	srand((unsigned int)time(0));
#endif

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 影の初期化処理
	InitShadow();

	// フィールドの初期化処理
	InitField();

	// 壁の初期化処理
	InitWall();

	// プレイヤーの初期化処理
	InitPlayer();

	return S_OK;
}

//========================================================================
// 終了処理
//========================================================================
void Uninit(void)
{
#if 0
	// タイトル画面の終了処理
	UninitTitle();

	// チュートリアル画面の終了処理
	UninitTutorial();

	// ゲーム画面の終了処理
	UninitGame();

	// リザルト画面の終了処理
	UninitResult();

	// リザルト画面の終了処理
	UninitRanking();

	// BGMの停止
	StopSound();

	// サウンドの終了処理
	UninitSound();
#endif

	// プレイヤーの終了処理
	UninitPlayer();

	// 壁の終了処理
	UninitWall();

	// フィールドの終了処理
	UninitField();

	// 影の終了処理
	UninitShadow();

	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// キーボードの終了処理
	UninitKeyboard();

	// ジョイパッドの終了処理
	UninitJoypad();

	// デバッグ用のフォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// // フェードの終了処理
	// UninitFade();

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//========================================================================
// 更新処理
//========================================================================
void Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();
	// ジョイパッドの更新処理
	UpdateJoypad();

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 影の更新処理
	UpdateShadow();

	// フィールドの更新処理
	UpdateField();

	// 壁の更新処理
	UpdateWall();

	// プレイヤーの更新処理
	UpdatePlayer();

#if 0
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_GAME:		// ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT:	// リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING:	// ランキング画面
		UpdateRanking();
		break;
	}

	// フェードの更新処理
	UpdateFade();
#endif
}

//========================================================================
// 描画処理
//========================================================================
void Draw(void)
{
	// 画面のクリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{ // 描画が成功した場合
		
#if 0
		switch (g_mode)
		{
		case MODE_TITLE:	// タイトル画面
			DrawTitle();
			break;

		case MODE_TUTORIAL:	// チュートリアル画面
			DrawTutorial();
			break;

		case MODE_GAME:		// ゲーム画面
			DrawGame();
			break;

		case MODE_RESULT:	// リザルト画面
			DrawResult();
			break;

		case MODE_RANKING:	// ランキング画面
			DrawRanking();
			break;
		}
		// フェードの描画処理
		DrawFade();
#endif
		// カメラの設定
		SetCamera();

		// フィールドの描画処理
		DrawField();

		// 壁の描画処理
		DrawWall();

		// 影の描画処理
		DrawShadow();

		// モデルの描画処理
		DrawPlayer();

		// デバッグ表示
// #ifdef _DEBUG
		DrawDebug();
// #endif 

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//========================================================================
// デバッグ表示
//========================================================================
void DrawDebug(void)
{
	char aStr[1024];	// 画面に表示する文字列
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Camera* pCamera = GetCamera();
	Player* pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		g_bDebug = g_bDebug ^ XOR_ONE;	// 表示非表示の切り替え
	}
	if (g_bDebug == true)
	{

		sprintf(&aStr[RESET_DATA], 
			"====================\n各種数値\n====================\n視点の座標 (%.2f, %.2f, %.2f)\n注視点の座標 (%.2f, %.2f, %.2f)\n向き (%.2f)\nモデルの位置 (%.2f, %.2f, %.2f)\nモデルの移動量 (%.2f, %.2f, %.2f)\nモデルの向き (%.2f)\n\n====================\n操作方法[視点]\n====================\n視点の平行移動\n前後移動  W/S\n左右移動  A/D\n\n視点のみの移動\n左右旋回  Z/C\n\n注視点の移動\n左右旋回  Q/E\n\nカメラ位置のリセット  SPACE\n\n\n====================\n操作方法[Xモデル]\n====================\n前後移動  ↑/↓\n\n左右移動  ←/→\n\nモデル位置のリセット  ENTER", 
			pCamera->posV.x, pCamera->posV.y, pCamera->posV.z, pCamera->posR.x, pCamera->posR.y, pCamera->posR.z, pCamera->rot.y, pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z, pPlayer->move.x, pPlayer->move.y, pPlayer->move.z, pPlayer->rot.y);

		// テキストの描画
		g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

}

//========================================================================
// モードの設定
//========================================================================
void SetMode(MODE mode)
{
#if 0
	// 現在のモードの終了処理
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
		UninitTutorial();
		break;

	case MODE_GAME:		// ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:	// リザルト画面
		UninitResult();
		break;

	case MODE_RANKING:	// ランキング画面
		UninitRanking();
		break;
	}

	// 新しいモードの初期化処理
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:		// ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:	// リザルト画面
		InitResult();
		break;

	case MODE_RANKING:	// リザルト画面
		InitRanking();
		break;
	}
	g_mode = mode;	// 現在のモードを保存
#endif
}

//========================================================================
// モードの取得
//========================================================================
MODE GetMode(void)
{
	return g_mode;
}