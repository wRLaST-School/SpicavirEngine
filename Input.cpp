#include "Input.h"
#include "Essentials.h"
#include "wWindow.h"

using namespace Input;

void Key::Init()
{
	Key* instance = GetInstance();
	HRESULT rr = DirectInput8Create(GetwWindow()->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&instance->dinput, nullptr);

	instance->dinput->CreateDevice(GUID_SysKeyboard, &instance->devkeyboard, NULL);

	instance->devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	instance->devkeyboard->SetCooperativeLevel(GetwWindow()->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Key::Update()
{
	Key* instance = GetInstance();
	for (size_t i = 0; i < 256; i++)
	{
		instance->prevKeys[i] = instance->keys[i];
	}

	instance->devkeyboard->Acquire();

	instance->devkeyboard->GetDeviceState(sizeof(instance->keys), instance->keys);
}

void Key::Close()
{
	Key* instance = GetInstance();
	delete instance->dinput;
	delete instance->devkeyboard;
}

bool Key::Down(int key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)GetInstance()->keys[key];
}

bool Key::Released(int key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)(!GetInstance()->keys[key] && GetInstance()->prevKeys[key]);
}

bool Key::Triggered(int key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)(GetInstance()->keys[key] && !GetInstance()->prevKeys[key]);
}

Key* Key::GetInstance()
{
	static Key obj;
	return &obj;
}

void Input::Pad::Init()
{
	XInputGetState(0, &GetInstance()->padState);
}

void Input::Pad::Update()
{
	
}

void Input::Pad::Close()
{
	//XInputEnable(false);
}

Pad* Input::Pad::GetInstance()
{
	static Pad obj;
	return &obj;
}

Input::Pad::Stick Input::Pad::GetLStick()
{
	short lx = GetInstance()->padState.Gamepad.sThumbLX;
	short ly = GetInstance()->padState.Gamepad.sThumbLY;
	return Stick{ (float)lx / 32.767f, (float)ly / 32.767f };
}

Input::Pad::Stick Input::Pad::GetRStick()
{
	short rx = GetInstance()->padState.Gamepad.sThumbRX;
	short ry = GetInstance()->padState.Gamepad.sThumbRY;
	return Stick{ (float)rx / 32.767f, (float)ry / 32.767f };
}
