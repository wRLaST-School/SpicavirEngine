#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void InitInput();

void UpdateInput();

void CloseInput();

//キーが押されているか
bool KeyDown(int key);
//キーが離された瞬間か
bool KeyReleased(int key);
//キーが押された瞬間か
bool KeyTriggered(int key);