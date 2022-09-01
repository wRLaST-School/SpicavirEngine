#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

enum class Button {
	Up = 0x0001,
	Down = 0x0002,
	Left = 0x0004,
	Right = 0x0008,
	Start = 0x0010,
	Back = 0x0020,
	LStickButton = 0x0040,
	RStickButton = 0x0080,
	L = 0x0100,
	R = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
};

enum class Trigger {
	Left = 0,
	Right = 1,
};

namespace Input {
	class Key
	{
	public:
		static void Init();

		static void Update();

		static void Close();

		//キーが押されているか
		static bool Down(int key);
		//キーが離された瞬間か
		static bool Released(int key);
		//キーが押された瞬間か
		static bool Triggered(int key);

		static Key* GetInstance();
	private:
		IDirectInput8* dinput = nullptr;
		IDirectInputDevice8* devkeyboard = nullptr;

		BYTE keys[256] = {};
		BYTE prevKeys[256] = {};
	};

	class Pad
	{
	public:
		static void Init();

		static void Update();

		static void Close();

		//ボタンが押されているか
		static bool Down(Button button);
		//ボタンが離された瞬間か
		static bool Released(Button button);
		//ボタンが押された瞬間か
		static bool Triggered(Button button);

		static bool DownAny();

		//ボタンが押されているか
		static bool Down(Trigger side);
		//ボタンが離された瞬間か
		static bool Released(Trigger side);
		//ボタンが押された瞬間か
		static bool Triggered(Trigger side);

		static Pad* GetInstance();

		struct Stick
		{
			float x;
			float y;
		};

		//左スティックの移動量を-1000から1000で返す
		static Stick GetLStick();
		//右スティックの移動量を-1000から1000で返す
		static Stick GetRStick();

		//デッドゾーンの範囲を設定(0から1000)
		static void SetDeadZone(float range);
		static float GetDeadZone();

	private:
		XINPUT_STATE padState;
		XINPUT_STATE lastPadState;

		bool triggerState[2] = {false, false};
		bool lastTriggerState[2] = {false, false};

		float deadZone = 50;

		int gamepadIndex = 1;
	};
}