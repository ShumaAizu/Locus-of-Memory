//=============================================================================
//
//	入力処理 [input.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "input.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];					// キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];					// キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];					// キーボードのリピート情報
BYTE g_aKeyCount[NUM_KEY_MAX];							// キーボードのカウント
XINPUT_STATE g_joykeyState;								// ジョイパッドのプレス情報
XINPUT_STATE g_joykeyStateTrigger;						// ジョイパッドのトリガー情報
XINPUT_STATE g_joykeyStateRelease;						// ジョイパッドのリリース情報
XINPUT_STATE g_joykeyStateRepeat;						// ジョイパッドのリピート情報
XINPUT_VIBRATION g_joypadVibration;						// ジョイパッドの振動情報
XINPUT_KEYSTROKE g_joypadStroke[NUM_JOYSTROKE_MAX];
int g_nVibCounter = 0;
bool g_bJoyStick[JOYSTICK_MAX] = {};
bool g_bJoyStickRepeat[NUM_JOYSTROKE_MAX] = {};
bool g_JoypadControl = false;
DIMOUSESTATE g_mousestate = {};

LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;		// 入力デバイス(マウス)へのポインタ

DIMOUSESTATE g_CurrentMouseState;				// マウスの入力情報
DIMOUSESTATE g_PrevMouseState;					// マウスの過去の入力情報
BYTE g_aMouseState[NUM_MOUSE_MAX];				// マウスのプレス情報
BYTE g_aMouseStateTrigger[NUM_MOUSE_MAX];		// マウスのトリガー情報
BYTE g_aMouseStateRelease[NUM_MOUSE_MAX];		// マウスのリリース情報
POINT g_mousePos = {};							// マウスの位置

HWND g_InputhWnd = NULL;						// 座標変換に使用するウィンドウのハンドル

//=============================================================================
//	キーボード初期化処理
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// デバイスへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	// ウィンドウハンドルを保存
	g_InputhWnd = hWnd;

	return S_OK;
}

//=============================================================================
//	ジョイパッドの初期化処理
//=============================================================================
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&g_joykeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joypadVibration, 0, sizeof(XINPUT_VIBRATION));
	memset(&g_joypadStroke[0], 0, sizeof(XINPUT_KEYSTROKE));
	g_JoypadControl = false;
	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//=============================================================================
//	キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	// 入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
//	ジョイパッドの終了処理
//=============================================================================
void UninitJoypad(void)
{
	// Xinputのステートを設定(無効にする)
	XInputEnable(false);
}

//=============================================================================
//	キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		// トリガー
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	// リリース
			g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] & aKeyState[nCntKey]);							// リピート
			g_aKeyState[nCntKey] = aKeyState[nCntKey];															// キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
//	ジョイパッドの更新処理
//=============================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		// ジョイパッドの入力情報
	XINPUT_KEYSTROKE joykeystroke;

	// ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyStateTrigger.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
		g_joykeyStateRelease.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & g_joykeyStateRelease.Gamepad.wButtons;
		g_joykeyStateRepeat.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons);
		g_joykeyState = joykeyState;		// ジョイパッドのプレス情報を保存

		// スティックの状態
		if (joykeyState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKL_UP] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKL_UP] = false;
		}

		if (joykeyState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKL_DOWN] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKL_DOWN] = false;
		}

		if (joykeyState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKL_LEFT] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKL_LEFT] = false;
		}

		if (joykeyState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKL_RIGHT] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKL_RIGHT] = false;
		}

		if (joykeyState.Gamepad.sThumbRY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKR_UP] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKR_UP] = false;
		}

		if (joykeyState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKR_DOWN] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKR_DOWN] = false;
		}

		if (joykeyState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKR_LEFT] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKR_LEFT] = false;
		}

		if (joykeyState.Gamepad.sThumbRX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICKR_RIGHT] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICKR_RIGHT] = false;
		}
	}

	if (XInputGetKeystroke(0, 0, &joykeystroke) == ERROR_SUCCESS)
	{
		for (int nCntKey = 0; nCntKey < NUM_JOYSTROKE_MAX; nCntKey++)
		{
			if (joykeystroke.VirtualKey - JOYKEYSTROKE_START == nCntKey)
			{
				g_joypadStroke[nCntKey] = joykeystroke;
			}
		}
	}

	// 振動修了処理
	if (g_joypadVibration.wLeftMotorSpeed != NULL || g_joypadVibration.wRightMotorSpeed != NULL)
	{
		g_nVibCounter--;
		if (g_nVibCounter <= 0)
		{
			g_joypadVibration.wLeftMotorSpeed = 0;
			g_joypadVibration.wRightMotorSpeed = 0;

			XInputSetState(0, &g_joypadVibration);
		}
	}
}

