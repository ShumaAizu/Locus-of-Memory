//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[player.cpp]
//
//======================================================================================
#include "main.h"
#include "player.h"
#include "debugproc.h"
#include "model.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define MOVE			(g_player.fMove)	// 移動量
#define MOVE_NORMAL		(1.5f)				// 通常時の移動量
#define STICK_MOVE		(0.04688f)			// ステックの入力値を用いた際の移動量[通常]
#define JUMP			(17.0f)				// ジャンプ
#define ROTATE			(0.05f)				// 回転量
#define MOVE_NEUTRAL	(10)				// 移動と待機のモーションブレンド
#define INERTIA			(g_player.fInertia)	// 慣性
#define INERTIA_NORMAL	(0.9f)				// 通常時の慣性
#define INERTIA_JUMP	(0.1f)				// 慣性(ジャンプ)
#define CORRECTION_ROT	(0.1f)				// 回転の減衰係数
#define NEUTRAL_MOTION	(0.15f)				// 待機モーションへの遷移
#define RIGHT			(D3DX_PI / 2)		// 右を向く
#define LEFT			(-(D3DX_PI / 2))	// 左を向く
#define BACK			(D3DX_PI)			// 後ろを向く
#define FRONT			(0.0f)				// 正面を向く
#define RIGHT_BACK		(D3DX_PI / 4 + D3DX_PI / 2)		// 右後ろ
#define RIGHT_FRONT		(D3DX_PI / 4)					// 右手前
#define LEFT_BACK		(-(D3DX_PI / 4 + D3DX_PI / 2))	// 左後ろ
#define LEFT_FRONT		(-(D3DX_PI / 4))	// 左手前
#define GRAVITY			(0.75f)				// 重力
#define POS				(D3DXVECTOR3(0.0f, 50.0f, 0.0f))				// プレイヤーの位置
#define DEFAULT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// xyzが0.0fの場合
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))					// 基本の法線
#define VTX_MIN			(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// プレイヤーの大きさの初期化値(最小)
#define VTX_MAX			(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// プレイヤーの大きさの初期化値(最大)

// グローバル変数
Player g_player;		// プレイヤーの情報を格納

// モデルの読み込み
const char* c_apFilenameModel[MAX_MODEL] =
{
	"data\\MODEL\\player\\00_body.x",	// 胴体[0]
	"data\\MODEL\\player\\01_Larm.x",	// 左腕[1]
	"data\\MODEL\\player\\02_Rarm.x",	// 右腕[2]
	"data\\MODEL\\player\\03_Lfoot.x",	// 左足[3]
	"data\\MODEL\\player\\04_Rfoot.x",	// 右足[4]
};

//======================================================================================
// プレイヤーの初期化処理
//======================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;
	int nNumVtx = RESET_DATA;	// 頂点数
	DWORD dwSizeFVF;			// 頂点フォーマットのサイズ

	// モーションの初期化処理
	InitMotion();

	// 初期化
	g_player.pos					= POS;				// 位置
	g_player.posOld					= DEFAULT;			// 前回の位置
	g_player.move					= DEFAULT;			// 移動量
	g_player.rot					= DEFAULT;			// 向き
	g_player.rotDest				= DEFAULT;			// 目的の向き
	g_player.nIdxShadow				= -1;				// 影のインデックス(NULL)
	g_player.state					= PLAYERSTATE_NONE;	// プレイヤーの状態を管理
	g_player.nCounterState			= RESET_DATA;		// 状態管理カウンターの初期化
	g_player.bJump					= false;			// ジャンプ状態の管理
	g_player.bMove					= false;			// 移動状態を管理
	g_player.bMoveOld				= false;			// 過去の移動状態を管理
	g_player.bRedKey				= false;			// RedKeyの取得状態を管理[trueで取得済み]
	g_player.bGreenKey				= false;			// GreenKeyの取得状態を管理[trueで取得済み]
	g_player.bCyanKey				= false;			// CyanKeyの取得状態を管理[trueで取得済み]
	g_player.bDisp					= true;				// 表示状態
	g_player.fMove					= MOVE_NORMAL;		// 通常時の移動量
	g_player.fInertia				= INERTIA_NORMAL;	// 通常時の慣性
	g_player.nNumMotion				= MOTIONTYPE_MAX;										// モーションの総数を設定
	g_player.motionType				= MOTIONTYPE_NEUTRAL;									// 現在のモーション(待機)
	g_player.bLoopMotion			= g_player.aMotionInfo[g_player.motionType].bLoop;		// モーションのループを管理
	g_player.nNumKey				= g_player.aMotionInfo[g_player.motionType].nNumKey;	// キーの総数
	g_player.nKey					= RESET_DATA;											// 現在実行されているキー
	g_player.nCounterMotion			= RESET_DATA;											// キーモーションカウンター
	g_player.nNumModel				= MAX_MODEL;
	g_player.bFinishMotion			= false;	// モーションが終了していない
	g_player.bBlendMotion			= false;	// ブレンドモーションを行わない
	g_player.motionTypeBlend		= MOTIONTYPE_NEUTRAL;	// ブレンドモーションの種類
	g_player.bLoopMotionBlend		= g_player.aMotionInfo[g_player.motionTypeBlend].bLoop;		// ブレンドモーションのループを管理
	g_player.nNumKeyBlend			= g_player.aMotionInfo[g_player.motionTypeBlend].nNumKey;	// ブレンドモーションのキー総数
	g_player.nKeyBlend				= RESET_DATA;	// ブレンドモーションの実行されているキー
	g_player.nCounterMotionBlend	= RESET_DATA;	// ブレンドモーションキーカウンター
	g_player.nFrameBlend			= RESET_DATA;	// 何フレームかけてブレンドモーションを行うか
	g_player.nCounterBlend			= RESET_DATA;	// ブレンドカウンター

	for (int nCntModel = RESET_DATA; nCntModel < MAX_MODEL; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(c_apFilenameModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[nCntModel].pBuffMat, NULL, &g_player.aModel[nCntModel].dwNumMat, &g_player.aModel[nCntModel].pMesh);
	
		// 頂点数を取得
		nNumVtx = g_player.aModel[nCntModel].pMesh->GetNumVertices();
		// 頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_player.aModel[nCntModel].pMesh->GetFVF());
	
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = RESET_DATA; nCntMat < (int)g_player.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_player.aModel[nCntModel].apTexture[nCntMat]);
			}
		}
	
	}

	// プレイヤーのオフセット情報を設定
	SetPlayerOffset();

	// 影のIDを設定
	g_player.nIdxShadow = SetShadow(SHADOWTYPE_SQUARE, SHADOW, SHADOW);
}

