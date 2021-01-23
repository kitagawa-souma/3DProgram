#include "DirectInput.h"

LPDIRECTINPUT8 g_InputInterface;			// DIRECTINPUT8のポインタ
LPDIRECTINPUTDEVICE8 g_InputKeyboardDevice;	// Keyboard用Deviceのポインタ

InputState g_KeyStates[256];

InputState UpdateInputState(bool is_push, InputState state);

InputState UpdateInputState(bool is_push, InputState state)
{
	if (is_push == true)
	{
		//前回押されていなかった
		if (state == InputState::NoHold)
		{
			return InputState::Pushed;
		}
		//前回押されている or 前回押された
		else if (state == InputState::Pushed ||
				 state == InputState::Held)
		{
			return InputState::Held;
		}
	}
	else
	{
		//前回押されている or 前回押された
		if (state == InputState::Pushed || 
			state == InputState::Held)
		{
			return InputState::Released;
		}
	}
	return InputState::NoHold;
}

bool InitDirectInput()
{
	/*
		GetModuleHandle => 現在のアプリのインスタンスハンドルを取得できる
	*/
	HRESULT hr = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル
		DIRECTINPUT_VERSION,		// DirectInputのバージョン(固定値)
		IID_IDirectInput8,			// 使用する機能(固定値)
		(void**)&g_InputInterface,	// 作成されたIDirectInput8保存用変数指定
		nullptr						// nullptr固定
	);

	if (FAILED(hr))
	{
		return false;
	}

	/*
		GUID_SysKeyboard => キーボードを使用する
	
		DirectInputはLPDIRECTINPUTDEVICE8一つにつき一つのデバイスを扱える

		例：
			キーボード、マウスを扱う場合
			LPDIRECTINPUTDEVICE8は二つ必要
	*/

	hr = g_InputInterface->CreateDevice(
		GUID_SysKeyboard,			// 使用するデバイス
		&g_InputKeyboardDevice,		// 作成されたIDirectInputDevice8保存用変数指定
		nullptr);					// nullptr固定
	if (FAILED(hr))
	{
		// 失敗
		return false;
	}

	/*
		作成されたデバイスをフォーマット(初期化)する
	*/

	hr = g_InputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	/*
		FindWindow(クラスネーム, ウィンドウネーム)

			指定して名前のウィンドウを見つけてハンドルを返す
	*/

	// 協調モード設定
	hr = g_InputKeyboardDevice->SetCooperativeLevel(
		FindWindow(TEXT("WindowClass"), nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		// 失敗
		return false;
	}
	// 制御開始
	hr = g_InputKeyboardDevice->Acquire();
	if (FAILED(hr))
	{
		// 失敗
		return false;
	}

	// 成功
	return true;
}

void UpdateDirectInput()
{
	//キーボード情報...
	BYTE key_states[256];
	HRESULT hr;

	/*
		関数成功で第二引数にキーボード情報が保存される

		256 => DirectInputによるキーボードデバイスの情報の数
	*/

	hr = g_InputKeyboardDevice->GetDeviceState(
		256,						//デバイス情報のサイズ
		key_states);				//デバイス情報格納用変数指定
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < 256; i++)
		{
			bool is_push = false;
			if (key_states[i] & 0x80)
			{
				is_push = true;
			}
			//キーの状態を更新する
			g_KeyStates[i] = UpdateInputState(is_push, g_KeyStates[i]);
		}
		if (key_states[DIK_A] & 0x80)
		{
			//Aが押されてる
		}

		if (key_states[DIK_B] & 0x80)
		{
			//Bが押されてる
		}
	}
}

void ReleaseDirectInput()
{
	if (g_InputKeyboardDevice != nullptr)
	{
		//強制停止 <= Input系のデバイスでは必須
		g_InputKeyboardDevice->Unacquire();

		//デバイスを解放
		g_InputKeyboardDevice->Release();
		g_InputKeyboardDevice = nullptr;
	}

	if (g_InputInterface != nullptr)
	{
		//インターフェイスを開放
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

bool IsKeyHeld(int key)
{
	if (key < 0 || key >= 256)
	{
		return false;
	}
	return g_KeyStates[key] == InputState::Held ? true : false;
}

bool IsKeyPushed(int key)
{
	if (key < 0 || key >= 256)
	{
		return false;
	}

	if (g_KeyStates[key] == InputState::Pushed)
	{
		return true;
	}

	return false;
}

bool IsKeyReleased(int key)
{
	if (key < 0 || key >= 256)
	{
		return false;
	}

	if (g_KeyStates[key] == InputState::Released)
	{
		return true;
	}

	return false;
}