#pragma once
#include <Essentials.h>
#include <RTVManager.h>

class IPostEffector
{
public:
	struct UsedShader {
		bool VS = true;
		bool GS = false;
		bool PS = true;
	} usedShader;

	static void RegisterPipeline(string name);
	static void RegisterRS(string name);

	static void Effect(TextureKey baseTex, TextureKey targetTex, string name);

	virtual void SetExclusiveCBV() = 0; // 専用の定数バッファをセットするコマンド用(継承先のEffect関数の実装内で呼び出し推奨)、0番の定数バッファ
};

class PostEffectCommon
{
public:
	static void Init();

	static D3D12_VERTEX_BUFFER_VIEW vbView;

	static ComPtr<ID3D12Resource> vertBuff;
};