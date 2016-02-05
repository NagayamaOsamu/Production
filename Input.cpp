/*===========================================================================================
概　略：ゲームの入力処理を行う関数定義
作成日：6月26日
更新日：1月18日
制作者：永山治夢
=============================================================================================*/

//インクルード
#include "Input.h"

//コンストラクタ
Input::Input()
{
	//keyStateを初期化
	ZeroMemory(keyState,sizeof(keyState));

	
}

//デストラクタ
Input::~Input()
{
	//キーボードのアクセス権を解放
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	//DirectInput解放
	SAFE_RELEASE(pMouseDevice);
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(pDinput);

}


//機能：DirectInputの準備
//引数：ウィンドウハンドル
//戻値：成功か失敗
HRESULT Input::Init(HWND hWnd)
{

	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return E_FAIL;	//作成に失敗したら失敗を知らせる
	}

	//-----------------------キーボードの準備------------------------------------

	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(pDinput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL)))
	{
		return E_FAIL;	//失敗を知らせる
	}

	// デバイスをキーボードに設定
	if (FAILED(pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調レベルの設定(別のウィンドウを開いていてもゲームに反応できるようにするかしないか)
	if (FAILED(pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	//-----------------------マウスの準備------------------------------------

	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(pDinput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL)))
	{
		return E_FAIL;
	}


	// デバイスをマウスに設定
	if (FAILED(pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調レベルの設定
	if (FAILED(pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	return S_OK;	//成功を知らせる
}

//機能：各入力デバイスの状態を取得
//引数：なし
//戻値：成功か失敗か
HRESULT  Input::Update()
{
	//XInputで情報を取得する前に前回の情報を入れる必要があるためその処理の前に書かないといけない
		memcpy(&prevButtonState, &controllerState, sizeof(controllerState));	//前回のコントローラの情報を入れる

	// デバイスのアクセス権を取得する
	HRESULT hr = pKeyDevice->Acquire();

	//コントローラーの状態を取得
	for (int i = 0; i < 4; i++)
	{
		connect[i] =  XInputGetState(i, &controllerState[i]);

	}

	//アクセス権を取得できた　もしくは　アクセス権をそもそも見失ってなかったとき
	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		memcpy(prevKeyState, keyState, sizeof(keyState));	//前回のキーの情報を入れる
	
		//全てのキーの状態を取得
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);	//キーの状態を第2引数（keyStateのアドレス）に渡している
		
	}


	// デバイスのアクセス権を取得する
	hr = pMouseDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));

		//全てのキーの状態を取得
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

		return S_OK;		//成功を知らせる
	}

	return E_FAIL;			//失敗を知らせる
}

//機能：任意のキーが押されているかチェック
//引数：任意のキー
//戻値：任意のキーが押されているかいないか
BOOL Input::IsKeyPush(DWORD keyCode)
{
	//任意のキーが押されていたら
	if (keyState[keyCode] & 0x80)
	{
		return TRUE; //押している
	}
	return FALSE;      //押してない
}

//機能:任意のキーが押されている瞬間かチェック
//引数:任意のキー
//戻値：任意キーが押された瞬間か
BOOL Input::IsKeyTap(DWORD keyCode)
{
	//前回任意のキーが押されていたか
	if (!(prevKeyState[keyCode] & 0x80) && keyState[keyCode] & 0x80)
	{
		return TRUE;	//押している
	}

	return FALSE;	//押されていない
}

//機能：任意のキーが離されたかチェック
//引数：任意のキー
//戻値：離されたかどうか
BOOL Input::IsKeyRelease(DWORD keyCode)
{
	//キーが離されたか
	if (prevKeyState[keyCode] & 0x80 && !(keyState[keyCode] & 0x80))
	{
		return TRUE;	//離された
	}

	return FALSE;		//押している
}

//機能：マウスの任意のボタンが押されているかチェック
//引数：押されたマウスのボタン
//戻値：マウスボタンが押されたかどうか
BOOL Input::IsMousePush(DWORD keyCode)
{
	//何かのマウスボタンが押されたら
	if (mouseState.rgbButtons[keyCode] & 0x80)
	{
		return TRUE;	//押している
	}

	return FALSE;      //押してない
}


//機能：マウスの任意のボタンが押された瞬間かチェック
//引数：押されたマウスのボタン
//戻値：マウスボタンが押されたかどうか
BOOL  Input::IsMouseTap(DWORD keyCode)
{
	//押した瞬間のマウスのボタンと前に押したボタンが違うとき
	if (!(prevMouseState.rgbButtons[keyCode] & 0x80) && (mouseState.rgbButtons[keyCode] & 0x80))
	{
		return TRUE;	//押している
	}

	return FALSE;      //押してない

}

//機能：マウスの任意のボタンがはなされた瞬間かチェック
//引数：押されたマウスのボタン
//戻値：マウスのボタンが離されたかどうか
BOOL  Input::IsMouseRelease(DWORD keyCode)
{
	//押したボタンが既に押されてないとき
	if ((prevMouseState.rgbButtons[keyCode] & 0x80) && !(mouseState.rgbButtons[keyCode] & 0x80))
	{
		return TRUE;	//押している
	}

	return FALSE;      //押してない
}


//機能：コントローラの任意のボタンが押されているか
//引数：buttonCode 任意のボタン
//引数:ID		　 コントローラが何Pか(初期値は1P)
//戻値：押されているかどうか
BOOL Input::IsPadButtonPush(DWORD buttonCode, int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	//ボタンが押されているか
	if (controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE; //押してる
	}
	return FALSE;   //押してない
}