//======================================================================================
// プレイヤーの終了処理
//======================================================================================
void UninitPlayer(void)
{
	for (int nCntModel = RESET_DATA; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// メッシュの破棄
		if (g_player.aModel[nCntModel].pMesh != NULL)
		{
			g_player.aModel[nCntModel].pMesh->Release();
			g_player.aModel[nCntModel].pMesh = NULL;
		}

		// テクスチャの破棄
		for (int nCntMat = RESET_DATA; nCntMat < (int)g_player.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_player.aModel[nCntModel].apTexture[nCntMat] != NULL)
			{
				g_player.aModel[nCntModel].apTexture[nCntMat]->Release();
				g_player.aModel[nCntModel].apTexture[nCntMat] = NULL;
			}
		}

		// マテリアルの破棄
		if (g_player.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.aModel[nCntModel].pBuffMat->Release();
			g_player.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//======================================================================================
// プレイヤーの更新処理
//======================================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();
	Goal* pGoal = GetGoal();
	// スティックの角度を取得
	XINPUT_STATE xInputState = GetJoypadState();
	float fRotDiffKey = 0.0f;	// キー入力時の角度補正計算用
	float fRotDiffPad = 0.0f;	// ジョイパッド入力時の角度補正計算用
	float fAngle;	// 角度
	int nValueH;	// 水平方向の入力を保存
	int nValueV;	// 垂直方向の入力を保存
	float fMove;	// 移動量を指定

	if (g_player.bDisp == true)
	{
		// 過去の情報を保存
		g_player.posOld = g_player.pos;
		g_player.bMoveOld = g_player.bMove;

		// 移動/ジャンプの処理
		if (g_player.motionType != MOTIONTYPE_LANDING && g_player.motionType != MOTIONTYPE_ACTION && g_player.state == PLAYERSTATE_NONE)
		{
			// キーボード/ジョイパッド[十字キー]移動操作
			if ((GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_S) == true) || // WASD移動
				(GetJoypadPress(JOYKEY_LEFT) == true || GetJoypadPress(JOYKEY_RIGHT) == true || GetJoypadPress(JOYKEY_UP) == true || GetJoypadPress(JOYKEY_DOWN) == true))	// 十字キー移動
			{
				// 移動情報をリセット
				g_player.bMove = true;
				if (pGoal->state == GOALSTATE_END)
				{
					SetEffect(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 20.0f, g_player.pos.z), DEFAULT, COLOR_ORANGE, EFFECTTYPE_EXPLOSION, 0.0f, 20, 10.0f);
				}

				// 移動を管理
				if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)	// 右に移動
				{
					if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)	// 奥に移動
					{
						g_player.move.x += sinf(pCamera->rot.y - D3DX_PI / 4) * MOVE;
						g_player.move.z += cosf(pCamera->rot.y - D3DX_PI / 4) * MOVE;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)	// 手前に移動
					{
						g_player.move.x -= sinf(pCamera->rot.y + D3DX_PI / 4) * MOVE;
						g_player.move.z -= cosf(pCamera->rot.y + D3DX_PI / 4) * MOVE;
					}
					else
					{
						g_player.move.x -= cosf(pCamera->rot.y) * MOVE;
						g_player.move.z += sinf(pCamera->rot.y) * MOVE;
					}
				}
				else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)	// 左に移動
				{
					if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)	// 奥に移動
					{
						g_player.move.x += cosf(pCamera->rot.y - D3DX_PI / 4) * MOVE;
						g_player.move.z -= sinf(pCamera->rot.y - D3DX_PI / 4) * MOVE;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)	// 手前に移動
					{
						g_player.move.x += cosf(pCamera->rot.y + D3DX_PI / 4) * MOVE;
						g_player.move.z -= sinf(pCamera->rot.y + D3DX_PI / 4) * MOVE;
					}
					else
					{
						g_player.move.x += cosf(pCamera->rot.y) * MOVE;
						g_player.move.z -= sinf(pCamera->rot.y) * MOVE;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)	// 奥に移動
				{
					g_player.move.x += sinf(pCamera->rot.y) * MOVE;
					g_player.move.z += cosf(pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)	// 手前に移動
				{
					g_player.move.x -= sinf(pCamera->rot.y) * MOVE;
					g_player.move.z -= cosf(pCamera->rot.y) * MOVE;
				}

				// プレイヤーの方向を設定
				if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)	// 右に移動
				{
					if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)	// 奥に移動
					{
						g_player.rotDest.y = pCamera->rot.y + D3DX_PI / 2 + D3DX_PI / 4;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)	// 手前に移動
					{
						g_player.rotDest.y = pCamera->rot.y + D3DX_PI / 2 - D3DX_PI / 4;
					}
					else
					{
						g_player.rotDest.y = pCamera->rot.y + D3DX_PI / 2;
					}
				}
				else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)	// 左に移動
				{
					if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)	// 奥に移動
					{
						g_player.rotDest.y = pCamera->rot.y - D3DX_PI / 2 - D3DX_PI / 4;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)	// 手前に移動
					{
						g_player.rotDest.y = pCamera->rot.y - D3DX_PI / 2 + D3DX_PI / 4;
					}
					else
					{
						g_player.rotDest.y = pCamera->rot.y - D3DX_PI / 2;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)	// 奥に移動
				{
					g_player.rotDest.y = pCamera->rot.y + D3DX_PI;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)	// 手前に移動
				{
					g_player.rotDest.y = pCamera->rot.y;
				}
			}

			// ジョイパッド[アナログスティック]移動操作[倒した数値によって移動量変化]
			else if (GetJoypadLeftStickValue(&nValueH, &nValueV) == true)
			{
				// 移動情報をリセット
				g_player.bMove = true;

				// ステックの値を小さくする[オーバーフロー対策]
				nValueH = nValueH / 1000;
				nValueV = nValueV / 1000;

				fMove = sqrtf((float)(nValueH * nValueH + nValueV * nValueV)) * STICK_MOVE;

				fAngle = atan2f(-(float)(nValueH), (float)(nValueV));

				g_player.move.x += sinf(pCamera->rot.y - fAngle) * fMove;
				g_player.move.z += cosf(pCamera->rot.y - fAngle) * fMove;

				// プレイヤーの方向を設定
				g_player.rotDest.y = atan2f(-(float)(sinf(pCamera->rot.y - fAngle)), -(float)cosf(pCamera->rot.y - fAngle));
			}

			// ジャンプ処理
			if ((GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_player.bJump == false)
			{
				PlaySound(SE_JUMP);
				g_player.move.y = JUMP;
				g_player.bJump = true;
				SetMotion(MOTIONTYPE_JUMP, true, 30);
			}

			if (g_player.move.x <= NEUTRAL_MOTION && g_player.move.x >= -NEUTRAL_MOTION &&
				g_player.move.z <= NEUTRAL_MOTION && g_player.move.z >= -NEUTRAL_MOTION)
			{ // 一定以上の移動量を下回ると移動フラグが折れる
				g_player.bMove = false;
			}

			if (g_player.motionType == MOTIONTYPE_JUMP || g_player.motionType == MOTIONTYPE_LANDING || g_player.motionType == MOTIONTYPE_ACTION)
			{

			}
			else
			{
				// モーション切り替え
				if (g_player.bMoveOld == false && g_player.bMove == true && g_player.bJump == false)	// 前回移動しておらず、今回は移動している場合
				{
					SetMotion(MOTIONTYPE_MOVE, true, MOVE_NEUTRAL);
				}
				else if (g_player.bMoveOld == true && g_player.bMove == false && g_player.bJump == false)	// 前回移動しており、今回は移動していない場合
				{
					SetMotion(MOTIONTYPE_NEUTRAL, true, MOVE_NEUTRAL);
				}
			}
		}

		// モーション切り替え(手動)
#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F1) == true)
		{
			SetMotion(MOTIONTYPE_NEUTRAL, true, MOVE_NEUTRAL);
		}

		if (GetKeyboardTrigger(DIK_F2) == true)
		{
			SetMotion(MOTIONTYPE_MOVE, true, MOVE_NEUTRAL);
		}

		if (GetKeyboardTrigger(DIK_F3) == true)
		{
			SetMotion(MOTIONTYPE_ACTION, true, MOVE_NEUTRAL);
		}

		if (GetKeyboardTrigger(DIK_F4) == true)
		{
			SetMotion(MOTIONTYPE_JUMP, true, MOVE_NEUTRAL);
		}

		if (GetKeyboardTrigger(DIK_F5) == true)
		{
			SetMotion(MOTIONTYPE_LANDING, true, MOVE_NEUTRAL);
		}
#endif

		// プレイヤーの方向を補正
		fRotDiffKey = g_player.rotDest.y - g_player.rot.y;	// 差分を計算
		if (fRotDiffKey > D3DX_PI)
		{
			fRotDiffKey -= D3DX_PI * 2;
		}
		if (fRotDiffKey < -D3DX_PI)
		{
			fRotDiffKey += D3DX_PI * 2;
		}
		g_player.rot.y += (fRotDiffKey)*CORRECTION_ROT;
		if (g_player.rot.y > D3DX_PI)
		{
			g_player.rot.y -= D3DX_PI * 2;
		}
		if (g_player.rot.y < -D3DX_PI)
		{
			g_player.rot.y += D3DX_PI * 2;
		}

		// 重力
		g_player.move.y -= GRAVITY;

		// 位置の更新
		g_player.pos += g_player.move;

		BlockModel* pBlockModel = GetBlockModel();
		MeshField* pMeshField = GetMeshField();
		MODE mode = GetMode();
		float fSize = 430.0f;

		// プレイヤーの行動範囲を制限
		if (pGoal->state != GOALSTATE_END || mode == MODE_TUTORIAL)
		{
			if (g_player.pos.x < (pBlockModel[BLOCKTYPE_BOOK000].vtxMin.x + COLLISION))	// 左の壁にぶつかったとき
			{
				g_player.pos.x = pBlockModel[BLOCKTYPE_BOOK000].vtxMin.x + COLLISION;
			}
			else if (g_player.pos.x > (pBlockModel[BLOCKTYPE_BOOK000].vtxMax.x - COLLISION))	// 右の壁にぶつかったとき
			{
				g_player.pos.x = pBlockModel[BLOCKTYPE_BOOK000].vtxMax.x - COLLISION;
			}
			if (g_player.pos.z < pBlockModel[BLOCKTYPE_BOOK000].vtxMin.z + COLLISION)	// 手前の壁にぶつかったとき
			{
				g_player.pos.z = pBlockModel[BLOCKTYPE_BOOK000].vtxMin.z + COLLISION;
			}
			else if (g_player.pos.z > pBlockModel[BLOCKTYPE_BOOK000].vtxMax.z - COLLISION)	// 奥の壁にぶつかったとき
			{
				g_player.pos.z = pBlockModel[BLOCKTYPE_BOOK000].vtxMax.z - COLLISION;
			}
			if (g_player.pos.y > 205.0f)	// 頭をぶつけた時
			{
				g_player.pos.y = 205.0f;
				g_player.move.y = RESET_DATA;
			}
		}
		else if (pGoal->state == GOALSTATE_END)
		{
			if (g_player.pos.x > (fSize + COLLISION))	// 左の壁にぶつかったとき
			{
				g_player.pos.x = fSize + COLLISION;
			}
			else if (g_player.pos.x < (-fSize - COLLISION))	// 右の壁にぶつかったとき
			{
				g_player.pos.x = -fSize - COLLISION;
			}
			if (g_player.pos.z < -fSize - COLLISION)	// 手前の壁にぶつかったとき
			{
				g_player.pos.z = -fSize - COLLISION;
			}
			else if (g_player.pos.z > fSize + COLLISION)	// 奥の壁にぶつかったとき
			{
				g_player.pos.z = fSize + COLLISION;
			}
		}

