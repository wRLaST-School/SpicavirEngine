#include "wWindow.h"

static string defWndID = "Default";
map<string, wWindow> wWindowList;
wWindow* GetwWindow(string ID)
{
    auto res = wWindowList.find(ID);
    return res != wWindowList.end() ? &res->second : nullptr;
}

wWindow* GetwWindow()
{
    return GetwWindow(defWndID);
}

void RegisterwWindow(wWindow wwnd, string ID)
{
    wWindowList[ID] = wwnd;
}

void SetDefaultWindowID(string ID)
{
    defWndID = ID;
}

void CloseAllwWindow()
{
	for (auto itr = wWindowList.begin(); itr != wWindowList.end(); itr++)
	{
		itr->second.Close();
	}
}

void wWindow::Create(LPCWSTR title, int windowWidth, int windowHeight) {
	width = windowWidth;
	height = windowHeight;

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = title;
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w);

	//�E�B���h�E�T�C�Y��RECT
	RECT wrc = { 0, 0, width, height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�T�C�Y�␳��������

	hwnd = CreateWindow(w.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr, //�e�E�B���h�E�n���h��
		nullptr, //���j���[�n���h��
		w.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);
}

void wWindow::Close()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}
