#include "SpWindow.h"
#include <SpSwapChainManager.h>
#include <SpDepth.h>
#include <Sprite.h>
#include <imgui_impl_win32.h>
static string defWndID = "Default";
map<string, SpWindow> wWindowList;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

SpWindow* GetSpWindow(string ID)
{
    auto res = wWindowList.find(ID);
    return res != wWindowList.end() ? &res->second : nullptr;
}

SpWindow* GetSpWindow()
{
    return GetSpWindow(defWndID);
}

void RegisterSpWindow(SpWindow wwnd, string ID)
{
    wWindowList[ID] = wwnd;
}

void SetDefaultWindowID(string ID)
{
    defWndID = ID;
}

void CloseAllSpWindow()
{
	for (auto itr = wWindowList.begin(); itr != wWindowList.end(); itr++)
	{
		itr->second.Close();
	}
}

void SpWindow::Create(LPCWSTR title, int windowWidth, int windowHeight) {
	width = windowWidth;
	height = windowHeight;

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = WndProc;
	w.lpszClassName = title;
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w);

	//ウィンドウサイズのRECT
	RECT wrc = { 0, 0, width, height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//サイズ補正を自動で

	hwnd = CreateWindow(w.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr, //親ウィンドウハンドル
		nullptr, //メニューハンドル
		w.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);
}

void SpWindow::Close()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool SpWindow::ProcessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//imgui wndproc
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) return true;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		{
			if (LOWORD(lParam) && HIWORD(lParam))
			{
				SpWindow* spw = GetSpWindow();
				if (spw != nullptr)
				{
					spw->width = LOWORD(lParam);
					spw->height = HIWORD(lParam);
					Sprite::InitCommon();
				}

				if (GetSCM()->swapchain.Get() != nullptr)
				{
					GetSCM()->ResizeAllBuffers();
				}

				if (GetWDepth()->depthBuffer != nullptr)
				{
					GetWDepth()->Resize();
				}

				SpTextureManager::ResizeScreenTextures();

				return 0;
			}
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}