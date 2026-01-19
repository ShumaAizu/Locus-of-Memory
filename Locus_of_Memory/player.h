//========================================================
// 
// 3Dアクションゲーム_MASTER KEY[player.h]
// 
//========================================================
#ifndef _PLAYER_H_
#define	_PLAYER_H_
#include "model.h"

// マクロ定義
#define MAX_MAT		(16)	// マテリアルの最大数

// プレイヤーの表示状態を管理
typedef enum
{
	PLAYERSTATE_NONE = RESET_DATA,	// ゴールしていない状態
	PLAYERSTATE_GOAL,				// ゴール後の状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

// モデルの構造体
typedef struct
{
	D3DXMATRIX	mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3	pos;					// プレイヤーの位置
	D3DXVECTOR3	posOld;					// プレイヤーの1フレーム前の位置
	D3DXVECTOR3	move;					// プレイヤーの移動量
	D3DXVECTOR3	rot;					// プレイヤーの向き
	D3DXVECTOR3	rotDest;				// プレイヤーの目的の向き
	Model		aModel[MAX_MODEL];		// モデル(パーツ)
	D3DXVECTOR3 aOffSet[MAX_MODEL];		// モデルのオフセット[位置]を保存
	D3DXVECTOR3 aOffSetRot[MAX_MODEL];	// モデルのオフセット[角度]を保存
	PLAYERSTATE	state;					// プレイヤーの状態管理
	int		nCounterState;				// 状態管理カウンター
	int		nNumModel;					// モデル(パーツ)の総数
	int		nIdxShadow;					// 影のインデックス
	bool	bJump;						// 空中状態を管理
	bool	bMove;						// 移動状態を管理
	bool	bMoveOld;					// 移動状態を管理
	bool	bDisp;						// 表示状態
	float	fMove;						// 移動量
	float	fInertia;					// 慣性
	MOTION_INFO	aMotionInfo[MOTIONTYPE_MAX];	// 現在のモーション情報
	int			nNumMotion;						// 現在のモーションの総数
	MOTIONTYPE	motionType;						// 現在のモーションの種類
	bool		bLoopMotion;					// 現在のモーションのループを管理
	int			nNumKey;						// 現在のキーの総数
	int			nKey;							// 現在実行されているキーの番号
	int			nCounterMotion;					// 現在のモーションのカウンター
	bool		bFinishMotion;					// 現在のモーションが終了しているか
	bool		bBlendMotion;					// ブレンドモーションするかどうか
	MOTIONTYPE	motionTypeBlend;				// ブレンドモーションの種類
	bool		bLoopMotionBlend;				// ブレンドモーションのループを管理
	int			nNumKeyBlend;					// ブレンドモーションのキーの総数
	int			nKeyBlend;						// ブレンドモーションの実行されているキーの番号
	int			nCounterMotionBlend;			// ブレンドモーションのカウンター
	int			nFrameBlend;					// ブレンドフレーム数(何フレームかけてブレンドをするか)
	int			nCounterBlend;					// ブレンドカウンター(ブレンドフレームが何フレーム処理されているか)
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void InitMotion(void);
void UpdateMotion(void);
void SetMotion(MOTIONTYPE type, bool bBlendMotion, int nFrameBlend);
void SetPlayerOffset(void);

#endif