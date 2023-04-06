#include "Input.h"
#include "Essentials.h"
#include "SpWindow.h"
#include "Util.h"
#include <ToolManager.h>

#ifdef _DEBUG
#include <SpImGui.h>
#endif
#include <imgui.h>

using namespace Input;

void Key::Init()
{
	Key* instance = GetInstance();
	DirectInput8Create(GetSpWindow()->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&instance->dinput, nullptr);

	instance->dinput->CreateDevice(GUID_SysKeyboard, &instance->devkeyboard, NULL);

	instance->devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	instance->devkeyboard->SetCooperativeLevel(GetSpWindow()->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
	instance->dinput->Release();
	instance->devkeyboard->Release();
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

	return Stick{ (float)lx / 32767.f, (float)ly / 32767.f };
}

Input::Pad::Stick Input::Pad::GetRStick()
{
	Input::Pad* instance = GetInstance();
	short rx = Util::Clamp(instance->padState.Gamepad.sThumbRX, (short)-32767, (short)32767);
	short ry = Util::Clamp(instance->padState.Gamepad.sThumbRY, (short)-32767, (short)32767);

	if (Vec2(rx, ry).GetSquaredLength() <= (instance->deadZone * 32.767f) * (instance->deadZone * 32.767f))
		return Stick{ 0,0 };

	return Stick{ (float)rx / 32767.f, (float)ry / 32767.f };
}

void Input::Pad::SetDeadZone(float range)
{
	GetInstance()->deadZone = Util::Clamp(range, 0.0f, 100.0f);
}

float Input::Pad::GetDeadZone()
{
	return GetInstance()->deadZone;
}

void Input::Mouse::Init()
{
	Mouse* instance = GetInstance();
	GetDInput()->CreateDevice(GUID_SysMouse, &instance->devmouse, NULL);
	instance->devmouse->SetDataFormat(&c_dfDIMouse);

	instance->devmouse->SetCooperativeLevel(GetSpWindow()->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void Input::Mouse::Update()
{
	if (ImGui::GetIO().WantCaptureMouse) return;
	Mouse* ins = GetInstance();
	ins->prevState = ins->state;

	ins->devmouse->Acquire();
	ins->devmouse->Poll();
	ins->devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &ins->state);

	POINT pos;
	GetCursorPos(&pos);

	ScreenToClient(GetSpWindow()->hwnd, &pos);

	ins->cursor = { (float)pos.x, (float)pos.y };
}

void Input::Mouse::Close()
{
	Mouse* ins = GetInstance();
	ins->devmouse->Release();
}

bool Input::Mouse::Down(Click b)
{
	return GetInstance()->state.rgbButtons[(int)b] & (0x80) && ToolManager::isMouseActive;
}

bool Input::Mouse::Triggered(Click b)
{
	return (!(GetInstance()->prevState.rgbButtons[(int)b] & (0x80))) && (GetInstance()->state.rgbButtons[(int)b] & (0x80)) && ToolManager::isMouseActive;
}

bool Input::Mouse::Released(Click b)
{
	return (!(GetInstance()->state.rgbButtons[(int)b] & (0x80))) && (GetInstance()->prevState.rgbButtons[(int)b] & (0x80));
}

Float2 Input::Mouse::GetVel()
{
	return { (float)GetInstance()->state.lX , (float)GetInstance()->state.lY };
}

Float2 Input::Mouse::GetPos()
{
	return GetInstance()->cursor;
}

Mouse* Input::Mouse::GetInstance()
{
	static Mouse obj;
	return &obj;
}

IDirectInput8* Input::GetDInput()
{
	return Key::GetInstance()->dinput;
}
