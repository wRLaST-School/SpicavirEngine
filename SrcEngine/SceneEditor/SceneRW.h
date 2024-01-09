#pragma once
/*****************************************************************//**
 * @file   SceneRW.h
 * @brief  シーン読み込み・書き出しを行う
 * 
 * @author Wrelf
 *********************************************************************/
#include <IScene.h>

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)

/**
 * @brief シーンの読み書きに関する処理を行うクラス
 */
class SceneRW
{
public:
	/**
	 * @brief シーンを保存する
	 * 
	 * @param scene 対象のシーン
	 * @param filePath 保存先ファイルパス
	 */
	static void SaveScene(IScene* scene, std::string filePath);

	/**
	 * @brief シーンで使用されるテクスチャを保存する処理
	 * 
	 * @param dist 保存するシーンのjsonオブジェクト
	 */
	static void SaveTextures(nlohmann::json& dist);

	/**
	 * @brief シーンで使用されるモデルを保存する処理
	 *
	 * @param dist 保存するシーンのjsonオブジェクト
	 */
	static void SaveModels(nlohmann::json& dist);

	/**
	 * @brief シーンで使用されるサウンドを保存する処理
	 *
	 * @param dist 保存するシーンのjsonオブジェクト
	 */
	static void SaveSounds(nlohmann::json& dist);

	/**
	 * @brief シーンで使用されるエフェクトを保存する処理
	 *
	 * @param dist 保存するシーンのjsonオブジェクト
	 */
	static void SaveEfkEffects(nlohmann::json& dist);

	/**
	 * @brief シーンの読み込みを行う
	 * 
	 * @param scene 読み込み先のシーン
	 * @param filePath 読み込み元ファイルパス
	 */
	static void LoadScene(IScene* scene, std::string filePath);

	/**
	 * @brief リソースの読み込みを行う
	 * 
	 * @param root 読み込むシーンのjsonオブジェクト
	 */
	static void LoadResources(nlohmann::json& root);

	/**
	 * @brief シーンで使用されるテクスチャを読み込む処理
	 * 
	 * @param textures 読み込むテクスチャリストのjsonオブジェクト
	 */
	static void LoadTextures(nlohmann::json& textures);

	/**
	 * @brief シーンで使用されるモデルを読み込む処理
	 *
	 * @param models 読み込むモデルリストのjsonオブジェクト
	 */
	static void LoadModels(nlohmann::json& models);

	/**
	 * @brief シーンで使用されるサウンドを読み込む処理
	 *
	 * @param sounds 読み込むサウンドリストのjsonオブジェクト
	 */
	static void LoadSounds(nlohmann::json& sounds);

	/**
	 * @brief シーンで使用されるエフェクトを読み込む処理
	 *
	 * @param efks 読み込むエフェクトリストのjsonオブジェクト
	 */
	static void LoadEfkEffects(nlohmann::json& efks);

private:
	bool waitingForLoad_ = false;

public://singleton
	SceneRW(const SceneRW&) = delete;
	SceneRW(SceneRW&&) = delete;
	SceneRW& operator=(const SceneRW&) = delete;
	SceneRW& operator=(SceneRW&&) = delete;

	static SceneRW* GetInstance();

private:
	SceneRW() {};
	~SceneRW() = default;
};

