//=============================================================================
//
//	入力処理 [input.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];							// 
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];					// キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];					// キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];					// キーボードのリピート情報
BYTE g_aKeyCount[NUM_KEY_MAX];							// キーボードのカウント
JoypadState g_JoypadState[MAX_JOYPAD];					// ジョイパッドの情報
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
	memset(&g_JoypadState, 0, sizeof(JoypadState));
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

	JoypadState* pJoypadState = &g_JoypadState[0];

	// ジョイパッドの状態を取得
	for (int nCntJoypad = 0; nCntJoypad < MAX_JOYPAD; nCntJoypad++, pJoypadState++)
	{
		XINPUT_STATE* pJoykeyState = &pJoypadState->JoykeyState;
		XINPUT_STATE* pJoykeyStateTrigger = &pJoypadState->JoykeyStateTrigger;
		XINPUT_STATE* pJoykeyStateRelease = &pJoypadState->JoykeyStateRelease;
		XINPUT_STATE* pJoykeyStateRepeat = &pJoypadState->JoykeyStateRepeat;
		XINPUT_VIBRATION* pJoypadVibration = &pJoypadState->JoypadVibration;

		if (XInputGetState(nCntJoypad, &joykeyState) == ERROR_SUCCESS)
		{// ジョイパッドの情報が取得出来たら
			pJoykeyStateTrigger->Gamepad.wButtons = (pJoykeyState->Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
			pJoykeyStateRelease->Gamepad.wButtons = (pJoykeyState->Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & pJoykeyState->Gamepad.wButtons;
			pJoykeyStateRepeat->Gamepad.wButtons = (pJoykeyState->Gamepad.wButtons & joykeyState.Gamepad.wButtons);
			*pJoykeyState = joykeyState;		// ジョイパッドのプレス情報を保存

			PrintDebugProc("%f", pJoykeyState->Gamepad.bLeftTrigger);

			//-----スティックの情報-----//

			// 左スティック
			if (joykeyState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが上に倒されていたら
				pJoypadState->bJoyStickL[JOYSTICK_UP] = true;
			}
			else
			{
				pJoypadState->bJoyStickL[JOYSTICK_UP] = false;
			}

			if (joykeyState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが下に倒されていたら
				pJoypadState->bJoyStickL[JOYSTICK_DOWN] = true;
			}
			else
			{
				pJoypadState->bJoyStickL[JOYSTICK_DOWN] = false;
			}

			if (joykeyState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが左に倒されていたら
				pJoypadState->bJoyStickL[JOYSTICK_LEFT] = true;
			}
			else
			{
				pJoypadState->bJoyStickL[JOYSTICK_LEFT] = false;
			}

			if (joykeyState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが右に倒されていたら
				pJoypadState->bJoyStickL[JOYSTICK_RIGHT] = true;
			}
			else
			{
				pJoypadState->bJoyStickL[JOYSTICK_RIGHT] = false;
			}

			// 右スティック
			if (joykeyState.Gamepad.sThumbRY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが上に倒されていたら
				pJoypadState->bJoyStickR[JOYSTICK_UP] = true;
			}
			else
			{
				pJoypadState->bJoyStickR[JOYSTICK_UP] = false;
			}

			if (joykeyState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが下に倒されていたら
				pJoypadState->bJoyStickR[JOYSTICK_DOWN] = true;
			}
			else
			{
				pJoypadState->bJoyStickR[JOYSTICK_DOWN] = false;
			}

			if (joykeyState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが左に倒されていたら
				pJoypadState->bJoyStickR[JOYSTICK_LEFT] = true;
			}
			else
			{
				pJoypadState->bJoyStickR[JOYSTICK_LEFT] = false;
			}

			if (joykeyState.Gamepad.sThumbRX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{// スティックが右に倒されていたら
				pJoypadState->bJoyStickR[JOYSTICK_RIGHT] = true;
			}
			else
			{
				pJoypadState->bJoyStickR[JOYSTICK_RIGHT] = false;
			}
		}

		// 振動の更新処理
		if (pJoypadVibration->wLeftMotorSpeed != NULL || pJoypadVibration->wRightMotorSpeed != NULL)
		{// バイブレーションが設定されている
			pJoypadState->nVibCounter--;		// カウントを減らす
			if (pJoypadState->nVibCounter <= 0)
			{// カウントが回り切ったら
				// リセット
				pJoypadVibration->wLeftMotorSpeed = 0;
				pJoypadVibration->wRightMotorSpeed = 0;

				XInputSetState(nCntJoypad, pJoypadVibration);
			}
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
bool GetJoypadPress(JOYKEY key, int nIdx)
{
	return (g_JoypadState[nIdx].JoykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのトリガー情報を取得
//=============================================================================
bool GetJoypadTrigger(JOYKEY key, int nIdx)
{
	return (g_JoypadState[nIdx].JoykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリリース情報を取得
//=============================================================================
bool GetJoypadRelease(JOYKEY key, int nIdx)
{
	return (g_JoypadState[nIdx].JoykeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリピート情報を取得
//=============================================================================
bool GetJoypadRepeat(JOYKEY key, int nIdx)
{
	JoypadState* pJoypadState = &g_JoypadState[nIdx];

	PrintDebugProc("%d\n", pJoypadState->JoykeyStateTrigger.Gamepad.wButtons);

	if (pJoypadState->JoykeyStateTrigger.Gamepad.wButtons & (0x01 << key))
	{// 最初はトリガー
		pJoypadState->nRepeatKeyCounter[key] = 0;		// フレームカウンターをリセット
		return true;									// trueを返して終了
	}

	if (pJoypadState->JoykeyStateRepeat.Gamepad.wButtons & (0x01 << key))
	{// リピートしてるなら入る
		pJoypadState->nRepeatKeyCounter[key]++;		// フレームカウンターを増やす
		if (pJoypadState->nRepeatKeyCounter[key] >= 30)
		{// カウントが一定以上なら
			if (pJoypadState->nRepeatKeyCounter[key] % 5 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	ジョイスティックのリピート情報を取得 (Lスティック)
//=============================================================================
bool GetJoypadLeftTriggePress(int nIdx)
{
	if (g_JoypadState[nIdx].JoykeyState.Gamepad.bLeftTrigger >= ZLRTRIGGER)
	{
		return true;
	}
}

//=============================================================================
//	ジョイスティックのプレス情報を取得 (Lスティック)
//=============================================================================
bool GetJoypadStickPressL(JOYSTICK stick, int nIdx)
{
	return g_JoypadState[nIdx].bJoyStickL[stick];
}

//=============================================================================
//	ジョイスティックのプレス情報を取得 (Rスティック)
//=============================================================================
bool GetJoypadStickPressR(JOYSTICK stick, int nIdx)
{
	return g_JoypadState[nIdx].bJoyStickR[stick];
}

//=============================================================================
//	ジョイスティックのリピート情報を取得 (Lスティック)
//=============================================================================
bool GetJoypadStickRepeatL(JOYSTICK stick, int nIdx)
{
	JoypadState* pJoypadState = &g_JoypadState[nIdx];

	if (pJoypadState->bJoyStickL[stick] == true && pJoypadState->nRepeatStickCounterL[stick] == 0)
	{
		pJoypadState->nRepeatStickCounterL[stick] = 5;
		return true;
	}

	if (g_JoypadState[nIdx].bJoyStickL[stick] == true)
	{
		pJoypadState->nRepeatStickCounterL[stick]++;
		if (pJoypadState->nRepeatStickCounterL[stick] >= 30)
		{
			if (pJoypadState->nRepeatStickCounterL[stick] % 5 == 0)
			{
				return true;
			}
		}

		return false;
	}

	if (g_JoypadState[nIdx].bJoyStickL[stick] == false)
	{
		pJoypadState->nRepeatStickCounterL[stick] = 0;
		return false;
	}

	return false;

}

//=============================================================================
//	ジョイスティックのリピート情報を取得 (Rスティック)
//=============================================================================
bool GetJoypadStickRepeatR(JOYSTICK stick, int nIdx)
{
	JoypadState* pJoypadState = &g_JoypadState[nIdx];

	if (g_JoypadState[nIdx].bJoyStickR[stick] == true && pJoypadState->nRepeatStickCounterR[stick] == 0)
	{
		pJoypadState->nRepeatStickCounterR[stick] = 5;
		return true;
	}

	if (g_JoypadState[nIdx].bJoyStickR[stick] == true)
	{
		pJoypadState->nRepeatStickCounterR[stick]++;
		if (pJoypadState->nRepeatStickCounterR[stick] >= 30)
		{
			if (pJoypadState->nRepeatStickCounterR[stick] % 5 == 0)
			{
				return true;
			}
		}

		return false;
	}

	if (g_JoypadState[nIdx].bJoyStickR[stick] == false)
	{
		pJoypadState->nRepeatStickCounterR[stick] = 0;
		return false;
	}

	return false;

}

//=============================================================================
//	ジョイパッドの左スティック取得処理
//=============================================================================
bool GetJoypadStickLeft(float* pValueH, float* pValueV, int nIdx)
{
	XINPUT_STATE* pJoykeyState = &g_JoypadState[nIdx].JoykeyState;

	float fValueH, fValueV;

	fValueH = pJoykeyState->Gamepad.sThumbLX;
	fValueV = pJoykeyState->Gamepad.sThumbLY;

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
bool GetJoypadStickRight(float* pValueH, float* pValueV, int nIdx)
{
	XINPUT_STATE* pJoykeyState = &g_JoypadState[nIdx].JoykeyState;

	float fValueH, fValueV;

	fValueH = pJoykeyState->Gamepad.sThumbRX;
	fValueV = pJoykeyState->Gamepad.sThumbRY;

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
//	ジョイパッドの情報を取得
//=============================================================================
bool GetJoypadAny(int nIdx)
{
	for (int nCntJoyKey = 0; nCntJoyKey < JOYKEY_MAX; nCntJoyKey++)
	{
		if (g_JoypadState[nIdx].JoykeyStateTrigger.Gamepad.wButtons & (0x01 << nCntJoyKey))
		{
			return true;
		}
	}

	for (int nCntJoyStick = 0; nCntJoyStick < JOYSTICK_MAX; nCntJoyStick++)
	{
		if (g_JoypadState[nIdx].bJoyStickL[nCntJoyStick] == true)
		{
			return true;
		}

		if (g_JoypadState[nIdx].bJoyStickR[nCntJoyStick] == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドの振動設定
//=============================================================================
void SetJoypadVibration(int nLVibration, int nRVibration, int nVibCounter, int nIdx)
{
	g_JoypadState[nIdx].JoypadVibration.wLeftMotorSpeed = nLVibration;
	g_JoypadState[nIdx].JoypadVibration.wRightMotorSpeed = nRVibration;
	g_JoypadState[nIdx].nVibCounter = nVibCounter;

	XInputSetState(nIdx, &g_JoypadState[nIdx].JoypadVibration);
}

//=============================================================================
//	ジョイパッドの情報取得
//=============================================================================
JoypadState* GetJoypadState(int nIdx)
{
	return &g_JoypadState[nIdx];
}

//=============================================================================
//	ジョイパッドの情報取得 (機能なし)
//=============================================================================
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