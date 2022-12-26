#pragma once
#include <Essentials.h>
#include <RTVManager.h>

class IPostEffector
{
public:
	static string name; //hlsl�́���XS.hlsl/����.hlsli�̕���

	struct UsedShader {
		bool VS = true;
		bool GS = false;
		bool PS = true;
	} usedShader;

	static void RegisterPipeline();
	static void RegisterRS();

	static void Effect(TextureKey baseTex, TextureKey targetTex);

	virtual void SetExclusiveCBV() = 0; // ��p�̒萔�o�b�t�@���Z�b�g����R�}���h�p(�p�����Effect�֐��̎������ŌĂяo������)
};