//=============================================================================
//
//	入力処理 [input.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX			(256)		//キーの最大数
#define NUM_JOYSTROKE_MAX	(38)
#define JOYKEYSTROKE_START	(0x5800)
#define NUM_MOUSE_MAX		(3)				// ボタンの最大数
#define MAX_JOYPAD			(4)			// ジョイパッドの最大数

//*****************************************************************************
// ジョイパッドのキーの種類
//*****************************************************************************
typedef enum
{
	JOYKEY_UP = 0,				// 十字キー上
	JOYKEY_DOWN,				// 十字キー下
	JOYKEY_LEFT,				// 十字キー左
	JOYKEY_RIGHT,				// 十字キー右
	JOYKEY_START,				// スタートボタン
	JOYKEY_BACK,				// バックボタン
	JOYKEY_LEFT_THUMB,			// 左スティック押し込み
	JOYKEY_RIGHT_THUMB,			// 右スティック押し込み
	JOYKEY_LEFT_SHOULDER,		// Lボタン
	JOYKEY_RIGHT_SHOULDER,		// Rボタン
	JOYKEY_NULL1,				// 未定義
	JOYKEY_NULL2,				// 未定義
	JOYKEY_A,					// Aボタン
	JOYKEY_B,					// Bボタン
	JOYKEY_X,					// Xボタン
	JOYKEY_Y,					// Yボタン
	JOYKEY_MAX					// 総数
}JOYKEY;

//*****************************************************************************
// ジョイパッドのスティックの種類
//*****************************************************************************
typedef enum
{
	JOYSTICK_UP = 0,			// 上
	JOYSTICK_DOWN,				// 下
	JOYSTICK_LEFT,				// 左
	JOYSTICK_RIGHT,				// 右
	JOYSTICK_MAX
}JOYSTICK;

//**********************************************************************************
//*** マウスのボタンの種類 ***
//**********************************************************************************
typedef enum
{
	MOUSEKEY_LEFT = 0,		// 左クリック
	MOUSEKEY_RIGHT,			// 右クリック
	MOUSEKEY_WHEEL,			// 中クリック
	MOUSEKEY_MAX
}MOUSEKEY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardAny(void);

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key, int nIdx);
bool GetJoypadTrigger(JOYKEY key, int nIdx);
bool GetJoypadRelease(JOYKEY key, int nIdx);
bool GetJoypadRepeat(JOYKEY key, int nIdx);
bool GetJoypadAny(void);
bool GetJoypadStickPress(JOYSTICK stick);
bool GetJoypadStickRepeat(JOYSTICK stick);
void SetJoypadVibration(int nLVibration, int nRVibration, int nVibCounter, int nIdx);
XINPUT_STATE* GetJoypadState(void);
bool GetJoypadControl(void);

HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nButton);
bool GetMouseTrigger(int nButton);
bool GetMouseRelease(int nButton);
POINT GetMousePos(void);

#endif
