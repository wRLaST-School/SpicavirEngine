#pragma once
#include <Essentials.h>
#include <functional>
#include <GPipelineManager.h>
#include <RootSignatureManager.h>
#include <IRenderStage.h>
#include <SrAddStage.h>
#include <SrAlphaStage.h>
#include <SrOpaqueStage.h>
#include <SrSpriteStage.h>
#include <SrPostEffectStage.h>
#include <SrImGuiStage.h>
#include <SrParticleStage.h>
class SpRenderer
{
public:
	static void Init();
	static void Render();

	static SpRenderer* GetInstance(); 
	
	enum class Stage {
		Opaque,
		Add,
		Alpha,
		Particle,
		PostEffect,
		Sprite,
		ImGui
	};

	static void DrawCommand(std::function<void()> cmd, const Stage& stg);
	static void RegisterAlphaObj(Object3D* obj);
private:
	std::unique_ptr<IRenderStage> stages_[7] = {
		std::make_unique<SrOpaqueStage>(),
		std::make_unique<SrAddStage>(),
		std::make_unique<SrAlphaStage>(),
		std::make_unique<SrParticleStage>(),
		std::make_unique<SrPostEffectStage>(),
		std::make_unique<SrSpriteStage>(),
		std::make_unique<SrImGuiStage>()
	};
};

