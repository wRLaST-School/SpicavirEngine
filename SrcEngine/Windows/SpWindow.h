#pragma once
#include "Essentials.h"

class SpWindow
{
public:
	void Create(LPCWSTR title, int32_t windowWidth, int32_t windowHeight);
	void Close();

	bool ProcessMessage();

	HWND hwnd;
	WNDCLASSEX  w{};

	int32_t width = 1280;
	int32_t height = 720;
private:
};

/// <summary>
///	SpWindow���X�g����w��ID��SpWindow��T���ĕԂ��B
/// ID���w�肵�Ȃ��ꍇ��SetDefaultWindowID�Ŏw�肵��ID���g���B
/// </summary>
/// <param name="ID">string�^��ID</param>
/// <returns>����:SpWindow�ւ̃|�C���^,���s:nullptr</returns>
SpWindow* GetSpWindow(const std::string& ID);
SpWindow* GetSpWindow();
extern std::map<std::string, SpWindow> wWindowList;
void RegisterSpWindow(SpWindow spwnd, const std::string& ID);
void SetDefaultWindowID(const std::string& ID);

void CloseAllSpWindow();
