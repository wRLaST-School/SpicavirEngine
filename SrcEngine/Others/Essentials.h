#pragma once
#include "stdafx.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "EASTL.lib")

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//à»â∫EASTLÇÃïKóvä÷êî
void* __cdecl operator new[](size_t size, const char* name, int32_t flags, unsigned debugFlags, const char* file, int32_t line);

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int32_t flags, unsigned debugFlags, const char* file, int32_t line);