#if 0
		D3DXVECTOR3 vecLine, vecToPos, vecMove = DEFAULT;
		float fRate, fWhole, fRatio = RESET_DATA;

		// 壁より外側に出ないように制限
		// 座標01の境界線との判定
		{
			vecLine = D3DXVECTOR3((pMeshField->pos.x + FIELD_WIDTH) - (pMeshField->pos.x - FIELD_WIDTH), pMeshField->pos.y, (pMeshField->pos.z + FIELD_DEPTH) - (pMeshField->pos.z + FIELD_DEPTH));
			vecToPos = D3DXVECTOR3(g_player.pos.x - (pMeshField->pos.x + FIELD_WIDTH), pMeshField->pos.y, g_player.pos.z - (pMeshField->pos.z + FIELD_DEPTH));
			vecMove = D3DXVECTOR3(g_player.pos.x - g_player.posOld.x, g_player.pos.y, g_player.pos.z - g_player.posOld.z);
			// 交点を求める[交点までの面積 / 全体の面積 = 比率]
			fWhole = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);		// 全体の面積
			fRatio = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);	// 比率の大きさ
			fRate = fRatio / fWhole;										// 比率の計算
			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < RESET_DATA)
			{
				g_player.pos.z = vecLine.z * fRate + FIELD_DEPTH;
				g_player.move.z = 0.0f;
			}
			if (g_player.pos.z > vecLine.z * fRate + FIELD_DEPTH)
			{
				g_player.pos.z = vecLine.z * fRate + FIELD_DEPTH;
			}
		}

		// 座標12の境界線との判定
		{
			vecLine = D3DXVECTOR3((pMeshField->pos.x + FIELD_WIDTH) - (pMeshField->pos.x + FIELD_WIDTH), pMeshField->pos.y, (pMeshField->pos.z - FIELD_DEPTH) - (pMeshField->pos.z + FIELD_DEPTH));
			vecToPos = D3DXVECTOR3(g_player.pos.x - (pMeshField->pos.x + FIELD_WIDTH), pMeshField->pos.y, g_player.pos.z - (pMeshField->pos.z + FIELD_DEPTH));
			vecMove = D3DXVECTOR3(g_player.pos.x - g_player.posOld.x, g_player.pos.y, g_player.pos.z - g_player.posOld.z);
			// 交点を求める[交点までの面積 / 全体の面積 = 比率]
			fWhole = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);	// 全体の面積
			fRatio = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);	// 比率の大きさ
			fRate = fRatio / fWhole;
			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < RESET_DATA)
			{
				g_player.pos.x = vecLine.x * fRate + FIELD_DEPTH;
				g_player.move.x = 0.0f;
			}
			if (g_player.pos.x > vecLine.x * fRate + FIELD_DEPTH)
			{
				g_player.pos.x = vecLine.x * fRate + FIELD_DEPTH;
			}
		}

		// 座標23の境界線との判定
		{
			vecLine = D3DXVECTOR3((pMeshField->pos.x - FIELD_WIDTH) - (pMeshField->pos.x + FIELD_WIDTH), pMeshField->pos.y, (pMeshField->pos.z - FIELD_DEPTH) - (pMeshField->pos.z - FIELD_DEPTH));
			vecToPos = D3DXVECTOR3(g_player.pos.x - (pMeshField->pos.x - FIELD_WIDTH), pMeshField->pos.y, g_player.pos.z - (pMeshField->pos.z - FIELD_DEPTH));
			vecMove = D3DXVECTOR3(g_player.pos.x - g_player.posOld.x, g_player.pos.y, g_player.pos.z - g_player.posOld.z);
			// 交点を求める[交点までの面積 / 全体の面積 = 比率]
			fWhole = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);	// 全体の面積
			fRatio = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);	// 比率の大きさ
			fRate = fRatio / fWhole;
			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < RESET_DATA)
			{
				g_player.pos.z = vecLine.z * fRate - FIELD_DEPTH;
				g_player.move.z = 0.0f;
			}
			if (g_player.pos.z < vecLine.z * fRate - FIELD_DEPTH)
			{
				g_player.pos.z = vecLine.z * fRate - FIELD_DEPTH;
			}
		}

		// 座標30の境界線との判定
		{
			vecLine = D3DXVECTOR3((pMeshField->pos.x - FIELD_WIDTH) - (pMeshField->pos.x - FIELD_WIDTH), pMeshField->pos.y, (pMeshField->pos.z + FIELD_SIZE / 2) - (pMeshField->pos.z - FIELD_SIZE / 2));
			vecToPos = D3DXVECTOR3(g_player.pos.x - (pMeshField->pos.x - FIELD_WIDTH), pMeshField->pos.y, g_player.pos.z - (pMeshField->pos.z - FIELD_SIZE / 2));
			vecMove = D3DXVECTOR3(g_player.pos.x - g_player.posOld.x, g_player.pos.y, g_player.pos.z - g_player.posOld.z);
			// 交点を求める[交点までの面積 / 全体の面積 = 比率]
			fWhole = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);	// 全体の面積
			fRatio = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);	// 比率の大きさ
			fRate = fRatio / fWhole;
			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < RESET_DATA)
			{
				g_player.pos.x = vecLine.x * fRate - FIELD_DEPTH;
				g_player.move.x = 0.0f;
			}
			if (g_player.pos.x < vecLine.x * fRate - FIELD_DEPTH)
			{
				g_player.pos.x = vecLine.x * fRate - FIELD_DEPTH;
			}
		}
