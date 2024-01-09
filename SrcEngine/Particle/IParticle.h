#pragma once
/*****************************************************************//**
 * @file   IParticle.h
 * @brief  パーティクルの粒ひとつひとつの動きを設定するための基底クラス
 * @details 継承して作成したクラスをEmitterのテンプレートとして渡すとパーティクルの作成が可能
 * 
 * @author Wrelf
 *********************************************************************/
#include <Essentials.h>
#include <SpTextureManager.h>
/**
 * @brief パーティクルの粒ひとつひとつの動きを設定するための基底クラス
 */
class IParticle
{
public:
	/**
	 * @brief コンストラクタ
	 *  
	 * @param pos 座標
	 */
	IParticle(const Float3& pos):position(pos)
	{	};

	/**
	 * @brief パーティクル発生時の処理
	 * 
	 */
	void Init() {
		time = 0;
	}

	/**
	 * @brief パーティクルの更新処理
	 * 
	 */
	void Update() {
		time++;

		if (time > maxTime) active = false;
	};

	Float4 color = {1.f, 1.f, 1.f, 1.f};
	Float3 position;
	float scale = 1.0f;

	int32_t time = 0;
	int32_t maxTime = 120;
	bool active = true;

	static TextureKey sTexture;
};
