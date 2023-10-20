#include "stdafx.h"
#include "ResourceWindow.h"
#include <SpImGui.h>
#include <SpTextureManager.h>
#include <SpDirectX.h>

void ResourceWindow::Draw()
{
    SpImGui::Command([&] {
        ImGui::Begin("Resource Window");

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
