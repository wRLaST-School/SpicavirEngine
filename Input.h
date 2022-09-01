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

		//�L�[��������Ă��邩
		static bool Down(int key);
		//�L�[�������ꂽ�u�Ԃ�
		static bool Released(int key);
		//�L�[�������ꂽ�u�Ԃ�
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

		//�{�^����������Ă��邩
		static bool Down(Button button);
		//�{�^���������ꂽ�u�Ԃ�
		static bool Released(Button button);
		//�{�^���������ꂽ�u�Ԃ�
		static bool Triggered(Button button);

		static bool DownAny();

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
		};

		//���X�e�B�b�N�̈ړ��ʂ�-1000����1000�ŕԂ�
		static Stick GetLStick();
		//�E�X�e�B�b�N�̈ړ��ʂ�-1000����1000�ŕԂ�
		static Stick GetRStick();

		//�f�b�h�]�[���͈̔͂�ݒ�(0����1000)
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