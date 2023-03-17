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
	bool isClientActive = true;
private:
};

/// <summary>
///	SpWindowリストから指定IDのSpWindowを探して返す。
/// IDを指定しない場合はSetDefaultWindowIDで指定したIDを使う。
/// </summary>
/// <param name="ID">string型のID</param>
/// <returns>成功:SpWindowへのポインタ,失敗:nullptr</returns>
SpWindow* GetSpWindow(string ID);
SpWindow* GetSpWindow();
extern map<string, SpWindow> wWindowList;
void RegisterSpWindow(SpWindow spwnd, string ID);
void SetDefaultWindowID(string ID);

void CloseAllSpWindow();
