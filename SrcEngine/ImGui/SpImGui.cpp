#include "stdafx.h"
#include "SpImGui.h"
#include <ImGuiStyle.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <SpWindow.h>

#include <SpSwapChainManager.h>
#include <SpTextureManager.h>
#include <SpDirectX.h>

void SpImGui::Init()
{
	ImGui::CreateContext();

	SpImguiCustom::StyleColorsSpicavirSakura();

	ImGui_ImplWin32_Init(GetSpWindow()->hwnd);

	InitDirectXForImGui();

	ImGui_ImplDX12_Init(GetWDX()->dev.Get(), SpSwapChainManager::bbNum, DXGI_FORMAT_R16G16B16A16_FLOAT,
		SpTextureManager::GetInstance().srvHeap.Get(),
		SpTextureManager::GetCPUDescHandle("imgui_srv"),
		SpTextureManager::GetGPUDescHandle("imgui_srv"));

	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontFromFileTTF("SrcImgui\\imgui\\ImGuiLibs\\Resources\\UDEVGothicLG-Bold.ttf", 14);
}

void SpImGui::InitDirectXForImGui()
{
	SpTextureManager::CreatePlainSRV("imgui_srv");
	SpTextureManager::AddMasterTextureKey("imgui_srv_");
}

void SpImGui::Command(std::function<void(void)> command)
{
	commands.push_back(command);
}

void SpImGui::Draw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& c : commands) {
		c();
	}

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetWDX()->cmdList.Get());

	commands.clear();
}

void SpImGui::Shutdown()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


std::list<std::function<void(void)>> SpImGui::commands;