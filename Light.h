#pragma once
#include "Essentials.h"
#include "wMath.h"
#include "wConstBuffer.h"
#include "DirectionalLight.h"
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

	static DirectionalLight directional;

private:
	wConstBuffer<LightCBData> lightCB;

	Light(){ };
	~Light(){ };
};

