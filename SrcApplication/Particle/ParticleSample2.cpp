#include "stdafx.h"
#include "ParticleSample2.h"

TextureKey ParticleSample2::texture = "particle1";

void ParticleSample2::Update()
{
	IParticle::Update(); 
	speed += speedInc;
	this->position = vel.SetLength(speed) + position;

	float t = (float)time / maxTime;

	scale =(1 - t) * 1.0f + 0.2f;
}