#endif

		// 最低高度に到達したとき
		if (g_player.pos.y < pMeshField->pos.y)
		{
			g_player.pos.y = pMeshField->pos.y;
			g_player.move.y = RESET_DATA;
			if (g_player.bJump == true)
			{
				PlaySound(SE_LANDING);
				SetMotion(MOTIONTYPE_LANDING, true, MOVE_NEUTRAL);
				SetVibration(LAND_VIBLATION, LAND_VIBLATION, 10);
				//SetMeshRing(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 1.0f, g_player.pos.z), DEFAULT, RINGTYPE_LANDING, 0.0f, 30.0f, 100);
				SetParticle(PARTICLETYPE_LANDING, g_player.pos, COLOR_05fSHADOW, 5);
			}
			g_player.bJump = false;
		}

		static bool bLand = false;

		// 当たり判定
		if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == true ||
			(CollisionPenWall(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == true) ||
			(CollisionDecoration(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == true))
		{
			if (g_player.bJump == true)
			{
				//SetMeshRing(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 1.0f + DEFAULT.y, g_player.pos.z), DEFAULT, RINGTYPE_LANDING, 0.0f, 30.0f, 100);
				SetParticle(PARTICLETYPE_LANDING, g_player.pos, COLOR_05fSHADOW, 5);
				SetMotion(MOTIONTYPE_LANDING, false, MOVE_NEUTRAL);
				SetVibration(LAND_VIBLATION, LAND_VIBLATION, 10);
				g_player.bMoveOld = false;
				PlaySound(SE_LANDING);
			}
			g_player.bJump = false;
			bLand = true;
		}
		else if ((CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == false && bLand == true) ||
			(CollisionPenWall(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == false && bLand == true) ||
			(CollisionDecoration(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == false && bLand == true))
		{
			SetMotion(MOTIONTYPE_JUMP, true, MOVE_NEUTRAL);
			g_player.bJump = true;
			bLand = false;
		}

		// ペンの壁との当たり判定
		if (CollisionPenWall(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == true && bLand == false)
		{

		}

		// 飾りとの当たり判定
		if (CollisionDecoration(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION_W, -0.0f, -COLLISION_W), D3DXVECTOR3(COLLISION_W, COLLISION_H, COLLISION_W)) == true && bLand == false)
		{

		}

		// カギとの当たり判定
		CollisionKey(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION, -0.0f, -COLLISION), D3DXVECTOR3(COLLISION, COLLISION_H, COLLISION));

		// コインとの当たり判定
		CollisionCoin(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(-COLLISION, -0.0f, -COLLISION), D3DXVECTOR3(COLLISION, COLLISION_H, COLLISION));

		// 移動量の更新
		g_player.move.x += (0.0f - g_player.move.x) * INERTIA;
		g_player.move.y += (0.0f - g_player.move.y) * INERTIA_JUMP;
		g_player.move.z += (0.0f - g_player.move.z) * INERTIA;

#ifdef _DEBUG
		//	// Enterキーでリセット
		//	if (GetKeyboardTrigger(DIK_RETURN) == true && g_player.bJump == false && g_player.motionType != MOTIONTYPE_LANDING)
		//	{
		//		g_player.pos = POS;
		//		g_player.rot = DEFAULT;
		//		g_player.move = DEFAULT;
		//		g_player.bJump = true;
		//		g_player.bRedKey = false;
		//		g_player.bGreenKey = false;
		//		g_player.bCyanKey = false;
		//		// プレイヤーのオフセット情報を設定
		//		SetPlayerOffset();
		//
		//		float fKeyHeight = -10.0f;
		//		KeyItem* pkeyItem = GetKeyItem();
		//		for (int nCntKey = RESET_DATA; nCntKey < MAX_KEYITEM; nCntKey++, pkeyItem++)
		//		{
		//			if (pkeyItem->bUse == true)
		//			{
		//				pkeyItem->bUse = false;
		//				ResetShadow(pkeyItem->nIdxShadow);
		//			}
		//		}
		//	}
#endif

	}
	// モーションの更新
	UpdateMotion();

	// オブジェクトとの距離を指定
	float fHeight = DistanceShadow(&g_player.pos, DEFAULT, DEFAULT);

	if (g_player.bDisp == true)
	{
		// 影の色を指定
		SetShadowColorData(g_player.nIdxShadow, g_player.pos.y, fHeight);
		// 影の位置を更新
		SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, fHeight, g_player.pos.z));
	}

	if (g_player.bDisp == true)
	{
		PrintDebugProc("%s : (%f, %f, %f)", "プレイヤーの位置", g_player.pos.x, g_player.pos.y, g_player.pos.z);
		PrintDebugProc("%s : (%f, %f, %f)", "プレイヤーの移動量", g_player.move.x, g_player.move.y, g_player.move.z);
		PrintDebugProc("%s : (%f)", "プレイヤーの向き", g_player.rot.y);

		if (g_player.bJump == true)
		{
			PrintDebugProc("%s : %s", "ジャンプ状態", "true");
		}
		else
		{
			PrintDebugProc("%s : %s", "ジャンプ状態", "false");
		}
		PrintDebugProc("%s : %d", "現在のモーション", g_player.motionType);
	}
}

//======================================================================================
// プレイヤーの描画処理
//======================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルを保存
	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	if (g_player.bDisp == true)
	{
		// ワールドマトリックスの初期化(デフォルトの値にする)
		D3DXMatrixIdentity(&g_player.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntModel = RESET_DATA; nCntModel < g_player.nNumModel; nCntModel++)
		{
			D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX	mtxParent;					// 親のマトリックス

			// パーツのワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

			// パーツの親マトリックスを設定
			if (g_player.aModel[nCntModel].nIdxModelParent != -1)
			{ // 親モデルがある場合
				mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{ // 親モデルがない場合
				mtxParent = g_player.mtxWorld;	// プレイヤーのマトリックスを設定
			}

			// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

			// マテリアルごとにプレイヤーを描画
			for (int nCntMat = RESET_DATA; nCntMat < (int)g_player.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_player.aModel[nCntModel].apTexture[nCntMat]);

				// プレイヤーパーツの描画
				g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

		}
		// 保存していたマテリアルに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//======================================================================================
// プレイヤーの情報を渡す
//======================================================================================
Player* GetPlayer(void)
{
	return &g_player;
}

//======================================================================================
// モーションの初期化処理
//======================================================================================
void InitMotion(void)
{
	// NUTORAL[0]
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0] = g_aKeyNeutral[0];
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1] = g_aKeyNeutral[1];
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].bLoop = true;		// ループあり
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey = 2;	// キーの総数

	// MOVE[1]
	g_player.aMotionInfo[MOTIONTYPE_MOVE].aKeyInfo[0] = g_aKeyMove[0];
	g_player.aMotionInfo[MOTIONTYPE_MOVE].aKeyInfo[1] = g_aKeyMove[1];
	g_player.aMotionInfo[MOTIONTYPE_MOVE].aKeyInfo[2] = g_aKeyMove[2];
	g_player.aMotionInfo[MOTIONTYPE_MOVE].aKeyInfo[3] = g_aKeyMove[3];
	g_player.aMotionInfo[MOTIONTYPE_MOVE].bLoop = true;	// ループあり
	g_player.aMotionInfo[MOTIONTYPE_MOVE].nNumKey = 4;	// キーの総数

	// ACTION[2]
	g_player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0] = g_aKeyAction[0];
	g_player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1] = g_aKeyAction[1];
	g_player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2] = g_aKeyAction[2];
	g_player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[3] = g_aKeyAction[3];
	g_player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[4] = g_aKeyAction[4];
	g_player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[5] = g_aKeyAction[5];
	g_player.aMotionInfo[MOTIONTYPE_ACTION].bLoop = false;	// ループなし
	g_player.aMotionInfo[MOTIONTYPE_ACTION].nNumKey = 6;	// キーの総数

	// JUMP[3]
	g_player.aMotionInfo[MOTIONTYPE_JUMP].aKeyInfo[0] = g_aKeyJump[0];
	g_player.aMotionInfo[MOTIONTYPE_JUMP].aKeyInfo[1] = g_aKeyJump[1];
	g_player.aMotionInfo[MOTIONTYPE_JUMP].aKeyInfo[2] = g_aKeyJump[2];
	g_player.aMotionInfo[MOTIONTYPE_JUMP].aKeyInfo[3] = g_aKeyJump[3];
	g_player.aMotionInfo[MOTIONTYPE_JUMP].aKeyInfo[4] = g_aKeyJump[4];
	g_player.aMotionInfo[MOTIONTYPE_JUMP].aKeyInfo[5] = g_aKeyJump[5];
	g_player.aMotionInfo[MOTIONTYPE_JUMP].bLoop = false;	// ループなし
	g_player.aMotionInfo[MOTIONTYPE_JUMP].nNumKey = 6;	// キーの総数

	// LANDING[4]
	g_player.aMotionInfo[MOTIONTYPE_LANDING].aKeyInfo[0] = g_aKeyLanding[0];
	g_player.aMotionInfo[MOTIONTYPE_LANDING].aKeyInfo[1] = g_aKeyLanding[1];
	g_player.aMotionInfo[MOTIONTYPE_LANDING].bLoop = false;		// ループあり
	g_player.aMotionInfo[MOTIONTYPE_LANDING].nNumKey = 2;	// キーの総数
}

