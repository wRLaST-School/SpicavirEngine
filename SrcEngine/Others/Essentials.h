#pragma once
#include "stdafx.h"

#pragma comment(lib, "EASTL.lib")

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//以下EASTLの必要関数
void* __cdecl operator new[](size_t size, const char* name, int32_t flags, unsigned debugFlags, const char* file, int32_t line);

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int32_t flags, unsigned debugFlags, const char* file, int32_t line);
