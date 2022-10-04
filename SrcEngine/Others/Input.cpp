#include "Input.h"
#include "Essentials.h"
#include "wWindow.h"
#include "Util.h"

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
	
}

void Input::Pad::Update()
{
	Input::Pad* instance = GetInstance();

	instance->lastPadState = instance->padState;
	instance->lastTriggerState[0] = instance->triggerState[0];
	instance->lastTriggerState[1] = instance->triggerState[1];

	XInputGetState(instance->gamepadIndex, &instance->padState);
	instance->triggerState[0] = instance->padState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	instance->triggerState[1] = instance->padState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

void Input::Pad::Close()
{
	//XInputEnable(false);
}

bool Input::Pad::Down(Button button)
{
	return GetInstance()->padState.Gamepad.wButtons & (UINT)button;
}

bool Input::Pad::Released(Button button)
{
	return !(GetInstance()->padState.Gamepad.wButtons & (UINT)button) && GetInstance()->lastPadState.Gamepad.wButtons & (UINT)button;
}

bool Input::Pad::Triggered(Button button)
{
	return !(GetInstance()->lastPadState.Gamepad.wButtons & (UINT)button) && GetInstance()->padState.Gamepad.wButtons & (UINT)button;
}

bool Input::Pad::Down(Trigger side)
{
	return GetInstance()->triggerState[(int)side];
}

bool Input::Pad::Released(Trigger side)
{
	return !(GetInstance()->triggerState[(int)side]) && GetInstance()->lastTriggerState[(int)side];
}

bool Input::Pad::Triggered(Trigger side)
{
	return GetInstance()->triggerState[(int)side] && !(GetInstance()->lastTriggerState[(int)side]);
}

Pad* Input::Pad::GetInstance()
{
	static Pad obj;
	return &obj;
}

Input::Pad::Stick Input::Pad::GetLStick()
{
	Input::Pad* instance = GetInstance();
	short lx = Util::Clamp(instance->padState.Gamepad.sThumbLX, (short)-32767, (short)32767);
	short ly = Util::Clamp(instance->padState.Gamepad.sThumbLY, (short)-32767, (short)32767);

	if (Vec2(lx, ly).GetSquaredLength() <= (instance->deadZone * 32.767f) * (instance->deadZone * 32.767f))
		return Stick{ 0,0 };

	return Stick{ (float)lx / 32.767f, (float)ly / 32.767f };
}

Input::Pad::Stick Input::Pad::GetRStick()
{
	Input::Pad* instance = GetInstance();
	short rx = Util::Clamp(instance->padState.Gamepad.sThumbRX, (short)-32767, (short)32767);
	short ry = Util::Clamp(instance->padState.Gamepad.sThumbRY, (short)-32767, (short)32767);

	if (Vec2(rx, ry).GetSquaredLength() <= (instance->deadZone * 32.767f) * (instance->deadZone * 32.767f))
		return Stick{ 0,0 };

	return Stick{ (float)rx / 32.767f, (float)ry / 32.767f };
}

void Input::Pad::SetDeadZone(float range)
{
	GetInstance()->deadZone = Util::Clamp(range, 0.0f, 100.0f);
}

float Input::Pad::GetDeadZone()
{
	return GetInstance()->deadZone;
}
