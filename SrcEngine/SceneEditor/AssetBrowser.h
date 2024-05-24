#pragma once
/*****************************************************************//**
 * @file   AssetBrowser.h
 * @brief  エディタ画面のAssetBrowserの表示と処理に関するファイル
 * 
 * @author Wrelf
 *********************************************************************/
#include <filesystem>
#include <SpTextureManager.h>
/**
 * @brief AssetBrowserのクラス
 */
class AssetBrowser final
{
public:
	/**
	 * @brief 描画を行う(Static)
	 * 
	 */
	static void SDraw();

	/**
	 * @brief 必須リソースの読み込み
	 * 
	 */
	static void LoadResources();

	/**
	 * @brief ファイルのDnD読み込み
	 */
	void CopyFileByDD(std::wstring path);

private:
	/**
	 * @brief ImGui描画時の処理
	 * 
	 */
	void OnImGuiRender();

	/**
	 * @brief 描画処理
	 * 
	 */
	void Draw();

	/**
	 * @brief ファイルがダブルクリックされた時の処理
	 * 
	 * @param ditr 開くファイルへのdirectory iterator
	 */
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

