#pragma once
#include <Essentials.h>
#include <RTVManager.h>
class IPostEffector
{
public:
	string name; //hlsl�́���XS.hlsl/����.hlsli�̕���

	struct UsedShader {
		bool VS = true;
		bool GS = false;
		bool PS = true;
	} usedShader;

	TextureKey rtvKey;

	static void RegisterPipeline();
	static void RegisterRS();

	static void Effect(TextureKey baseTex, TextureKey targetTex);
};