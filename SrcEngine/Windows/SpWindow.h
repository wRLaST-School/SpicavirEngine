#pragma once
#include "Essentials.h"


class SpWindow
{
public:
	void Create(LPCWSTR title, int windowWidth, int windowHeight);
	void Close();

	bool ProcessMessage();

	HWND hwnd;
	WNDCLASSEX  w{};

	int width = 1280;
	int height = 720;
private:
};

/// <summary>
///	wWindowリストから指定IDのwWindowを探して返す。
/// IDを指定しない場合はSetDefaultWindowIDで指定したIDを使う。
/// </summary>
/// <param name="ID">string型のID</param>
/// <returns>成功:wWindowへのポインタ,失敗:nullptr</returns>
SpWindow* GetwWindow(string ID);
SpWindow* GetwWindow();
extern map<string, SpWindow> wWindowList;
void RegisterwWindow(SpWindow wwnd, string ID);
void SetDefaultWindowID(string ID);

void CloseAllwWindow();
