#pragma once
#include  <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "libs/WrelfyLib.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <vector>
#include <string>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "libs/WrelfyLib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);