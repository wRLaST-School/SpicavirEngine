#pragma once
#include <SpImGui.h>
class DockPanel
{
public:
	static void EnableScreenDock();

	static void DrawViewPort();

	static ImGuiWindow* GetViewPortWindow();
private:
	inline static ImGuiWindow* viewPortWindow;

};

