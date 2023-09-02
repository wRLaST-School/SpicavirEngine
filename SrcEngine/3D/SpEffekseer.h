#pragma once
#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(push)
#pragma warning(disable:26495)
#pragma warning(push)
#pragma warning(disable:26451)
#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(push)
#pragma warning(disable:4267)
#pragma warning(push)
#pragma warning(disable:26813)
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)

#include <exc_unordered_map.h>

typedef std::string EffectKey;

class SpEffekseer
{
public:
	static void Init();

	static void Update();

	static void Draw();

	static EffectKey Load(std::wstring texFolder, std::wstring path, EffectKey key);

	static Effekseer::Handle Play(EffectKey key, Float3 pos);

	static void SetMatrices(Effekseer::Matrix44 view, Effekseer::Matrix44 proj);

	static void ReleasePerSceneEffects();

	static void PreLoadNewScene();

	static Effekseer::Manager* Manager();

private:
	static Effekseer::RefPtr<EffekseerRenderer::Renderer> sEfkRenderer;

	static Effekseer::RefPtr<Effekseer::Manager> sEfkManager;

	static Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> sEfkMemoryPool;

	static Effekseer::RefPtr<EffekseerRenderer::CommandList> sEfkCmdList;

	static exc_unordered_map<EffectKey, Effekseer::EffectRef> sEffects;

	static std::list <EffectKey > sPerSceneEffects[2];
	static int32_t sCurrentSceneResIndex;
};

