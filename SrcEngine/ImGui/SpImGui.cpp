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

void SpImGui::Init()
{
	ImGui::CreateContext();

	SpImguiCustom::StyleColorsDarkBlossom();

	ImGui_ImplWin32_Init(GetSpWindow()->hwnd);

	InitDirectXForImGui();

	ImGui_ImplDX12_Init(GetSpDX()->dev.Get(), SpSwapChainManager::BB_NUM, DXGI_FORMAT_R8G8B8A8_UNORM,
		SpTextureManager::GetInstance().srvHeap.Get(),
		SpTextureManager::GetCPUDescHandle("imgui_srv"),
		SpTextureManager::GetGPUDescHandle("imgui_srv"));

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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