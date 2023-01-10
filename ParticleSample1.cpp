#include "stdafx.h"
#include "ParticleSample1.h"

TextureKey ParticleSample1::texture = "particle1";

void ParticleSample1::Update()
{
	IParticle::Update();
	//this->position = vel.SetLength(speed) + position;
	//speed -= speedDec;

	float t = (float)time / maxTime;

	scale = { 1.f - t };
}
