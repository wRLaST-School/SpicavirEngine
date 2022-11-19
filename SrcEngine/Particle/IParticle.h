#pragma once
#include <Essentials.h>
#include <SpTextureManager.h>
class IParticle
{
public:
	IParticle(Float3 pos):position(pos) 
	{	};

	void Init() {
		time = 0;
	}

	void Update() {
		time++;

		if (time > maxTime) active = false;
	};

	Float4 color;
	Float3 position;
	float scale;

	int time;
	int maxTime;
	bool active;

	static TextureKey texture;
};
