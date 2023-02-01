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
class SpRenderer
{
public:
	static void Init();
	static void Render();

	static SpRenderer* GetInstance();
private:
	unique_ptr<IRenderStage> stages[6] = {
		make_unique<SrOpaqueStage>(),
		make_unique<SrAddStage>(),
		make_unique<SrAlphaStage>(),
		make_unique<SrPostEffectStage>(),
		make_unique<SrSpriteStage>(),
		make_unique<SrImGuiStage>()
	};

	enum class StageTable {
		Opaque,
		Add,
		Alpha,
		PostEffect,
		Sprite,
		ImGui
	};
};

