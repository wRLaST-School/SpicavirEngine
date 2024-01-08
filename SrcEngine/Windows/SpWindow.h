#pragma once
/*****************************************************************//**
 * @file   SpWindow.h
 * @brief  Windows API関連の処理を行う
 * @details 複数ウィンドウ対応WIP
 * 
 * @author Wrelf
 *********************************************************************/
#include "Essentials.h"

/**
 * @brief WindowsAPI関連の処理を行うクラス
 */
class SpWindow
{
public:
	/**
	 * @brief ウィンドウの生成
	 * 
	 * @param title ウィンドウタイトル
	 * @param windowWidth ウィンドウ横幅
	 * @param windowHeight ウィンドウ縦幅
	 */
	void Create(LPCWSTR title, int32_t windowWidth, int32_t windowHeight);
	
	/**
	 * @brief Window終了処理
	 * 
	 */
	void Close();

	/**
	 * @brief メッセージを処理
	 * 
	 * @return 終了リクエストがあればtrue(フレームワーク用)
	 */
	bool ProcessMessage();

	HWND hwnd;
	WNDCLASSEX  w{};

	int32_t width = 1280;
	int32_t height = 720;
private:
};

/// <summary>
///	SpWindowリストから指定IDのSpWindowを探して返す。
/// IDを指定しない場合はSetDefaultWindowIDで指定したIDを使う。
/// </summary>
/// <param name="ID">string型のID</param>
/// <returns>成功:SpWindowへのポインタ,失敗:nullptr</returns>
SpWindow* GetSpWindow(const std::string& ID);
SpWindow* GetSpWindow();
extern std::map<std::string, SpWindow> wWindowList;
void RegisterSpWindow(SpWindow spwnd, const std::string& ID);
void SetDefaultWindowID(const std::string& ID);

void CloseAllSpWindow();
