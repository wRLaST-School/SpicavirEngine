#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

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

		static Pad* GetInstance();

		struct Stick
		{
			float x;
			float y;
		};

		//スティックの移動量を-100から100で返す
		static Stick GetLStick();
		static Stick GetRStick();

		static void SetDeadZone(Stick range);

	private:
		XINPUT_STATE padState;
		XINPUT_STATE lastPadState;
	};

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
		Y = 0x8000
	};

}