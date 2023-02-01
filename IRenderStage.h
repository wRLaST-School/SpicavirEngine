#pragma once
#include <GPipeline.h>
#include <SpRootSignature.h>
class IRenderStage
{
public:
	virtual void Init() = 0;
	virtual void PreStage() = 0;
	virtual void PostStage() = 0;
};

