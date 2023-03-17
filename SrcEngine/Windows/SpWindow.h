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
///	SpWindow���X�g����w��ID��SpWindow��T���ĕԂ��B
/// ID���w�肵�Ȃ��ꍇ��SetDefaultWindowID�Ŏw�肵��ID���g���B
/// </summary>
/// <param name="ID">string�^��ID</param>
/// <returns>����:SpWindow�ւ̃|�C���^,���s:nullptr</returns>
SpWindow* GetSpWindow(string ID);
SpWindow* GetSpWindow();
extern map<string, SpWindow> wWindowList;
void RegisterSpWindow(SpWindow spwnd, string ID);
void SetDefaultWindowID(string ID);

void CloseAllSpWindow();
