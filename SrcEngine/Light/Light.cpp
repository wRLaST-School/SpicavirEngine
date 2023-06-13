#include "Light.h"
#include <SpDirectX.h>

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
	Light* instance = GetInstance();

	//directional lights
	instance->lightCB.contents->dLightVec = -directional.direction;
	instance->lightCB.contents->dLightColor = directional.color;

	//point lights
	int32_t nextTransferIndex = 0;

	if (instance->pointLights.size() > PointLight::MAX_LIGHTS) 
	{
		throw PointLight::QuantityOverflow(PointLight::MAX_LIGHTS, (int32_t)instance->pointLights.size());
	};

	for (auto itr = instance->pointLights.begin(); itr != instance->pointLights.end(); itr++)
	{
		PointLight::CBData* pLight = &instance->lightCB.contents->pointLights[nextTransferIndex];
		pLight->pos = itr->second.pos;
		pLight->color = itr->second.color;
		pLight->att = itr->second.att;
		pLight->isActive = itr->second.isActive;

		nextTransferIndex++;
	}

	for (nextTransferIndex; nextTransferIndex < PointLight::MAX_LIGHTS; nextTransferIndex++)
	{
		instance->lightCB.contents->pointLights[nextTransferIndex] =
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
	GetInstance()->pointLights.emplace(key, PointLight(position, color, attenuation)).first->second.name = key;
	return key;
}

void Light::RemovePointLight(const PointLightKey& key)
{
	GetInstance()->pointLights.erase(key);
}

PointLight* Light::GetPointLightPtr(const PointLightKey& key)
{
	return &GetInstance()->pointLights.find(key)->second;
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
	GetInstance()->pointLights.clear();
}

DirectionalLight Light::directional;