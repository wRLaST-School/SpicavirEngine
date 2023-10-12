#pragma once
#include <functional>
#include <imgui.h>
class SpImGui
{
public:
	static void Init();

	static void InitDirectXForImGui();

	static void Command(std::function<void(void)> command);

	static void Draw();

	static void EnableScreenDock();

	static void EndFrame();

	static void Shutdown();

private:
	static std::list<std::function<void(void)>> sCommands;

	//ImGui ItemのWrapper関数群
public:
	static bool DoubleClickButton(const char* label, const ImVec2& size = ImVec2(0, 0));

	static bool DoubleClickImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
};

