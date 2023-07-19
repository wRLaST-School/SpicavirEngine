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

	static void RegisterPipeline(const std::string& name);
	static void RegisterRS(const std::string& name);

	static void Effect(const TextureKey& baseTex, const TextureKey& targetTex, const std::string& name, std::function<void(void)> commands = [](void) {return; });

	virtual void SetExclusiveCBV() = 0; // ��p�̒萔�o�b�t�@���Z�b�g����R�}���h�p(�p�����Effect�֐��̎������ŌĂяo������)�A0�Ԃ̒萔�o�b�t�@
};

class PostEffectCommon
{
public:
	static void Init();

	static D3D12_VERTEX_BUFFER_VIEW sVbView;

	static Microsoft::WRL::ComPtr<ID3D12Resource> sVertBuff;
};