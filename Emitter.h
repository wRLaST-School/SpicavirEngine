#pragma once
#include "ParticleManager.h"
#include <IParticle.h>
#include "Essentials.h"

template <class ParticleType>
class Emitters : public ParticleManager
{
public:
	Emitter() {

	};

private:
	vector<ParticleType> particles;
};