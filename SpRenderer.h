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
		Particle,
		Alpha,
		Sprite,
		PostEffect,
		ImGui
	};

	static void DrawCommand(function<void()> cmd, Stage stg);
	static void RegisterAlphaObj(Object3D* obj);
private:
	unique_ptr<IRenderStage> stages[7] = {
		make_unique<SrOpaqueStage>(),
		make_unique<SrAddStage>(),
		make_unique<SrParticleStage>(),
		make_unique<SrAlphaStage>(),
		make_unique<SrSpriteStage>(),
		make_unique<SrPostEffectStage>(),
		make_unique<SrImGuiStage>()
	};
};

