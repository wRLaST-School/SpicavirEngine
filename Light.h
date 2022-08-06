#pragma once
#include "Essentials.h"
#include "wMath.h"
#include "wConstBuffer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
class Light
{
public:
	struct LightCBData 
	{
		Vec3 dLightVec;//���̕����̋t�x�N�g��
		float padding1 = 0;
		Float3 dLightColor;
		float padding2 = 0;

		PointLight::CBData pointLights[PointLight::MAX_LIGHTS];
	};

	static void Init();

	static void Use();

	static void UpdateLightData() throw (
		PointLight::QuantityOverflow
		);

	static Light* GetInstance();


	static PointLightKey CreatePointLight(Float3 position, Float3 color, Float3 attenuation, PointLightKey key);

	static void RemovePointLight(PointLightKey key);

	static PointLight* GetPointLightPtr(PointLightKey key);

	static Float3 GetPointLightPos(PointLightKey key);

	static void SetPointLightPos(PointLightKey key, Float3 pos);

	static void ClearAllPointLights();


	static DirectionalLight directional;

private:
	wConstBuffer<LightCBData> lightCB;

	map<PointLightKey, PointLight> pointLights;

	Light(){ };
	~Light(){ };
};

