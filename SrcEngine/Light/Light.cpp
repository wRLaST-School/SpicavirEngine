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
		throw PointLight::QuantityOverflow(PointLight::MAX_LIGHTS, (int32_t)instance->pointLights_.size());
	};

	for (auto itr = instance->pointLights_.begin(); itr != instance->pointLights_.end(); itr++)
	{
		PointLight::CBData* pLight = &instance->lightCB_.contents->pointLights[nextTransferIndex];
		pLight->pos = itr->second.pos;
		pLight->color = itr->second.color;
		pLight->att = itr->second.att;
		pLight->isActive = itr->second.isActive;

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

PointLightKey Light::CreatePointLight(const Float3& position, const Float3& color, const Float3& attenuation, const PointLightKey& key)
{
	GetInstance()->pointLights_.emplace(key, PointLight(position, color, attenuation)).first->second.name = key;
	return key;
}

void Light::RemovePointLight(const PointLightKey& key)
{
	GetInstance()->pointLights_.erase(key);
}

PointLight* Light::GetPointLightPtr(const PointLightKey& key)
{
	return &GetInstance()->pointLights_.find(key)->second;
}

Float3 Light::GetPointLightPos(const PointLightKey& key)
{
	return GetPointLightPtr(key)->pos;
}

void Light::SetPointLightPos(const PointLightKey& key, const Float3& pos)
{
	GetPointLightPtr(key)->pos = pos;
}

void Light::ClearAllPointLights()
{
	GetInstance()->pointLights_.clear();
}

DirectionalLight Light::sDirectional;