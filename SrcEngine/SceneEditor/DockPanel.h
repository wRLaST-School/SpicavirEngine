#pragma once
#include <SpImGui.h>
class DockPanel
{
public:
	static void EnableScreenDock();

	static void DrawViewPort();

	static void DrawSaveDialog();

	static void DrawLoadDialog();

	static ImGuiWindow* GetViewPortWindow();
private:
	inline static ImGuiWindow* sViewPortWindow;

	inline static bool showSaveDialog = false;
	inline static bool showLoadDialog = false;
};

