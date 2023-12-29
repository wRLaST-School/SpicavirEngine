#pragma once
/*****************************************************************//**
 * @file   ResourceWindow.h
 * @brief  エディタ画面のResourceWindowの表示と処理に関するファイル
 * 
 * @author Wrelf
 *********************************************************************/

/**
 * @brief ResourceWindowのクラス
 */
class ResourceWindow
{
public:
	/**
	 * @brief 描画処理
	 * 
	 */
	void Draw();

	/**
	 * @brief テクスチャ一覧の描画
	 * 
	 */
	void DrawTextureList();

	/**
	 * @brief モデル一覧の描画
	 * 
	 */
	void DrawModelList();

	/**
	 * @brief ドラッグドロップ受付
	 * 
	 */
	void DragDropTarget();

	/**
	 * @brief レイアウトを揃える処理の共通関数
	 * 
	 */
	void AdjustLayout();

	/**
	 * @brief Static描画処理
	 * 
	 */
	static void SDraw();

private:
	float thumbnailSize = 128.f;
	float padding = 16.f;

public://singleton
	ResourceWindow(const ResourceWindow&) = delete;
	ResourceWindow(ResourceWindow&&) = delete;
	ResourceWindow& operator=(const ResourceWindow&) = delete;
	ResourceWindow& operator=(ResourceWindow&&) = delete;

	static ResourceWindow* GetInstance();

private:
	ResourceWindow() = default;
	~ResourceWindow() = default;
};

