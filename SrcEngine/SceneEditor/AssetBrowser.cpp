#include "stdafx.h"
#include "AssetBrowser.h"
#include <SpImGui.h>

#include <fstream>
#include <iostream>

#include <windows.system.h>

#include <SpTextureManager.h>
#include <SceneManager.h>
#include <BTEditorScene.h>

constexpr char* sAssetsDirectory = "Assets";
const static std::filesystem::path sAssetPath = "Assets";

void AssetBrowser::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

	ImGui::Text("%s\\", currentDirectory_.string().c_str());

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

	//何かのフォルダ内にいるなら戻るボタンを表示
	if (currentDirectory_ != std::filesystem::path(sAssetsDirectory))
	{
		if (SpImGui::DoubleClickButton("<-", { thumbnailSize, thumbnailSize }))
		{
			currentDirectory_ = currentDirectory_.parent_path();
		};
		ImGui::Text("^");

		ImGui::NextColumn();
	}

	//現在のフォルダ内のすべてのファイルに対して
	for (auto& ditr : std::filesystem::directory_iterator(currentDirectory_))
	{
		//フォルダなら
		if (ditr.is_directory())
		{
			//const auto& path = ditr.path().string();
			std::string fileName = ditr.path().filename().string();
			if (SpImGui::DoubleClickImageButton((ImTextureID)SpTextureManager::GetGPUDescHandle("Engine_FolderIcon").ptr, { thumbnailSize, thumbnailSize }))
			{
				currentDirectory_ /= ditr.path().filename();
			};
			ImGui::Text(fileName.c_str());

			ImGui::NextColumn();
		}
	}

	//現在のフォルダ内のすべてのファイルに対して
	//フォルダ->ファイルの順でソートするため2周探索している
	for (auto& ditr : std::filesystem::directory_iterator(currentDirectory_))
	{
		//ファイルなら
		if (!ditr.is_directory())
		{
			std::string fileName = ditr.path().filename().string();
			std::string filePath = ditr.path().string();

			ImGui::ImageButton("buttonTag", (ImTextureID)SpTextureManager::GetGPUDescHandle("Engine_FileIcon").ptr, { thumbnailSize, thumbnailSize });

			if (ImGui::IsItemHovered() && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				selectedItemPath = filePath;
			};

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				FileOpenAction(ditr);
			};

			if (ImGui::BeginDragDropSource())
			{
				const char* relPath = selectedItemPath.c_str();
				ImGui::SetDragDropPayload("AST_BROWSER_ITEM", relPath, strlen(relPath) * sizeof(char));
				ImGui::EndDragDropSource();
			}

			ImGui::Text(fileName.c_str());

			ImGui::NextColumn();
		}
	}

	ImGui::Columns(1);

	//サムネイル画像サイズ調整用スライダー
	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.f, 512.f);

	ImGui::End();
}

void AssetBrowser::Draw()
{
	SpImGui::Command(std::bind(&AssetBrowser::OnImGuiRender, this));
}

void AssetBrowser::FileOpenAction(const std::filesystem::directory_entry& ditr)
{
	const auto& path = ditr.path().string();
	const auto& ext = ditr.path().extension();

	//拡張子ごとに特殊なプログラム呼び出しがあれば実行
	if (ext == ".bt")
	{
		SceneManager::LoadScene<BTEditorScene>(path);
		SceneManager::WaitForLoadAndTransition();
	}
	else
	{
		ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
	}

}

void AssetBrowser::SDraw()
{
	GetInstance()->Draw();
}

void AssetBrowser::LoadResources()
{
	SpTextureManager::LoadTexture("Resources/icons/folder.png", "Engine_FolderIcon");
	SpTextureManager::LoadTexture("Resources/icons/file.png", "Engine_FileIcon");

	SpTextureManager::AddMasterTextureKey("Engine_FolderIcon");
	SpTextureManager::AddMasterTextureKey("Engine_FileIcon");
}

AssetBrowser* AssetBrowser::GetInstance()
{
	static AssetBrowser ins;
	return &ins;
}

AssetBrowser::AssetBrowser() : currentDirectory_(sAssetsDirectory)
{
}
