#pragma once
#include <Essentials.h>
#include <SpTextureManager.h>
class IParticle
{
public:
	IParticle(const Float3& pos):position(pos)
	{	};

	void Init() {
		time = 0;
	}

	void Update() {
		time++;

		if (time > maxTime) active = false;
	};

	Float4 color = {1.f, 1.f, 1.f, 1.f};
	Float3 position;
	float scale = 1.0f;

	int32_t time = 0;
	int32_t maxTime = 120;
	bool active = true;

	static TextureKey texture;
};
