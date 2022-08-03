#include "Light.h"

Light::Light()
{
}

void Light::Use()
{
	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(5, lightCB.buffer->GetGPUVirtualAddress());
}

void Light::UpdateLightData()
{
	lightCB.contents->dLightVec = -directionalLight.direction;

	lightCB.contents->dLightColor = directionalLight.color;
}

