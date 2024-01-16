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

	io.Fonts->AddFontFromFileTTF("Resources/fonts/rounded-x-mgenplus-1m-bold.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
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
	ImGuizmo::BeginFrame();

	for (auto& c : sCommands) {
		c();
	}

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetSpDX()->cmdList.Get());

	sCommands.clear();
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

bool SpImGui::DoubleClickImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int32_t frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding,  bg_col, tint_col);

	return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
}

bool SpImGui::DoubleClickImageButton2(ImTextureID user_texture_id, const ImVec2& size, std::function<void(void)> singleClickEvent, const ImVec2& uv0, const ImVec2& uv1, int32_t frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	if (ImGui::ImageButton("buttonTag", user_texture_id, size, uv0, uv1, bg_col, tint_col)) {
		singleClickEvent();
	};

	frame_padding;

	return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
}

bool SpImGui::InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
	flags |= ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = str;
	cb_user_data.ChainCallback = callback;
	cb_user_data.ChainCallbackUserData = user_data;
	return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

int SpImGui::InputTextCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
		std::string* str = user_data->Str;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
	}
	else if (user_data->ChainCallback)
	{
		// Forward to user callback, if any
		data->UserData = user_data->ChainCallbackUserData;
		return user_data->ChainCallback(data);
	}
	return 0;
}


std::list<std::function<void(void)>> SpImGui::sCommands;