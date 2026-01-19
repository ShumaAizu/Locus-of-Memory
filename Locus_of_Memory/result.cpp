//=============================================================================
//
//	Locus of Memory [result.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"main.h"
#include"result.h"
#include"input.h"
#include"fade.h"
//#include"sound.h"



// グローバル変数
LPDIRECT3DTEXTURE9    g_pTextureResult = NULL;        // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;    // 頂点バッファへのポインタ
D3DXVECTOR3 g_posResult;
D3DXVECTOR3 g_moveResult;
int g_nCounterAnimResult;
int g_nPatternAnimResult;

//=======================================================
// リザルトの初期化処理
//=======================================================
void InitResult(void)
{
    LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ
    VERTEX_2D* pVtx;                // 頂点情報へのポインタ

    // デバイスの取得
    pDevice = GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice,
        "data\\TEXTURE\\result000.png",
        &g_pTextureResult);

    //PlaySound(SOUND_LABEL_BGM004);

    g_posResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // 頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffResult,
        NULL);

    // 頂点バッファをロックし、頂点情報へのポインタ取得
    g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(SCREEN_ZERO, SCREEN_ZERO, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_ZERO, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(SCREEN_ZERO, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    // rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // 頂点カラーの設定
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    // テクスチャ座標の指定
    //pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimResult % 8 * 0.5, g_nPatternAnimResult % 8 * 0.5);
    //pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimResult % 8 * 0.5 + 0.5, g_nPatternAnimResult % 8 * 0.5);
    //pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimResult % 8 * 0.5, g_nPatternAnimResult % 8 * 0.5 + 0.5);
    //pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimResult % 8 * 0.5 + 0.5, g_nPatternAnimResult % 8 * 0.5 + 0.5);

    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    // 頂点バッファをアンロック
    g_pVtxBuffResult->Unlock();
}
//=======================================================
// リザルトの終了処理
//=======================================================
void UninitResult(void)
{
    // テクスチャの破棄
    if (g_pTextureResult != NULL)
    {
        g_pTextureResult->Release();
        g_pTextureResult = NULL;
    }

    // 頂点バッファの破棄
    if (g_pVtxBuffResult != NULL)
    {
        g_pVtxBuffResult->Release();
        g_pVtxBuffResult = NULL;
    }
}
//=======================================================
// リザルトの更新処理
//=======================================================
void UpdateResult(void)
{
    FADE* pFade = GetFade();

    if (GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE || GetKeyboardTrigger(JOYKEY_A) == true && *pFade == FADE_NONE)
    {
        //StopSound();

        SetFade(MODE_TITLE);
    }
}
//=======================================================
// リザルトの描画処理
//=======================================================
void DrawResult(void)
{
    LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ

    // デバイスの取得
    pDevice = GetDevice();

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    // テクスチャの設定
    pDevice->SetTexture(0, g_pTextureResult);

    // ポリゴンの描写
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}