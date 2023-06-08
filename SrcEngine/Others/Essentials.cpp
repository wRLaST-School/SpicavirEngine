#include "Essentials.h"

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void* __cdecl operator new[](size_t size, const char* name, int32_t flags, unsigned debugFlags, const char* file, int32_t line)
{
	line;
	file;
	debugFlags;
	flags;
	name;
	return new uint8_t[size];
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int32_t flags, unsigned debugFlags, const char* file, int32_t line)
{
	line;
	file;
	debugFlags;
	flags;
	pName;
	alignment;
	alignmentOffset;
	return malloc(size);
}
