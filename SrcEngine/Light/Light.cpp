#include "Light.h"
#include <SpDirectX.h>

void Light::Init()
{
	sDirectional = DirectionalLight();
}

void Light::Use()
{
	GetSpDX()->cmdList->SetGraphicsRootConstantBufferView(RootParamIndex::LightBasic, GetInstance()->lightCB_.buffer->GetGPUVirtualAddress());
}

void Light::UpdateLightData()
{
	Light* instance = GetInstance();

	//directional lights
	instance->lightCB_.contents->dLightVec = -sDirectional.direction;
	instance->lightCB_.contents->dLightColor = sDirectional.color;

	//point lights
	int32_t nextTransferIndex = 0;

	if (instance->pointLights_.size() > PointLight::MAX_LIGHTS) 
	{
		throw PointLight::ReachedLightLimit(PointLight::MAX_LIGHTS, (int32_t)instance->pointLights_.size());
	};

	for (auto itr = instance->pointLights_.begin(); itr != instance->pointLights_.end(); itr++)
	{
		PointLight::CBData* pLight = &instance->lightCB_.contents->pointLights[nextTransferIndex];
		pLight->pos =  (*itr)->pos;
		pLight->color = (*itr)->color;
		pLight->att = (*itr)->att;
		pLight->isActive = (*itr)->isActive;

		nextTransferIndex++;
	}

	for (nextTransferIndex; nextTransferIndex < PointLight::MAX_LIGHTS; nextTransferIndex++)
	{
		instance->lightCB_.contents->pointLights[nextTransferIndex] =
		{
			{},
			{},
			{},
			false
		};
	}
}

Light* Light::GetInstance()
{
	static Light obj;
	return &obj;
}

void Light::RegisterPointLight(PointLight* ptr)
{
	GetInstance()->pointLights_.emplace_back(ptr);
}

void Light::ClearPointLight(PointLight* ptr)
{
	for (auto itr = GetInstance()->pointLights_.begin(); itr != GetInstance()->pointLights_.end();)
	{
		if ((*itr) == ptr)
		{
			itr = GetInstance()->pointLights_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Light::ClearAllPointLights()
{
	GetInstance()->pointLights_.clear();
}

DirectionalLight Light::sDirectional;