#pragma once
/*****************************************************************//**
 * @file   DockPanel.h
 * @brief  エディタのドックパネル
 * 
 * @author Wrelf
 *********************************************************************/
#include <SpImGui.h>
/**
 * @brief エディタ用のドックパネル制御クラス
 */
class DockPanel
{
public:
	/**
	 * @brief ImGui関連処理
	 * 
	 */
	static void EnableScreenDock();

	/**
	 * @brief ゲーム画面プレビューの表示
	 * 
	 */
	static void DrawViewPort();

	/**
	 * @brief ドラッグドロップ受付
	 *
	 */
	static void DragDropTarget();

	/**
	 * @brief DD Texture
	 */
	static void DDTargetTexture();

	/**
	 * @brief シーン保存ダイアログ
	 * 
	 */
	static void DrawSaveDialog();

	/**
	 * @brief シーン読み込みダイアログ(使用非推奨)
	 * 
	 */
	static void DrawLoadDialog();

	/**
	 * @brief 他の場所からゲーム画面プレビューにアクセスするための関数
	 * 
	 * @return ImGuiWindow*型
	 */
	static ImGuiWindow* GetViewPortWindow();
private:
	inline static ImGuiWindow* sViewPortWindow;

	inline static bool showSaveDialog = false;
	inline static bool showLoadDialog = false;
};

