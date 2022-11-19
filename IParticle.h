#pragma once
#include <Essentials.h>

class IParticle
{
protected:
	void Init() {
		time = 0;
	}

	void Update() {
		time++;

		if (time > maxTime) active = false;
	};

	Float4 color;
	int time;
	int maxTime;
	bool active;
};