//======================================================================================
// モーションの更新処理
//======================================================================================
void UpdateMotion(void)
{
	// 全モデル(パーツ)ごとに位置、向きを更新
	// fDiffKeyは現在のキーと次のキーとの差分を取得
	// fRateKeyは現在のキーが何フレーム処理されたかの割合を取得
	for (int nCntModel = RESET_DATA; nCntModel < g_player.nNumModel; nCntModel++)
	{
		KEY Key = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel];												// 現在のキー
		KEY KeyNext = g_player.aMotionInfo[g_player.motionType].aKeyInfo[(g_player.nKey + 1) % g_player.nNumKey].aKey[nCntModel];						// 次のフレームのキー
		KEY KeyBlend = g_player.aMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend].aKey[nCntModel];										// ブレンドモーションの現在のキー
		KEY KeyNextBlend = g_player.aMotionInfo[g_player.motionTypeBlend].aKeyInfo[(g_player.nKeyBlend + 1) % g_player.nNumKeyBlend].aKey[nCntModel];		// ブレンドモーションの次のキー
		float fRateKey = (float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;					// 現在のモーションカウンター / 現在の再生フレーム
		float fRateKeyBlend = (float)g_player.nCounterMotionBlend / (float)g_player.aMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend].nFrame;	// ブレンドモーションカウンター / ブレンドモーションフレーム
		float fRateBlend = (float)g_player.nCounterBlend / (float)g_player.nFrameBlend;	// ブレンドフレームを処理している比率
		float fDiffKey = 0.0f;	// 現在のモーションの次のキーと現在のキーの差分
		float fDiffKeyBlend = 0.0f;	// ブレンドモーションの次のキーと現在のキーの差分
		float fDiffBrend = 0.0f;	// 現在のモーションとブレンドモーションとの差分
		float fPosXCurrent = 0.0f;	// 現在のX座標の位置を格納
		float fPosYCurrent = 0.0f;	// 現在のY座標の位置を格納
		float fPosZCurrent = 0.0f;	// 現在のZ座標の位置を格納
		float fRotXCurrent = 0.0f;	// 現在のX座標の角度を格納
		float fRotYCurrent = 0.0f;	// 現在のY座標の角度を格納
		float fRotZCurrent = 0.0f;	// 現在のZ座標の角度を格納
		float fPosXBlend = 0.0f;	// ブレンドモーションのX座標の位置を格納
		float fPosYBlend = 0.0f;	// ブレンドモーションのY座標の位置を格納
		float fPosZBlend = 0.0f;	// ブレンドモーションのZ座標の位置を格納
		float fRotXBlend = 0.0f;	// ブレンドモーションのX座標の角度を格納
		float fRotYBlend = 0.0f;	// ブレンドモーションのY座標の角度を格納
		float fRotZBlend = 0.0f;	// ブレンドモーションのZ座標の角度を格納
		float fPosX = 0.0f;	// 求める値[X座標の位置]
		float fPosY = 0.0f; // 求める値[Y座標の位置]
		float fPosZ = 0.0f; // 求める値[Z座標の位置]
		float fRotX = 0.0f; // 求める値[X座標の角度]
		float fRotY = 0.0f; // 求める値[Y座標の角度]
		float fRotZ = 0.0f; // 求める値[Z座標の角度]

		if (g_player.bBlendMotion == true)	// ブレンド有の場合
		{
			// 位置の更新 [Integer division by zero]に対処すること
			// X
			// 現在のモーション情報
			fDiffKey = KeyNext.fPosX - Key.fPosX;
			fPosXCurrent = Key.fPosX + (fDiffKey * fRateKey);
			// ブレンドモーション情報
			fDiffKeyBlend = KeyNextBlend.fPosX - KeyBlend.fPosX;
			fPosXBlend = KeyBlend.fPosX + (fDiffKeyBlend * fRateKeyBlend);
			// 差分を求め、位置を求める
			fDiffBrend = fPosXBlend - fPosXCurrent;
			fPosX = fPosXCurrent + (fDiffBrend * fRateBlend);

			// Y
			// 現在のモーション情報
			fDiffKey = KeyNext.fPosY - Key.fPosY;
			fPosYCurrent = Key.fPosY + (fDiffKey * fRateKey);
			// ブレンドモーション情報
			fDiffKeyBlend = KeyNextBlend.fPosY - KeyBlend.fPosY;
			fPosYBlend = KeyBlend.fPosY + (fDiffKeyBlend * fRateKeyBlend);
			// 差分を求め、位置を求める
			fDiffBrend = fPosYBlend - fPosYCurrent;
			fPosY = fPosYCurrent + (fDiffBrend * fRateBlend);

			// Z
			// 現在のモーション情報
			fDiffKey = KeyNext.fPosZ - Key.fPosZ;
			fPosZCurrent = Key.fPosZ + (fDiffKey * fRateKey);
			// ブレンドモーション情報
			fDiffKeyBlend = KeyNextBlend.fPosZ - KeyBlend.fPosZ;
			fPosZBlend = KeyBlend.fPosZ + (fDiffKeyBlend * fRateKeyBlend);

			// 差分を求め、位置を求める
			fDiffBrend = fPosZBlend - fPosZCurrent;
			fPosZ = fPosZCurrent + (fDiffBrend * fRateBlend);

			// 角度の更新
			// X
			// 現在のモーション情報
			fDiffKey = KeyNext.fRotX - Key.fRotX;
			fRotXCurrent = Key.fRotX + (fDiffKey * fRateKey);
			// 角度補正
			fRotXCurrent = AngleCorrection(fRotXCurrent);
			// ブレンドモーション情報
			fDiffKeyBlend = KeyNextBlend.fRotX - KeyBlend.fRotX;
			// 角度補正
			fDiffKeyBlend = AngleCorrection(fDiffKeyBlend);
			fRotXBlend = KeyBlend.fRotX + (fDiffKeyBlend * fRateKeyBlend);
			// 差分を求め、位置を求める
			fDiffBrend = fRotXBlend - fRotXCurrent;
			fRotX = fRotXCurrent + (fDiffBrend * fRateBlend);

			// Y
			// 現在のモーション情報
			fDiffKey = KeyNext.fRotY - Key.fRotY;
			fRotYCurrent = Key.fRotY + (fDiffKey * fRateKey);
			// 角度補正
			fRotYCurrent = AngleCorrection(fRotYCurrent);
			// ブレンドモーション情報
			fDiffKeyBlend = KeyNextBlend.fRotY - KeyBlend.fRotY;
			// 角度補正
			fDiffKeyBlend = AngleCorrection(fDiffKeyBlend);
			fRotYBlend = KeyBlend.fRotY + (fDiffKeyBlend * fRateKeyBlend);
			// 差分を求め、位置を求める
			fDiffBrend = fRotYBlend - fRotYCurrent;
			fRotY = fRotYCurrent + (fDiffBrend * fRateBlend);

			// Z
			// 現在のモーション情報
			fDiffKey = KeyNext.fRotZ - Key.fRotZ;
			fRotZCurrent = Key.fRotZ + (fDiffKey * fRateKey);
			// 角度補正
			fRotZCurrent = AngleCorrection(fRotZCurrent);
			// ブレンドモーション情報
			fDiffKeyBlend = KeyNextBlend.fRotZ - KeyBlend.fRotZ;
			// 角度補正
			fDiffKeyBlend = AngleCorrection(fDiffKeyBlend);
			fRotZBlend = KeyBlend.fRotZ + (fDiffKeyBlend * fRateKeyBlend);
			// 差分を求め、位置を求める
			fDiffBrend = fRotZBlend - fRotZCurrent;
			fRotZ = fRotZCurrent + (fDiffBrend * fRateBlend);

		}
		else // ブレンド無の場合
		{
			// 位置の更新 [Integer division by zero]に対処すること
			// X
			fDiffKey = KeyNext.fPosX - Key.fPosX;
			fPosX = Key.fPosX + fDiffKey * fRateKey;

			// Y
			fDiffKey = KeyNext.fPosY - Key.fPosY;
			fPosY = Key.fPosY + fDiffKey * fRateKey;

			// Z
			fDiffKey = KeyNext.fPosZ - Key.fPosZ;
			fPosZ = Key.fPosZ + fDiffKey * fRateKey;

			// 角度の更新
			// X
			fDiffKey = KeyNext.fRotX - Key.fRotX;
			// 角度補正
			fDiffKey = AngleCorrection(fDiffKey);
			fRotX = Key.fRotX + fDiffKey * fRateKey;

			// Y
			fDiffKey = KeyNext.fRotY - Key.fRotY;
			// 角度補正
			fDiffKey = AngleCorrection(fDiffKey);
			fRotY = Key.fRotY + fDiffKey * fRateKey;

			// Z
			fDiffKey = KeyNext.fRotZ - Key.fRotZ;
			// 角度補正
			fDiffKey = AngleCorrection(fDiffKey);
			fRotZ = Key.fRotZ + fDiffKey * fRateKey;
		}

		// 角度補正
		fRotX = AngleCorrection(fRotX);
		fRotY = AngleCorrection(fRotY);
		fRotZ = AngleCorrection(fRotZ);

		// モデルの位置を更新
		g_player.aModel[nCntModel].pos.x = fPosX + g_player.aOffSet[nCntModel].x;
		g_player.aModel[nCntModel].pos.y = fPosY + g_player.aOffSet[nCntModel].y;
		g_player.aModel[nCntModel].pos.z = fPosZ + g_player.aOffSet[nCntModel].z;

		// モデルの角度を更新
		g_player.aModel[nCntModel].rot.x = fRotX + g_player.aOffSetRot[nCntModel].x;
		g_player.aModel[nCntModel].rot.y = fRotY + g_player.aOffSetRot[nCntModel].y;
		g_player.aModel[nCntModel].rot.z = fRotZ + g_player.aOffSetRot[nCntModel].z;

		// 角度補正
		g_player.aModel[nCntModel].rot.x = AngleCorrection(g_player.aModel[nCntModel].rot.x);
		g_player.aModel[nCntModel].rot.y = AngleCorrection(g_player.aModel[nCntModel].rot.y);
		g_player.aModel[nCntModel].rot.z = AngleCorrection(g_player.aModel[nCntModel].rot.z);

		if (g_player.bDisp == true)
		{
			PrintDebugProc("%s%d%s : (%f, %f, %f)", "モデル[", nCntModel, "]の位置", g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
			PrintDebugProc("%s%d%s : (%f, %f, %f)", "モデル[", nCntModel, "]の角度", g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.z);
		}
	}
	Goal* pGoal = GetGoal();

	if (g_player.bBlendMotion == true)
	{
		// ブレンドモーションカウンターを加算
		if (pGoal->state == GOALSTATE_END)
		{
			g_player.nCounterMotionBlend += 2;
		}
		else
		{
			g_player.nCounterMotionBlend++;
		}
		
		if (g_player.nCounterMotionBlend >= g_player.aMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend].nFrame)
		{
			g_player.nKeyBlend++;
			g_player.nCounterMotionBlend = RESET_DATA;
			g_player.nKeyBlend = g_player.nKeyBlend % g_player.nNumKeyBlend;
		}

		g_player.nCounterBlend++;	// ブレンドカウンターを加算
		if (g_player.nCounterBlend >= g_player.nFrameBlend)
		{
			g_player.bBlendMotion = false;
			g_player.bFinishMotion = false;
			g_player.nCounterBlend = RESET_DATA;
			g_player.motionType = g_player.motionTypeBlend;
			g_player.bLoopMotion = g_player.aMotionInfo[g_player.motionType].bLoop;
			g_player.nNumKey = (int)g_player.aMotionInfo[g_player.motionType].nNumKey;
			g_player.nKey = g_player.nKeyBlend;
			g_player.nCounterMotion = g_player.nCounterMotionBlend;
		}
	}
	else
	{	// ブレンド無
		// ブレンドモーションカウンターを加算
		if (pGoal->state == GOALSTATE_END)
		{
			g_player.nCounterMotion += 2;
		}
		else
		{
			g_player.nCounterMotion++;
		}
		if (g_player.nCounterMotion >= g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame)
		{
			g_player.nCounterMotion = RESET_DATA;
			g_player.nKey++;
			if (g_player.bLoopMotion == false && g_player.nKey + 1 >= g_player.nNumKey)
			{
				if (g_player.motionType == MOTIONTYPE_ACTION)
				{
					SetMotion(MOTIONTYPE_NEUTRAL, true, MOVE_NEUTRAL);
					g_player.bMoveOld = false;
					g_player.bMove = false;
				}
				else if (g_player.motionType == MOTIONTYPE_LANDING)
				{
					SetMotion(MOTIONTYPE_NEUTRAL, true, MOVE_NEUTRAL);
					g_player.bMoveOld = false;
					g_player.bMove = false;
				}
			}
			g_player.nKey = g_player.nKey % g_player.nNumKey;
		}
		if (g_player.motionType == MOTIONTYPE_JUMP && g_player.nKey >= 5)
		{
			g_player.nKey = 5;
			g_player.nCounterMotion = RESET_DATA;
		}
	}

	if (g_player.motionType == MOTIONTYPE_MOVE &&	// 移動モーション
		g_player.nKey % 2 == RESET_DATA &&			// 既定のキー
		g_player.nCounterMotion == RESET_DATA)		// ちょうどモーションが切り替わる
	{
		SetEffect(g_player.pos, DEFAULT, COLOR_05fSHADOW, EFFECTTYPE_MOVING, 0.0f, 20, 10.0f);
	}
	else if (g_player.motionType == MOTIONTYPE_JUMP)
	{
		SetEffect(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 20.0f, g_player.pos.z), DEFAULT, COLOR_ORANGE, EFFECTTYPE_EXPLOSION, 0.0f, 20, 10.0f);
	}

	if (g_player.bDisp == true)
	{
		PrintDebugProc("%s : %d", "現在のキー", g_player.nKey);
		PrintDebugProc("%s : %d", "キーカウンター", g_player.nCounterMotion);
		if (g_player.bLoopMotion == true)
		{
			PrintDebugProc("%s : %s", "ループ状態", "true");
		}
		else
		{
			PrintDebugProc("%s : %s", "ループ状態", "false");
		}
		if (g_player.bBlendMotion == true)
		{
			PrintDebugProc("%s : %s", "ブレンドモーション", "true");
			PrintDebugProc("%s : %d", "ブレンドモーションの種類", g_player.motionTypeBlend);
			PrintDebugProc("%s : %d", "ブレンドカウンター", g_player.nCounterBlend);
			PrintDebugProc("%s : %d", "処理するフレーム数", g_player.nFrameBlend);
		}
		else
		{
			PrintDebugProc("%s : %s", "ブレンドモーション", "false");
		}
	}
}

