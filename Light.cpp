#include "Light.h"

void Light::Init()
{
	directional = DirectionalLight();
}

void Light::Use()
{
	GetWDX()->cmdList->SetGraphicsRootConstantBufferView(RootParamIndex::LightBasic, GetInstance()->lightCB.buffer->GetGPUVirtualAddress());
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

PointLightHandle Light::CreatePointLight(Float3 position, Float3 color, Float3 attenuation)
{
	
	return GetInstance()->pointLights.size() - 1;
}

void Light::RemovePointLight(PointLightHandle handle)
{
}

DirectionalLight Light::directional;