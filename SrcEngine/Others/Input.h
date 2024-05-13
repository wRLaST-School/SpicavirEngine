#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include <Xinput.h>

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

enum class Click {
	Left = 0,
	Right = 1,
	Middle = 2,
	MB4 = 3
};

enum class Trigger {
	Left = 0,
	Right = 1,
};

namespace Input {
	class DLLExport Key
	{
	public:
		static void Init();

		static void Update();

		static void Close();

		//キーが押されているか
		static bool Down(int32_t key);
		//キーが離された瞬間か
		static bool Released(int32_t key);
		//キーが押された瞬間か
		static bool Triggered(int32_t key);

		static Key* GetInstance();
	private:
		IDirectInput8* dinput_ = nullptr;
		IDirectInputDevice8* devkeyboard_ = nullptr;

		BYTE keys_[256] = {};
		BYTE prevKeys_[256] = {};

		friend IDirectInput8* GetDInput();
	};

	class DLLExport Mouse {
	public:

		static void Init();

		static void Update();

		static void Close();

		static bool Down(const Click& b);

		static bool Triggered(const Click& b);

		static bool Released(const Click& b);

		//マウスの移動量
		static Float2 GetVel();

		//マウスの座標
		static Float2 GetPos();

		static Mouse* GetInstance();

	private:
		IDirectInputDevice8* devmouse_ = nullptr;

		Float2 cursor_;

		DIMOUSESTATE state_;
		DIMOUSESTATE prevState_;
	};

	class DLLExport Pad
	{
	public:
		static void Init();

		static void Update();

		static void Close();

		//ボタンが押されているか
		static bool Down(const Button& button);
		//ボタンが離された瞬間か
		static bool Released(const Button& button);
		//ボタンが押された瞬間か
		static bool Triggered(const Button& button);

		//ボタンが押されているか
		static bool Down(const Trigger& side);
		//ボタンが離された瞬間か
		static bool Released(const Trigger& side);
		//ボタンが押された瞬間か
		static bool Triggered(const Trigger& side);

		static Pad* GetInstance();

		struct Stick
		{
			float x;
			float y;
			operator Vec2() { return Vec2(x, y); }; // vec2
		};

		//左スティックの移動量を-1から1で返す
		static Stick GetLStick();
		//右スティックの移動量を-1から1で返す
		static Stick GetRStick();

		//デッドゾーンの範囲を設定(0から1000)
		static void SetDeadZone(float range);
		static float GetDeadZone();

		int32_t gamepadIndex = 0;
	private:
		XINPUT_STATE padState_;
		XINPUT_STATE lastPadState_;

		bool triggerState_[2] = { false, false };
		bool lastTriggerState_[2] = { false, false };

		float deadZone_ = 50;
	};
}