//機能:任意のボタンが押されている瞬間かチェック
//引数：buttonCode 任意のボタン
//引数:ID		　 コントローラが何Pか(初期値は1P)
//戻値:任意ボタンが押された瞬間か
BOOL Input::IsPadButtonTap(DWORD buttonCode, int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	//前回任意のキーが押されていたか
	if (!(prevButtonState[ID].Gamepad.wButtons & buttonCode) && controllerState[ID].Gamepad.wButtons & buttonCode)
	{
		return TRUE;	//押している
	}

	return FALSE;	//押されていない
}

//機能:任意のボタンが離されたかチェック
//引数：buttonCode 任意のボタン
//引数:ID		　 コントローラが何Pか(初期値は1P)
//戻値:任意ボタンが押された瞬間か
BOOL Input::IsPadButtonRelease(DWORD buttonCode, int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return FALSE;
	}

	//前回任意のキーが押されているか
	if ( prevButtonState[ID].Gamepad.wButtons & buttonCode && !(controllerState[ID].Gamepad.wButtons & buttonCode))
	{
		return TRUE;	//離した
	}

	return FALSE;	//離していない
}

//機能：コントローラの左スティック左右の傾きを取得
//引数：コントローラが何Pか
//戻値：左右の傾き
float Input::GetPadLStickX(int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
			return 0.0f;
	}

	//コントローラの左右の傾きを取得（sThumbLXが左スティックの横の傾きになる）
	float value = controllerState[ID].Gamepad.sThumbLX;

	//デッドゾーン(ホコリなどで何もしなくてもスティックを倒していると判断されてしまうのを防ぐ)
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;	//0を返す
	}

	//傾きを返す
	return value / 32767;	//普通に返すと-32768～32767の値になるので計算が面倒になるので最大値で割って-1～1の値にする
}

//機能：コントローラの左スティック上下の傾きを取得
//引数：コントローラが何Pか
//戻値：上下の傾き
float Input::GetPadLStickY(int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return  0.0f;
	}

	//コントローラの上下の傾きを取得（sThumbLYが左スティックの縦の傾きになる）
	float value = controllerState[ID].Gamepad.sThumbLY;

	

	//デッドゾーン(ホコリなどで何もしなくてもスティックを倒していると判断されてしまうのを防ぐ)
	if (value < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return 0.0f;	//0を返す
	}

	//傾きを返す
	return value / 32767;	//普通に返すと-32768～32767の値になるので計算が面倒になるので最大値で割って-1～1の値にする
}

//機能：コントローラの右スティック左右の傾きを取得
//引数：コントローラが何Pか
//戻値：左右の傾き
float Input::GetPadRStickX(int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return 0.0f;
	}

	//コントローラの左右の傾きを取得（
	float value = controllerState[ID].Gamepad.sThumbRX;



	//デッドゾーン(ホコリなどで何もしなくてもスティックを倒していると判断されてしまうのを防ぐ)
	if (value < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && value > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return 0.0f;	//0を返す
	}

	//傾きを返す
	return value / 32767;	//普通に返すと-32768～32767の値になるので計算が面倒になるので最大値で割って-1～1の値にする
}

//機能：コントローラの右スティック上下の傾きを取得
//引数：コントローラが何Pか
//戻値：上下の傾き
float Input::GetPadRStickY(int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return 0.0f;
	}
	//コントローラの上下の傾きを取得（sThumbLYが左スティックの縦の傾きになる）
	float value = controllerState[ID].Gamepad.sThumbRY;



	//デッドゾーン(ホコリなどで何もしなくてもスティックを倒していると判断されてしまうのを防ぐ)
	if (value <  XINPUT_GAMEPAD_TRIGGER_THRESHOLD  && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;	//0を返す
	}

	//傾きを返す
	return value / 32767;	//普通に返すと-32768～32767の値になるので計算が面倒になるので最大値で割って-1～1の値にする
}

//機能：左トリガーの押し込みを取得
//引数：コントローラが何Pか
//戻値：左トリガーの押し込み
float Input::GetPadLTrigger(int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return 0.0f;
	}

	//左トリガーの押し込みの強さを取得
	float value = controllerState[ID].Gamepad.bLeftTrigger;

	//デッドゾーン(ホコリなどで何もしなくてもスティックを倒していると判断されてしまうのを防ぐ)
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD  && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;	//0を返す
	}

	//傾きを返す
	return value / 255;
}

//機能：右トリガーの押し込みを取得
//引数：コントローラが何Pか
//戻値：右トリガーの押し込み
float Input::GetPadRTrigger(int ID)
{
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return 0.0f;
	}

	//左トリガーの押し込みの強さを取得
	float value = controllerState[ID].Gamepad.bRightTrigger;

	//デッドゾーン(ホコリなどで何もしなくてもスティックを倒していると判断されてしまうのを防ぐ)
	if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD  && value > -XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return 0.0f;	//0を返す
	}

	//傾きを返す
	return value / 255;
}

//機能：ゲームパッドを振動させる
//引数：なし
//戻値：なし
void Input::Vibration(float leftSpeed, float rightSpeed,int ID)
{	
	//接続されていないゲームパッドがあったらなにもしない
	if (connect[ID] == ERROR_DEVICE_NOT_CONNECTED)
	{
		return;
	}

	//振動させるのに必要な構造体の準備
	XINPUT_VIBRATION vibration[4];							//構造体作成
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));	//初期化

	//振動の強さ(ここで最大値分だけ掛けているので引数には0.0倍から1倍までの数値にする必要がある)
	vibration[ID].wLeftMotorSpeed  = leftSpeed  * 65535; // 左モーター
	vibration[ID].wRightMotorSpeed = rightSpeed * 65535; // 右モーター

	//振動させる
	XInputSetState(ID, &vibration[ID]);
}


