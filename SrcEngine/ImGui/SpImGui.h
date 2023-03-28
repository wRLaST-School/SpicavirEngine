#pragma once
#include <functional>
#include <imgui.h>
class SpImGui
{
public:
	static void Init();

	static void InitDirectXForImGui();

	static void Command(std::function<void(void)> command);

	static void Window(string windowTitle, std::function<void(void)> command);

	static void Draw();

	static void Shutdown();

private:
	static std::list<std::function<void(void)>> commands;
};

