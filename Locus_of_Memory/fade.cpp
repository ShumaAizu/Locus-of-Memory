//=============================================================================
//
//	Locus of Memory [fade.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"main.h"
#include"fade.h"
#include"game.h"

// グローバル変数
LPDIRECT3DTEXTURE9    g_pTextureFade = NULL;        // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;    // 頂点バッファへのポインタ
FADE g_fade;
MODE g_modeNext;
D3DXCOLOR g_colorFade;

//=======================================================
// プレイヤーの初期化処理
//=======================================================
void InitFade(MODE modeNext)
{
    LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ
    VERTEX_2D* pVtx;                // 頂点情報へのポインタ

    // デバイスの取得
    pDevice = GetDevice();

    g_fade = FADE_IN;
    g_modeNext = g_modeNext;
    g_colorFade = D3DXCOLOR(1.0f,1.0f,1.0f,0.0f);

    // 頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffFade,
        NULL);

    // 頂点バッファをロックし、頂点情報へのポインタ取得
    g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
    pVtx[0].col = g_colorFade;
    pVtx[1].col = g_colorFade;
    pVtx[2].col = g_colorFade;
    pVtx[3].col = g_colorFade;

    // テクスチャ座標の指定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    SetMode(g_modeNext);

    // 頂点バッファをアンロック
    g_pVtxBuffFade->Unlock();
}
//=======================================================
// プレイヤーの終了処理
//=======================================================
void UninitFade(void)
{
    // テクスチャの破棄
    if (g_pTextureFade != NULL)
    {
        g_pTextureFade->Release();
        g_pTextureFade = NULL;
    }

    // 頂点バッファの破棄
    if (g_pVtxBuffFade != NULL)
    {
        g_pVtxBuffFade->Release();
        g_pVtxBuffFade = NULL;
    }
}

//=======================================================
// プレイヤーの更新処理
//=======================================================
void UpdateFade(void)
{   
    VERTEX_2D* pVtx;                // 頂点情報へのポインタ
    

    if (g_fade != FADE_NONE)
    {
        if (g_fade == FADE_IN)
        {
            g_colorFade.a -= 0.03;

            if (g_colorFade.a <= 0.0f)
            {
                g_colorFade.a = 0.0f;
                g_fade = FADE_NONE;
            }
        }
        else if (g_fade == FADE_OUT)
        {
            // ポーズを無効にする
            SetEnablePause(false);

            g_colorFade.a += 0.03;

            if (g_colorFade.a >= 1.0f)
            {
                g_colorFade.a = 1.0f;
                g_fade = FADE_IN;

                SetMode(g_modeNext);
            }
        }

        // 頂点バッファをロックし、頂点情報へのポインタ取得
        g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

        // 頂点カラーの設定
        pVtx[0].col = g_colorFade;
        pVtx[1].col = g_colorFade;
        pVtx[2].col = g_colorFade;
        pVtx[3].col = g_colorFade;


        // 頂点バッファをアンロック
        g_pVtxBuffFade->Unlock();
    }
}

//=======================================================
// プレイヤーの描画処理
//=======================================================
void DrawFade(void)
{
    LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ

    // デバイスの取得
    pDevice = GetDevice();

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    // テクスチャの設定
    pDevice->SetTexture(0, NULL);

    // ポリゴンの描写
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
void SetFade(MODE modeNext)
{
    g_fade = FADE_OUT;
    g_modeNext = modeNext;
    g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

}
FADE *GetFade(void)
{
    return &g_fade;
}
