#include "Light.h"

void Light::Init()
{
	directional = DirectionalLight();
}

void Light::Use()
{
	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(5, GetInstance()->lightCB.buffer->GetGPUVirtualAddress());
}

void Light::UpdateLightData()
{
	GetInstance()->lightCB.contents->dLightVec = -directional.direction;

	GetInstance()->lightCB.contents->dLightColor = directional.color;
}

Light* Light::GetInstance()
{
	static Light obj;
	return &obj;
}

DirectionalLight Light::directional;