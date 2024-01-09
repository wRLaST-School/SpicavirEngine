#pragma once
/*****************************************************************//**
 * @file   IPostEffector.h
 * @brief  ポストエフェクト作成の基底クラス
 * 
 * @author Wrelf
 *********************************************************************/
#include <Essentials.h>
#include <RTVManager.h>

/**
 * @brief ポストエフェクト作成の基底クラス
 */
class IPostEffector
{
public:
	/**
	 * @brief 描画に使用するシェーダー
	 */
	struct UsedShader {
		bool VS = true;
		bool GS = false;
		bool PS = true;
	} usedShader;

	/**
	 * @brief パイプライン作成
	 * 
	 * @param name PipelineManagerへの登録名称
	 */
	static void RegisterPipeline(const std::string& name);
	
	/**
	 * @brief RS作成
	 * 
	 * @param name RSManagerへの登録名称
	 */
	static void RegisterRS(const std::string& name);

	/**
	 * @brief ポストエフェクトの実行
	 * 
	 * @param baseTex ベースとなるテクスチャ
	 * @param targetTex 描画先テクスチャ
	 * @param name 使用するパイプラインやシェーダー等の名前
	 * @param commands ポストエフェクト実行前に実行したい処理
	 */
	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex, const std::string& name, std::function<void(void)> commands = [](void) {return; });

	/**
	 * @brief 専用の定数バッファをセットするコマンド用
	 * @details (継承先のEffect関数の実装内で呼び出し推奨)、0番の定数バッファ
	 */
	virtual void SetExclusiveCBV() = 0;
};

class PostEffectCommon
{
public:
	static void Init();

	static D3D12_VERTEX_BUFFER_VIEW sVbView;

	static Microsoft::WRL::ComPtr<ID3D12Resource> sVertBuff;
};