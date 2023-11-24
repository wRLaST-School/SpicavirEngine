#pragma once
#include <filesystem>
#include <SpTextureManager.h>
class AssetBrowser final
{
public:
	static void SDraw();

	static void LoadResources();

private:
	void OnImGuiRender();

	void Draw();

	void FileOpenAction(const std::filesystem::directory_entry& ditr);

	std::filesystem::path currentDirectory_;

	float thumbnailSize = 128.f;
	float padding = 16.f;

	std::string selectedItemPath = "ERROR";

	//非同期プレビュー読み込み関連の排他制御
	exc_unordered_map<TextureKey, int32_t> loadedPreviewTextures;
	std::mutex mtx;
	
public://singleton
	AssetBrowser(const AssetBrowser&) = delete;
	AssetBrowser(AssetBrowser&&) = delete;
	AssetBrowser& operator=(const AssetBrowser&) = delete;
	AssetBrowser& operator=(AssetBrowser&&) = delete;

	static AssetBrowser* GetInstance();

private:
	AssetBrowser();
	~AssetBrowser() = default;
};

