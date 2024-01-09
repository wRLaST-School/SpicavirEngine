#pragma once
/*****************************************************************//**
 * @file   InspectorWindow.h
 * @brief  エディタ画面のInspectorWindowの表示と処理に関するファイル
 * 
 * @author Wrelf
 *********************************************************************/

/**
 * @brief InspectorWindowのクラス
 */
class InspectorWindow
{
public:
	/**
	 * @brief オブジェクトを選択する関数
	 * @details HierarchyPanel等でInspectorWindowに表示するコンポーネントを選択する時に使用する。
	 * 
	 * @param component 選択するコンポーネント
	 */
	static void SelectObject(IComponent* component);
	
	/**
	 * @brief 描画処理
	 * 
	 */
	void DrawWindow();

	/**
	 * @brief 選択されているコンポーネントを取得
	 * 
	 * @return 選択されているコンポーネントのポインタ
	 */
	template <class Type>
	static Type* GetSelected();

	/**
	 * @brief Static描画処理
	 * 
	 */
	static void SDraw();

private:
	IComponent* selected_ = nullptr;

public://singleton
	InspectorWindow(const InspectorWindow&) = delete;
	InspectorWindow(InspectorWindow&&) = delete;
	InspectorWindow& operator=(const InspectorWindow&) = delete;
	InspectorWindow& operator=(InspectorWindow&&) = delete;

	static InspectorWindow* GetInstance();

private:
	InspectorWindow() = default;
	~InspectorWindow() = default;
};

template<class Type>
inline Type* InspectorWindow::GetSelected()
{
	return dynamic_cast<Type*>(GetInstance()->selected_);
}