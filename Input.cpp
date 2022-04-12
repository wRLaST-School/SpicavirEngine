#include "Input.h"
#include "Essentials.h"
#include "wWindow.h"

IDirectInput8* dinput = nullptr;
IDirectInputDevice8* devkeyboard = nullptr;

static BYTE keys[256] = {};
static BYTE prevKeys[256] = {};

void InitInput()
{
	HRESULT rr = DirectInput8Create(GetwWindow()->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	devkeyboard->SetCooperativeLevel(GetwWindow()->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void UpdateInput()
{
	for (size_t i = 0; i < 256; i++)
	{
		prevKeys[i] = keys[i];
	}

	devkeyboard->Acquire();

	devkeyboard->GetDeviceState(sizeof(keys), keys);
}

void CloseInput()
{
	delete dinput;
	delete devkeyboard;
}

bool KeyDown(int key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)keys[key];
}

bool KeyReleased(int key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)(!keys[key] && prevKeys[key]);
}

bool KeyTriggered(int key)
{
	if (key >= 256 || key < 0) return false;
	return (bool)(keys[key] && !prevKeys[key]);
}