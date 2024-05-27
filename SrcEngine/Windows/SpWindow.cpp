#include "SpWindow.h"
#include <SpSwapChainManager.h>
#include <SpDepth.h>
#include <Sprite.h>
#include <AssetBrowser.h>

#pragma warning(push, 0)
#include <imgui_impl_win32.h>
#pragma warning(pop)

static std::string defWndID = "Default";
std::map<std::string, SpWindow> wWindowList;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

SpWindow* GetSpWindow(const std::string& ID)
{
    auto res = wWindowList.find(ID);
    return res != wWindowList.end() ? &res->second : nullptr;
}

SpWindow* GetSpWindow()
{
    return GetSpWindow(defWndID);
}

void RegisterSpWindow(SpWindow wwnd, const std::string& ID)
{
    wWindowList[ID] = wwnd;
}

void SetDefaultWindowID(const std::string& ID)
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

void SpWindow::Create(LPCWSTR title, int32_t windowWidth, int32_t windowHeight) {
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

	DragAcceptFiles(hwnd, TRUE);

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
	case WM_CREATE:
		//SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_BORDER); //ボーダーレスならコメントアウト解除
		return DefWindowProc(hwnd, msg, wParam, lParam);
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

				if (GetSpDepth()->depthBuffer != nullptr)
				{
					GetSpDepth()->Resize();
				}

				SpTextureManager::ResizeScreenTextures();

				return 0;
			}
		}
	case WM_DROPFILES:
		{
			auto hdrop = (HDROP)wParam;
			//ドロップされたFile数
			int32_t num = DragQueryFile(hdrop, (UINT)(-1), NULL, 0);

			TCHAR filename[255];

			for (size_t i = 0; i < num; i++) {
				DragQueryFile(hdrop, (UINT)i, filename, sizeof(filename) / sizeof(TCHAR));

				AssetBrowser::GetInstance()->CopyFileByDD(filename);
			}

			DragFinish(hdrop);
			return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}