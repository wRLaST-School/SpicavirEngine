#pragma once
#include <Essentials.h>
#include <RTVManager.h>

class IPostEffector
{
public:
	static string name; //hlslの○○XS.hlsl/○○.hlsliの部分

	struct UsedShader {
		bool VS = true;
		bool GS = false;
		bool PS = true;
	} usedShader;

	static void RegisterPipeline();
	static void RegisterRS();

	static void Effect(TextureKey baseTex, TextureKey targetTex);

	virtual void SetExclusiveCBV() = 0; // 専用の定数バッファをセットするコマンド用(継承先のEffect関数の実装内で呼び出し推奨)
};