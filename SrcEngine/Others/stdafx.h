#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <SpMath.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <map>

#include <vector>
#include <string>
#include <array>
#include <wrl.h>
#include <stdexcept>

#ifdef _DEBUG
#pragma comment(lib, "zlibstaticd.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp/contrib/zlibstatic.lib")
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif