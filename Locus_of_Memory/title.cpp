//=============================================================================
//
//	Locus of Memory [title.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"main.h"
#include"title.h"
#include"input.h"
#include"fade.h"
//#include"sound.h"
#include"camera.h"
#include"light.h"
//#include"meshfield.h"

#define MAX_TITLE   (2)

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};    // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;    // 頂点バッファへのポインタ
TITLE g_aTitle;
START g_aStart;
int g_nCounterAnimTitle;
int g_nPatternAnimTitle;
int g_nCntStart;
int g_nCntStartGame;

//=======================================================
// タイトルの初期化処理
//=======================================================
void InitTitle(void)
{

    LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ
    VERTEX_2D* pVtx;                // 頂点情報へのポインタ

    // デバイスの取得
    pDevice = GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice,
        "data\\TEXTURE\\title000.png",
        &g_pTextureTitle[0]);

    D3DXCreateTextureFromFile(pDevice,
        "data\\TEXTURE\\title001.png",
        &g_pTextureTitle[1]);

    // 音源の読み込み
    //PlaySound(SOUND_LABEL_BGM000);

    // タイトルロゴの初期化
    g_aTitle.pos = D3DXVECTOR3(650.0f, -15.0f, 0.0f);
    g_aTitle.move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    g_aTitle.state = TITLESTATE_MOVETITLE;

    // スタートボタンの初期化
    g_aStart.pos = D3DXVECTOR3(650.0f,545.0f, 0.0f);
    g_aStart.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
    g_aStart.state = STARTSTATE_NORMAL;

    // 頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffTitle,
        NULL);

    // 頂点バッファをロックし、頂点情報へのポインタ取得
    g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

    // ロゴの初期化
    // 頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(g_aTitle.pos.x - 390, g_aTitle.pos.y - 205, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(g_aTitle.pos.x + 390, g_aTitle.pos.y - 205, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(g_aTitle.pos.x - 390, g_aTitle.pos.y + 205, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(g_aTitle.pos.x + 390, g_aTitle.pos.y + 205, 0.0f);

     // rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // 頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    // テクスチャ座標の指定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    pVtx += 4;

    // ゲームスタートの初期化
    // 頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(g_aStart.pos.x - 250, g_aStart.pos.y - 145, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(g_aStart.pos.x + 250, g_aStart.pos.y - 145, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(g_aStart.pos.x - 250, g_aStart.pos.y + 145, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(g_aStart.pos.x + 250, g_aStart.pos.y + 145, 0.0f);

    // rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // 頂点カラーの設定
    pVtx[0].col = g_aStart.col;
    pVtx[1].col = g_aStart.col;
    pVtx[2].col = g_aStart.col;
    pVtx[3].col = g_aStart.col;

    // テクスチャ座標の指定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    // 頂点バッファをアンロック
    g_pVtxBuffTitle->Unlock();
    
    InitLight();

    InitCamera();

    //InitMeshfield();
}
//=======================================================
// タイトルの終了処理
//=======================================================
void UninitTitle(void)
{
    for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
    {
        // テクスチャの破棄
        if (g_pTextureTitle[nCntTitle] != NULL)
        {
            g_pTextureTitle[nCntTitle]->Release();
            g_pTextureTitle[nCntTitle] = NULL;
        }
    }

    // 頂点バッファの破棄
    if (g_pVtxBuffTitle != NULL)
    {
        g_pVtxBuffTitle->Release();
        g_pVtxBuffTitle = NULL;
    }

    UninitLight();

    UninitCamera();

    //UninitMeshfield();
}
//=======================================================
// タイトルの更新処理
//=======================================================
void UpdateTitle(void)
{
    FADE* pFade = GetFade();

    //LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ
    VERTEX_2D* pVtx;                    // 頂点情報へのポインタ


    // 頂点バッファをロックし、頂点情報へのポインタ取得
    g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

    switch (g_aTitle.state)
    {

    case TITLESTATE_MOVETITLE:  // タイトルが出てる状態
        g_aTitle.pos += g_aTitle.move;

        pVtx[0].pos = D3DXVECTOR3(g_aTitle.pos.x - 390, g_aTitle.pos.y - 205, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(g_aTitle.pos.x + 390, g_aTitle.pos.y - 205, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(g_aTitle.pos.x - 390, g_aTitle.pos.y + 205, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(g_aTitle.pos.x + 390, g_aTitle.pos.y + 205, 0.0f);    
        if (g_aTitle.pos.y == 215.0f)
        {
            g_aTitle.state = TITLESTATE_STOPTITLE;

            pVtx += 4;

            g_aStart.col.a = 1.0f;

            pVtx[0].col = g_aStart.col;
            pVtx[1].col = g_aStart.col;
            pVtx[2].col = g_aStart.col;
            pVtx[3].col = g_aStart.col;

            pVtx -= 4;
        }

        if (GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE || GetKeyboardTrigger(JOYKEY_A) == true && *pFade == FADE_NONE)
        {

            g_aTitle.pos.y = 215.0f;

            pVtx[0].pos = D3DXVECTOR3(g_aTitle.pos.x - 390, g_aTitle.pos.y - 205, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(g_aTitle.pos.x + 390, g_aTitle.pos.y - 205, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(g_aTitle.pos.x - 390, g_aTitle.pos.y + 205, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(g_aTitle.pos.x + 390, g_aTitle.pos.y + 205, 0.0f);

            g_aTitle.state = TITLESTATE_STOPTITLE;

            pVtx += 4;

            g_aStart.col.a = 1.0f;

            pVtx[0].col = g_aStart.col;
            pVtx[1].col = g_aStart.col;
            pVtx[2].col = g_aStart.col;
            pVtx[3].col = g_aStart.col;

            pVtx -= 4;
        }

        break;

    case TITLESTATE_STOPTITLE:		// タイトルが止まってる状態

        switch (g_aStart.state)
        {
            case STARTSTATE_NORMAL:

                g_nCntStart++;
                if (g_nCntStart >= 30)
                {
                    g_nCntStartGame++;
                    if (g_nCntStartGame % 2 == 1)
                    {
                        pVtx += 4;

                        g_aStart.col.a = 0.5f;

                        pVtx[0].col = g_aStart.col;
                        pVtx[1].col = g_aStart.col;
                        pVtx[2].col = g_aStart.col;
                        pVtx[3].col = g_aStart.col;

                        pVtx -= 4;
                    }
                    else
                    {
                        pVtx += 4;

                        g_aStart.col.a = 1.0f;

                        pVtx[0].col = g_aStart.col;
                        pVtx[1].col = g_aStart.col;
                        pVtx[2].col = g_aStart.col;
                        pVtx[3].col = g_aStart.col;

                        pVtx -= 4;
                    }

                    g_nCntStart = 0;
                }
                

                if (GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE || GetKeyboardTrigger(JOYKEY_A) == true && *pFade == FADE_NONE)
                {

                    SetJoypadVibration(30000, 30000, 60,1);

                    g_nCntStart = 0;
                    g_nCntStartGame = 0;

                    //PlaySound(SOUND_LABEL_SE_START);

                    g_aStart.state = STARTSTATE_MOVE;

                }
                break;

            case STARTSTATE_MOVE:

                g_nCntStart++;
                if (g_nCntStart >= 4)
                {
                    g_nCntStartGame++;
                    if (g_nCntStartGame % 2 == 1)
                    {
                        pVtx += 4;

                        g_aStart.col.a = 0.5f;

                        pVtx[0].col = g_aStart.col;
                        pVtx[1].col = g_aStart.col;
                        pVtx[2].col = g_aStart.col;
                        pVtx[3].col = g_aStart.col;

                        pVtx -= 4;
                    }
                    else
                    {
                        pVtx += 4;

                        g_aStart.col.a = 1.0f;

                        pVtx[0].col = g_aStart.col;
                        pVtx[1].col = g_aStart.col;
                        pVtx[2].col = g_aStart.col;
                        pVtx[3].col = g_aStart.col;

                        pVtx -= 4;
                    }

                    if (g_nCntStartGame <= 12)
                    {
                        g_nCntStart = 0;
                    }
                    else
                    {
                        g_aStart.state = STARTSTATE_NORMAL;

                        //StopSound();
                        
                        SetFade(MODE_TUTORIAL);
                    }

                }
                break;

        }

        break;

    }

    // 頂点バッファをアンロック
    g_pVtxBuffTitle->Unlock();

    UpdateLight();

    UpdateCamera();

    //UpdateMeshfield();
}
//=======================================================
// タイトルの描画処理
//=======================================================
void DrawTitle(void)
{

    //DrawMeshfield();

    LPDIRECT3DDEVICE9 pDevice;        // デバイスへのポインタ

    // デバイスの取得
    pDevice = GetDevice();

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
    {
        // テクスチャの設定
        pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

        // ポリゴンの描写
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
    }
}