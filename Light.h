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
		Vec3 dLightVec;//Œõ‚Ì•ûŒü‚Ì‹tƒxƒNƒgƒ‹
		float padding1 = 0;
		Float3 dLightColor;
	};

	static void Init();

	static void Use();

	static void UpdateLightData();

	static Light* GetInstance();

	static PointLightHandle CreatePointLight(Float3 position, Float3 color, Float3 attenuation);

	static void RemovePointLight(PointLightHandle handle);

	static PointLight* GetPointLightPtr(PointLightHandle handle);

	static Float3 GetPointLightPos(PointLightHandle handle);

	static void SetPointLightPos(PointLightHandle handle, Float3 pos);

	static void ClearAllPointLights();

	static DirectionalLight directional;

private:
	wConstBuffer<LightCBData> lightCB;

	PointLight 256 pointLights;

	Light(){ };
	~Light(){ };
};

