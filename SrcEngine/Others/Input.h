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
	class Key
	{
	public:
		static void Init();

		static void Update();

		static void Close();

		//�L�[��������Ă��邩
		static bool Down(int32_t key);
		//�L�[�������ꂽ�u�Ԃ�
		static bool Released(int32_t key);
		//�L�[�������ꂽ�u�Ԃ�
		static bool Triggered(int32_t key);

		static Key* GetInstance();
	private:
		IDirectInput8* dinput = nullptr;
		IDirectInputDevice8* devkeyboard = nullptr;

		BYTE keys[256] = {};
		BYTE prevKeys[256] = {};

		friend IDirectInput8* GetDInput();
	};

	class Mouse {
	public:

		static void Init();

		static void Update();

		static void Close();

		static bool Down(Click b);

		static bool Triggered(Click b);

		static bool Released(Click b);

		//�}�E�X�̈ړ���
		static Float2 GetVel();

		//�}�E�X�̍��W
		static Float2 GetPos();

		static Mouse* GetInstance();

	private:
		IDirectInputDevice8* devmouse = nullptr;

		Float2 cursor;

		DIMOUSESTATE state;
		DIMOUSESTATE prevState;
	};

	class Pad
	{
	public:
		static void Init();

		static void Update();

		static void Close();

		//�{�^����������Ă��邩
		static bool Down(Button button);
		//�{�^���������ꂽ�u�Ԃ�
		static bool Released(Button button);
		//�{�^���������ꂽ�u�Ԃ�
		static bool Triggered(Button button);

		//�{�^����������Ă��邩
		static bool Down(Trigger side);
		//�{�^���������ꂽ�u�Ԃ�
		static bool Released(Trigger side);
		//�{�^���������ꂽ�u�Ԃ�
		static bool Triggered(Trigger side);

		static Pad* GetInstance();

		struct Stick
		{
			float x;
			float y;
			operator Vec2() { return Vec2(x, y); }; // vec2
		};

		//���X�e�B�b�N�̈ړ��ʂ�-1����1�ŕԂ�
		static Stick GetLStick();
		//�E�X�e�B�b�N�̈ړ��ʂ�-1����1�ŕԂ�
		static Stick GetRStick();

		//�f�b�h�]�[���͈̔͂�ݒ�(0����1000)
		static void SetDeadZone(float range);
		static float GetDeadZone();

		int32_t gamepadIndex = 0;
	private:
		XINPUT_STATE padState;
		XINPUT_STATE lastPadState;

		bool triggerState[2] = { false, false };
		bool lastTriggerState[2] = { false, false };

		float deadZone = 50;
	};
}