#pragma once
/*****************************************************************//**
 * @file   HierarchyPanel.h
 * @brief  エディタ画面のHierarchyPanelの表示と処理に関するファイル
 * 
 * @author Wrelf
 *********************************************************************/

/**
 * @brief HierarchyPanel用のクラス
 */
class HierarchyPanel final
{
public:
	/**
	 * @brief 描画処理
	 * 
	 */
	void Draw();

	/**
	 * @brief ImGui描画時の処理
	 * 
	 */
	void OnImGuiRender();

	/**
	 * @brief 再帰的にリソースを表示する関数
	 * 
	 * @param current 現在の対象コンポーネント
	 */
	void ShowItemRecursive(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付処理
	 * 
	 * @param current 対象コンポーネント
	 */
	void DragDropTarget(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付結果が画像だった場合
	 * 
	 * @param current 対象コンポーネント
	 */
	void DDTargetTexture(IComponent* current);

	/**
	 * @brief ドラッグドロップ受付結果がモデルだった場合
	 * 
	 * @param current 対象コンポーネント
	 */
	void DDTargetModel(IComponent* current);

	/**
	 * @brief Static描画関数
	 * 
	 */
	static void SDraw();

private:

	int32_t itemIndex = 0;

public://singleton
	HierarchyPanel(const HierarchyPanel&) = delete;
	HierarchyPanel(HierarchyPanel&&) = delete;
	HierarchyPanel& operator=(const HierarchyPanel&) = delete;
	HierarchyPanel& operator=(HierarchyPanel&&) = delete;

	static HierarchyPanel* GetInstance();

private:
	HierarchyPanel() = default;
	~HierarchyPanel() = default;
};

