#include "stdafx.h"
#include "SpImGui.h"

#pragma warning(push, 1)
#include <ImGuiStyle.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#pragma warning(pop)

#include <SpWindow.h>

#include <SpSwapChainManager.h>
#include <SpTextureManager.h>
#include <SpDirectX.h>
#include <GameManager.h>

void SpImGui::Init()
{
	ImGui::CreateContext();

	SpImguiCustom::StyleColorsDarkBlossom();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplWin32_Init(GetSpWindow()->hwnd);

	InitDirectXForImGui();

	ImGui_ImplDX12_Init(GetSpDX()->dev.Get(), SpSwapChainManager::BB_NUM, DXGI_FORMAT_R8G8B8A8_UNORM,
		SpTextureManager::GetInstance().srvHeap.Get(),
		SpTextureManager::GetCPUDescHandle("imgui_srv"),
		SpTextureManager::GetGPUDescHandle("imgui_srv"));

	io.Fonts->AddFontFromFileTTF("Resources/fonts/rounded-x-mgenplus-1m-bold.ttf", 18);
}

void SpImGui::InitDirectXForImGui()
{
	SpTextureManager::CreatePlainSRV("imgui_srv");
	SpTextureManager::AddMasterTextureKey("imgui_srv");
}

void SpImGui::Command(std::function<void(void)> command)
{
	sCommands.push_back(command);
}

void SpImGui::Draw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& c : sCommands) {
		c();
	}

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetSpDX()->cmdList.Get());

	sCommands.clear();
}

void SpImGui::EnableScreenDock()
{
	Command([&] {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		if (!GameManager::sShowDebug) dockspace_flags |= ImGuiDockNodeFlags_::ImGuiDockNodeFlags_KeepAliveOnly;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		static bool open = true;

		ImGui::Begin("Editor", &open, window_flags);
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		ImGui::End();

		ImGui::PopStyleVar(3);
	});
}

void SpImGui::EndFrame()
{
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(nullptr, (void*)GetSpDX()->cmdList.Get());
	}
}

void SpImGui::Shutdown()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool SpImGui::DoubleClickButton(const char* label, const ImVec2& size_arg)
{
	ImGui::Button(label, size_arg);

	return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
}

bool SpImGui::DoubleClickImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding,  bg_col, tint_col);

	return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
}


std::list<std::function<void(void)>> SpImGui::sCommands;