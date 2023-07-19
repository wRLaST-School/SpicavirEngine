#include "stdafx.h"
#include "SpEffekseer.h"
#include <SpDirectX.h>

Effekseer::RefPtr<EffekseerRenderer::Renderer> SpEffekseer::sEfkRenderer = nullptr;

Effekseer::RefPtr<Effekseer::Manager> SpEffekseer::sEfkManager = nullptr;

Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> SpEffekseer::sEfkMemoryPool;

Effekseer::RefPtr<EffekseerRenderer::CommandList> SpEffekseer::sEfkCmdList;

exc_unordered_map<EffectKey, Effekseer::EffectRef> SpEffekseer::sEffects;

std::list <EffectKey > SpEffekseer::sPerSceneEffects[2];
int32_t SpEffekseer::sCurrentSceneResIndex;

DXGI_FORMAT format[] = {DXGI_FORMAT_R8G8B8A8_UNORM};

void SpEffekseer::Init()
{
	sEfkRenderer = EffekseerRendererDX12::Create(
		GetWDX()->dev.Get(),
		GetWDX()->cmdQueue.Get(),
		2,
		format,
		1,
		DXGI_FORMAT_D32_FLOAT,
		false,
		10000
	);

	sEfkManager = Effekseer::Manager::Create(10000);

	sEfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	sEfkManager->SetSpriteRenderer(sEfkRenderer->CreateSpriteRenderer());
	sEfkManager->SetRibbonRenderer(sEfkRenderer->CreateRibbonRenderer());
	sEfkManager->SetRingRenderer(sEfkRenderer->CreateRingRenderer());
	sEfkManager->SetTrackRenderer(sEfkRenderer->CreateTrackRenderer());
	sEfkManager->SetModelRenderer(sEfkRenderer->CreateModelRenderer());
	sEfkManager->SetTextureLoader(sEfkRenderer->CreateTextureLoader());
	sEfkManager->SetModelLoader(sEfkRenderer->CreateModelLoader());

	sEfkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(sEfkRenderer->GetGraphicsDevice());
	sEfkCmdList = EffekseerRenderer::CreateCommandList(sEfkRenderer->GetGraphicsDevice(), sEfkMemoryPool);
	sEfkRenderer->SetCommandList(sEfkCmdList);
}

void SpEffekseer::Update()
{
	sEfkManager->Update();
	sEfkMemoryPool->NewFrame();
}

void SpEffekseer::Draw()
{
	EffekseerRendererDX12::BeginCommandList(sEfkCmdList, GetWDX()->cmdList.Get());

	sEfkRenderer->BeginRendering();
	sEfkManager->Draw();
	sEfkRenderer->EndRendering();

	EffekseerRendererDX12::EndCommandList(sEfkCmdList);
}

EffectKey SpEffekseer::Load(std::wstring texfolder, std::wstring path, EffectKey key)
{
	sEffects.Access([&](auto& map) {
		map.emplace(key, Effekseer::Effect::Create(
			sEfkManager,
			(const EFK_CHAR*)path.c_str(),
			1.0f,
			(const EFK_CHAR*)texfolder.c_str()
		));
	});

	sPerSceneEffects[sCurrentSceneResIndex].push_back(key);
	return key;
}

Effekseer::Handle SpEffekseer::Play(EffectKey key, Float3 pos)
{
	Effekseer::Handle ret;

	sEffects.Access([&](auto& map) {
		ret = sEfkManager->Play(map.at(key), pos.x, pos.y, pos.z);
	});

	return ret;
}

void SpEffekseer::SetMatrices(Effekseer::Matrix44 view, Effekseer::Matrix44 proj)
{
	sEfkRenderer->SetCameraMatrix(view);
	sEfkRenderer->SetProjectionMatrix(proj);
}

void SpEffekseer::ReleasePerSceneEffects()
{
	int32_t lastSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
	for (auto itr = sPerSceneEffects[lastSceneResIndex].begin(); itr != sPerSceneEffects[lastSceneResIndex].end(); itr++)
	{
		bool usingInCurrentScene = false;
		for (auto& key : sPerSceneEffects[sCurrentSceneResIndex])
		{
			if (key == *itr)
			{
				usingInCurrentScene = true;
			}
		}

		if (!usingInCurrentScene) //今のシーンで使われていないならリリース
		{
			//TODO:再生を止める処理を書くなら書く

			sEffects.Access(
				[&](auto& map) {
					map.erase(*itr);
				}
			);
		}
	}

	sPerSceneEffects[lastSceneResIndex].clear();
}

void SpEffekseer::PreLoadNewScene()
{
	sCurrentSceneResIndex = sCurrentSceneResIndex == 0 ? 1 : 0;
}
