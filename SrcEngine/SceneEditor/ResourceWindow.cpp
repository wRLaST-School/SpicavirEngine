#include "stdafx.h"
#include "ResourceWindow.h"
#include <SpImGui.h>
#include <SpTextureManager.h>
#include <SpDirectX.h>
#include <filesystem>
#include <regex>
#include <Model.h>

void ResourceWindow::Draw()
{
    SpImGui::Command([&] {
        ImGui::Begin("Resource Window");

		//ウィンドウを覆うダミーを作る
		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::Dummy(ImGui::GetContentRegionAvail());
		
		//ダミーにD&D受け入れを適用
		DragDropTarget();

		//左上にアイテムの位置を戻す
		ImGui::SetCursorPos(pos);

		if (ImGui::BeginTabBar("##ResWindowTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Texture"))
			{	
				AdjustLayout();

				DrawTextureList();

				ImGui::Columns(1);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Model"))
			{
				AdjustLayout();

				DrawModelList();

				ImGui::Columns(1);

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}


		//サムネイル画像サイズ調整用スライダー
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.f, 512.f);

        ImGui::End();
    });
}

void ResourceWindow::DrawTextureList()
{
	SpTextureManager& tmIns = SpTextureManager::GetInstance();

	tmIns.textureMap_.Access([&](auto& map) {
		for (auto& c : map) {

			bool isMasterTexture = false;
			for (auto& mt : tmIns.sMasterTextures)
			{
				if (c.first == mt)
				{
					isMasterTexture = true;
				}
			}

			if (isMasterTexture)
			{
				continue;
			}

			D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;
			heapHandle = tmIns.srvHeap->GetGPUDescriptorHandleForHeapStart();
			heapHandle.ptr += GetSpDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * c.second;
			if (SpImGui::DoubleClickImageButton(reinterpret_cast<ImTextureID>(heapHandle.ptr), { thumbnailSize, thumbnailSize }))
			{
				//ダブルクリック時の処理
			};

			if (ImGui::BeginDragDropSource())
			{
				const char* texKey = c.first.c_str();
				ImGui::SetDragDropPayload("RES_WINDOW_ITEM_TEXTURE", texKey, strlen(texKey) * sizeof(char), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::Text(c.first.c_str());

			ImGui::NextColumn();
		}
		});
}

void ResourceWindow::DrawModelList()
{
	int i = 0;
	ModelManager::sModels.Access([&](auto& map) {
		for (auto& c : map) {
			ImGui::PushID(i++);
			if (SpImGui::DoubleClickImageButton(reinterpret_cast<ImTextureID>(
					SpTextureManager::GetGPUDescHandle("Engine_3DFileIcon").ptr
				),
				{thumbnailSize, thumbnailSize}))
			{
				//ダブルクリック時の処理
			};

			if (ImGui::BeginDragDropSource())
			{
				const char* modelKey = c.first.c_str();
				ImGui::SetDragDropPayload("RES_WINDOW_ITEM_MODEL", modelKey, strlen(modelKey) * sizeof(char), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::Text(c.first.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		});
}

void ResourceWindow::DragDropTarget()
{//テクスチャのAssetBrowserからDDによる読み込み
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AST_BROWSER_ITEM");

		if (payload) {
			const char* filePathCharP = reinterpret_cast<const char*>(payload->Data);
			std::string filePath = filePathCharP;

			filePath = filePath.substr(0, static_cast<rsize_t>(payload->DataSize) / sizeof(char));

			auto ditr = std::filesystem::directory_entry(filePath);

			//画像ファイルの場合
			std::regex re_tex(".(png|PNG|jpg|JPG|dds|DDS)");
			std::string ext = ditr.path().extension().string();
			if (std::regex_match(ext, re_tex))
			{
				SpTextureManager::LoadTexture(filePath, ditr.path().filename().string());
			}

			//モデルファイルの場合
			std::regex re_model(".(obj|OBJ|glb|GLB)");
			if (std::regex_match(ext, re_model))
			{
				ModelManager::Register(filePath, ditr.path().filename().string(), true);
			}

		}

		ImGui::EndDragDropTarget();
	}
}

void ResourceWindow::AdjustLayout()
{
	//レイアウト調整
	float cellSize = thumbnailSize + padding;

	float panelW = ImGui::GetContentRegionAvail().x;

	int columnCount = static_cast<int>(panelW / cellSize);

	//1つも入らないときも1つは入れる(0だとエラーになる)
	if (columnCount < 1)
	{
		columnCount = 1;
	}

	ImGui::Columns(columnCount, 0, false);

}

void ResourceWindow::SDraw()
{
    GetInstance()->Draw();
}

ResourceWindow* ResourceWindow::GetInstance()
{
    static ResourceWindow ins;
    return &ins;
}