//======================================================================================
// モーションの設定
//======================================================================================
void SetMotion(MOTIONTYPE type, bool bBlendMotion, int nFrameBlend)
{
	g_player.bBlendMotion = bBlendMotion;
	if (g_player.bBlendMotion == true)
	{
		g_player.motionTypeBlend = type;
		g_player.nFrameBlend = nFrameBlend;
		g_player.nNumKeyBlend = g_player.aMotionInfo[g_player.motionTypeBlend].nNumKey;
		g_player.nKeyBlend = 0;

		g_player.bLoopMotionBlend = g_player.aMotionInfo[g_player.motionTypeBlend].bLoop;
		g_player.nCounterMotionBlend = RESET_DATA;
		g_player.nCounterBlend = RESET_DATA;
	}
	else
	{
		g_player.motionType = type;
		g_player.nNumKey = g_player.aMotionInfo[g_player.motionType].nNumKey;
		g_player.nKey = 0;
		g_player.nCounterMotion = RESET_DATA;
		g_player.bLoopMotion = g_player.aMotionInfo[g_player.motionType].bLoop;

		// プレイヤーのオフセット情報を設定
		SetPlayerOffset();
	}
}

//======================================================================================
// プレイヤーのオフセット情報を設定
//======================================================================================
void SetPlayerOffset(void)
{
	// 頭[0]の情報を設定
	g_player.aModel[0].nIdxModelParent = -1;
	g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 19.5f, 0.0f);
	g_player.aOffSet[0] = g_player.aModel[0].pos;
	g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.aOffSetRot[0] = g_player.aModel[0].rot;
	// 左腕[1]の情報を設定
	g_player.aModel[1].nIdxModelParent = 0;
	g_player.aModel[1].pos = D3DXVECTOR3(12.5f, -2.0f, 0.0f);
	g_player.aOffSet[1] = g_player.aModel[1].pos;
	g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 4.75f);
	g_player.aOffSetRot[1] = g_player.aModel[1].rot;
	// 右腕[2]の情報を設定
	g_player.aModel[2].nIdxModelParent = 0;
	g_player.aModel[2].pos = D3DXVECTOR3(-12.5f, -2.0f, 0.0f);
	g_player.aOffSet[2] = g_player.aModel[2].pos;
	g_player.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, -4.75f);
	g_player.aOffSetRot[2] = g_player.aModel[2].rot;
	// 左足[3]の情報を設定
	g_player.aModel[3].nIdxModelParent = 0;
	g_player.aModel[3].pos = D3DXVECTOR3(5.0f, -12.5f, 0.0f);
	g_player.aOffSet[3] = g_player.aModel[3].pos;
	g_player.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.aOffSetRot[3] = g_player.aModel[3].rot;
	// 右足[4]の情報を設定
	g_player.aModel[4].nIdxModelParent = 0;
	g_player.aModel[4].pos = D3DXVECTOR3(-5.0f, -12.5f, 0.0f);
	g_player.aOffSet[4] = g_player.aModel[4].pos;
	g_player.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.aOffSetRot[4] = g_player.aModel[4].rot;
}
