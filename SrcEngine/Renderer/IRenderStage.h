#pragma once
/*****************************************************************//**
 * @file   IRenderStage.h
 * @brief  レンダー順管理のステージの基底
 * 
 * @author Wrelf
 *********************************************************************/
#include <GPipeline.h>
#include <SpRootSignature.h>
/**
 * @brief レンダー順管理を行うためのレンダーステージの基底クラス
 */
class IRenderStage
{
public:
	virtual ~IRenderStage(){};
	/**
	 * @brief 初期化処理
	 * 
	 */
	virtual void Init() = 0;
	/**
	 * @brief 描画ステージの描画実行前処理
	 * 
	 */
	virtual void PreDraw() = 0;

	/**
	 * @brief 描画ステージの対応描画コマンド発行処理
	 * 
	 */
	virtual void Render() = 0;

	/**
	 * @brief 描画ステージの描画実行後処理
	 * 
	 */
	virtual void PostDraw() = 0;

	/**
	 * @brief 描画コマンドの登録
	 * 
	 * @param cmd
	 */
	virtual void DrawCommands(std::function<void(void)> cmd) = 0;
};

