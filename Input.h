#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

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

	};

}