#include "stdafx.h"
#include "ResourceWindow.h"
#include <SpImGui.h>
#include <SpTextureManager.h>
#include <SpDirectX.h>
#include <filesystem>
#include <regex>

void ResourceWindow::Draw()
{
    SpImGui::Command([&] {
        ImGui::Begin("Resource Window");

		//テクスチャのAssetBrowserからDDによる読み込み
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AST_BROWSER_ITEM");

			if (payload) {
				const char* filePathCharP = reinterpret_cast<const char*>(payload->Data);
				std::string filePath = filePathCharP;

				filePath = filePath.substr(0, static_cast<rsize_t>(payload->DataSize) / sizeof(char));

				auto ditr = std::filesystem::directory_entry(filePath);

				//画像ファイルの場合
				std::regex re(".(png|PNG|jpg|JPG|dds|DDS)");
				std::string ext = ditr.path().extension().string();
				if (std::regex_match(ext, re))
				{
					SpTextureManager::LoadTexture(filePath, ditr.path().filename().string());
				}

			}

			ImGui::EndDragDropTarget();
		}

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

		SpTextureManager& tmIns = SpTextureManager::GetInstance();

		tmIns.textureMap_.Access([&](auto& map) {
			for (auto& c : map) {
				D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;
				heapHandle = tmIns.srvHeap->GetGPUDescriptorHandleForHeapStart();
				heapHandle.ptr += GetSpDX()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)* c.second;
				if (SpImGui::DoubleClickImageButton((ImTextureID)heapHandle.ptr, {thumbnailSize, thumbnailSize}))
				{
					//ダブルクリック時の処理
				};

				if (ImGui::BeginDragDropSource())
				{
					const char* texKey = c.first.c_str();
					ImGui::SetDragDropPayload("RES_WINDOW_ITEM", texKey, strlen(texKey) * sizeof(char), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}

				ImGui::Text(c.first.c_str());

				ImGui::NextColumn();
			}
		});

		ImGui::Columns(1);

		//サムネイル画像サイズ調整用スライダー
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.f, 512.f);

        ImGui::End();
    });
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