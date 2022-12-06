#include "stdafx.h"
#include "SpImGui.h"
#include <imgui.h>
#include <ImGuiStyle.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <SpWindow.h>

#include <SpSwapChainManager.h>
#include <SpTextureManager.h>

void SpImGui::Init()
{
	ImGui::CreateContext();

	SpImguiCustom::StyleColorsSpicavirSakura();

	ImGui_ImplWin32_Init(GetSpWindow()->hwnd);

	ImGui_ImplDX12_Init( , SpSwapChainManager::bbNum, );
}

void SpImGui::InitDirectXForImGui()
{
	SpTextureManager::AddMasterTextureKey("imgui_srv_");
}