//=============================================================================
//	キーボードのプレス情報を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのトリガー情報を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのリリース情報を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのリピート情報を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	// フレームカウンターを宣言
	int static nFrameCounter[NUM_KEY_MAX] = {};

	if (g_aKeyStateTrigger[nKey] & 0x80)
	{// 最初はトリガー
		nFrameCounter[nKey] = 0;		// フレームカウンターをリセット
		return true;					// trueを返して終了
	}

	if (g_aKeyStateRepeat[nKey] & 0x80)
	{// リピートしてるなら入る
   		nFrameCounter[nKey]++;		// フレームカウンターを増やす
		if (nFrameCounter[nKey] >= 30)
		{// カウントが一定以上なら
			if (nFrameCounter[nKey] % 5 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	キーボードの情報を取得
//=============================================================================
bool GetKeyboardAny(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (g_aKeyState[nCntKey] & 0x80)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドのプレス情報を取得
//=============================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのプレス情報を取得
//=============================================================================
bool GetJoypadStroke(WORD key)
{
	static int nCounter[NUM_JOYSTROKE_MAX] = {};		// カウンターを回す
	if (g_joypadStroke[key - JOYKEYSTROKE_START].VirtualKey & key)
	{// 取得したキーが一致していたら
		if (g_bJoyStickRepeat[key - JOYKEYSTROKE_START] == false && g_joypadStroke[key - JOYKEYSTROKE_START].Flags == XINPUT_KEYSTROKE_KEYDOWN)
		{// リピートがオフかつプレスの時
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = true;		// リピート待機
			return true;					// いったん返す
		}

		if (g_joypadStroke[key - JOYKEYSTROKE_START].Flags == 5)
		{// リピートになったら
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = false;		// 待機状態から戻す
			
			nCounter[key - JOYKEYSTROKE_START]++;						// カウントを回す
			if (nCounter[key - JOYKEYSTROKE_START] % 5 == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			nCounter[key - JOYKEYSTROKE_START] = 0;
		}

		if(g_joypadStroke[key - JOYKEYSTROKE_START].Flags == XINPUT_KEYSTROKE_KEYUP)
		{
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = false;		// 待機状態から戻す
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドのトリガー情報を取得
//=============================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリリース情報を取得
//=============================================================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリピート情報を取得
//=============================================================================
bool GetJoypadRepeat(JOYKEY key)
{
	// フレームカウンターを宣言
	int static nFrameCounter[JOYKEY_MAX] = {};

	if (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key))
	{// 最初はトリガー
		nFrameCounter[key] = 0;		// フレームカウンターをリセット
		return true;				// trueを返して終了
	}

	if (g_joykeyStateRepeat.Gamepad.wButtons & (0x01 << key))
	{// リピートしてるなら入る
		nFrameCounter[key]++;		// フレームカウンターを増やす
		if (nFrameCounter[key] >= 30)
		{// カウントが一定以上なら
			if (nFrameCounter[key] % 5 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	ジョイスティックのプレス情報を取得
//=============================================================================
bool GetJoypadStickPress(JOYSTICK stick)
{
	return g_bJoyStick[stick];
}

//=============================================================================
//	ジョイスティックのリピート情報を取得
//=============================================================================
bool GetJoypadStickRepeat(JOYSTICK stick)
{
	// フレームカウンター
	static int nFrameCounter[JOYSTICK_MAX] = {};

	if (g_bJoyStick[stick] == true && nFrameCounter[stick] == 0)
	{
		nFrameCounter[stick] = 5;
		return true;
	}

	if (g_bJoyStick[stick] == true)
	{
		nFrameCounter[stick]++;
		if (nFrameCounter[stick] >= 30)
		{
			if (nFrameCounter[stick] % 5 == 0)
			{
				return true;
			}
		}
		
		return false;
	}

	if (g_bJoyStick[stick] == false)
	{
		nFrameCounter[stick] = 0;
		return false;
	}

	return false;

}

//=============================================================================
//	ジョイパッドの情報を取得
//=============================================================================
bool GetJoypadAny(void)
{
	for (int nCntJoyKey = 0; nCntJoyKey < JOYKEY_MAX; nCntJoyKey++)
	{
		if (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << nCntJoyKey))
		{
			return true;
		}
	}

	for (int nCntJoyStick = 0; nCntJoyStick < JOYSTICK_MAX; nCntJoyStick++)
	{
		if (g_bJoyStick[nCntJoyStick] == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドの左スティック取得処理
//=============================================================================
bool GetJoypadStickLeft(float* pValueH, float* pValueV)
{
	float fValueH, fValueV;

	fValueH = g_joykeyState.Gamepad.sThumbLX;
	fValueV = g_joykeyState.Gamepad.sThumbLY;

	if (SQRTF(fValueH, fValueV) * 0.5f > CUSTOM_DEADZONE)
	{// デッドゾーン外なら
		// 正規化
		fValueH = (fValueH) / (JOYSTICKVALUE_MAX - CUSTOM_DEADZONE);
		fValueV = (fValueV) / (JOYSTICKVALUE_MAX - CUSTOM_DEADZONE);

		// 値を渡す
		*pValueH = fValueH;
		*pValueV = fValueV;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
//	ジョイパッドの右スティック取得処理
//=============================================================================
bool GetJoypadStickRight(float* pValueH, float* pValueV)
{
	float fValueH, fValueV;

	fValueH = g_joykeyState.Gamepad.sThumbRX;
	fValueV = g_joykeyState.Gamepad.sThumbRY;

	if (SQRTF(fValueH, fValueV) * 0.5f > CUSTOM_DEADZONE)
	{// デッドゾーン外なら
		// 正規化
		fValueH = (fValueH) / (JOYSTICKVALUE_MAX - CUSTOM_DEADZONE);
		fValueV = (fValueV) / (JOYSTICKVALUE_MAX - CUSTOM_DEADZONE);

		// 値を渡す
		*pValueH = fValueH;
		*pValueV = fValueV;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
//	ジョイパッドの振動設定
//=============================================================================
void SetJoypadVibration(WORD nLVibration, WORD nRVibration, int nVibCounter)
{
	g_joypadVibration.wLeftMotorSpeed = nLVibration;
	g_joypadVibration.wRightMotorSpeed = nRVibration;
	g_nVibCounter = nVibCounter;

	XInputSetState(0, &g_joypadVibration);
}

//=============================================================================
//	ジョイパッドの情報取得
//=============================================================================
XINPUT_STATE *GetJoypadState(void)
{
	return &g_joykeyState;
}

//=============================================================================
//	ジョイパッドの情報取得
//=============================================================================
XINPUT_KEYSTROKE* GetJoypadStroke(void)
{
	return &g_joypadStroke[0];
}

bool GetJoypadControl(void)
{
	return g_JoypadControl;
}

//================================================================================================================
// マウスの初期化処理
//================================================================================================================
HRESULT InitMouse(HWND hWnd)
{
	// 入力デバイス(マウス)の生成
	g_pInput->CreateDevice(
		GUID_SysMouse,
		&g_pDevMouse,
		NULL
	);

	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	g_pDevMouse->Acquire();

	return S_OK;
}

//================================================================================================================
// マウスの終了処理
//================================================================================================================
void UninitMouse(void)
{
	// 入力デバイス(マウス)の破棄
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//================================================================================================================
// マウスの更新処理
//================================================================================================================
void UpdateMouse(void)
{
	BYTE aMouseState[NUM_MOUSE_MAX];			// マウスの入力情報

	// 更新前に過去の入力情報(マウス)を保存
	g_PrevMouseState = g_CurrentMouseState;

	// 現在のマウスの入力情報を取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState)))
	{
		for (int nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			aMouseState[nCntMouse] = g_CurrentMouseState.rgbButtons[nCntMouse];
			g_aMouseStateTrigger[nCntMouse] = ((aMouseState[nCntMouse] ^ g_aMouseState[nCntMouse]) & aMouseState[nCntMouse]);
			g_aMouseStateRelease[nCntMouse] = (g_aMouseState[nCntMouse] & (g_aMouseState[nCntMouse] ^ aMouseState[nCntMouse]));
			g_aMouseState[nCntMouse] = aMouseState[nCntMouse];
		}
	}
	else
	{
		g_pDevMouse->Acquire();			// マウスへのアクセス権を取得
	}
}

//================================================================================================================
// マウスのプレス情報を取得
//================================================================================================================
bool GetMousePress(int nButton)
{
	return (g_aMouseState[nButton] & 0x80) ? true : false;
}

//================================================================================================================
// マウスのトリガー情報を取得
//================================================================================================================
bool GetMouseTrigger(int nButton)
{
	return (g_aMouseStateTrigger[nButton] & 0x80) ? true : false;
}

//================================================================================================================
// マウスのリリース情報を取得
//================================================================================================================
bool GetMouseRelease(int nButton)
{
	return (g_aMouseStateRelease[nButton] & 0x80) ? true : false;
}

//================================================================================================================
// マウスの位置情報を取得
//================================================================================================================
POINT GetMousePos(void)
{
	POINT mousePos;

	// マウスの位置を取得
	if (GetCursorPos(&mousePos) != TRUE)
	{// 取得失敗
		g_pDevMouse->Acquire();			// マウスへのアクセス権を取得
	}
	else
	{// 取得成功
		// 取得した座標をウィンドウ内の座標に変換
		ScreenToClient(g_InputhWnd, &mousePos);
		g_mousePos = mousePos;			// 現在の位置を保存
	}

	return g_mousePos;
}