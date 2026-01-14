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
#define NUM_MOUSE_MAX		(3)			// ボタンの最大数
#define JOYSTICKVALUE_MAX	(32767)		// スティックの値の最大
#define JOYSTICKVALUE_MIN	(-32768)	// スティックの値の最小
#define CUSTOM_DEADZONE		(512)		// デッドゾーン

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
	JOYSTICKL_UP = 0,			// 上(Lスティック)
	JOYSTICKL_DOWN,				// 下(Lスティック)
	JOYSTICKL_LEFT,				// 左(Lスティック)
	JOYSTICKL_RIGHT,			// 右(Lスティック)
	JOYSTICKR_UP,				// 上(Rスティック)
	JOYSTICKR_DOWN,				// 下(Rスティック)
	JOYSTICKR_LEFT,				// 左(Rスティック)
	JOYSTICKR_RIGHT,			// 右(Rスティック)
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
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
bool GetJoypadRepeat(JOYKEY key);
bool GetJoypadAny(void);
bool GetJoypadStickLeft(float* pValueH, float* pValueV);
bool GetJoypadStickLeft(float* pValueH, float* pValueV);
bool GetJoypadStickPress(JOYSTICK stick);
bool GetJoypadStickRepeat(JOYSTICK stick);
void SetJoypadVibration(WORD nLVibration, WORD nRVibration, int nVibCounter);
XINPUT_STATE *GetJoypadState(void);
bool GetJoypadStroke(WORD key);
XINPUT_KEYSTROKE* GetJoypadStroke(void);
bool GetJoypadControl(void);

HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nButton);
bool GetMouseTrigger(int nButton);
bool GetMouseRelease(int nButton);
POINT GetMousePos(void);

#endif
