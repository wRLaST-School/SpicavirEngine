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
		static bool Down(int key);
		//ボタンが離された瞬間か
		static bool Released(int key);
		//ボタンが押された瞬間か
		static bool Triggered(int key);

		static Pad* GetInstance();

		static class LStick {

		};

		static class RStick {

		};

	private:
		XINPUT_STATE padState;
	};

}