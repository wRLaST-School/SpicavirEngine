#pragma once
#include <GPipeline.h>
#include <SpRootSignature.h>
class IRenderStage
{
public:
	virtual ~IRenderStage(){};
	virtual void Init() = 0;
	virtual void PreDraw() = 0;
	virtual void Render() = 0;
	virtual void PostDraw() = 0;
	virtual void DrawCommands(std::function<void(void)> cmd) = 0;
};

