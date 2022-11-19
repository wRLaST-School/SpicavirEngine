#pragma once
#include "ParticleManager.h"
#include <IParticle.h>
#include "Essentials.h"
#include <Util.h>

template <class ParticleType>
class Emitter : public ParticleManager
{
public:
	Emitter() {
		this->texture = ParticleType::texture;
	}

	void Update() {
		timer++;
		if (timer > timeBetweenEmit) {
			timer = 0;

			for (int i = 0; i < quantity; i++) { Emit(); }
		}

		for (auto itr = particles.begin(); itr != particles.end(); itr++)
		{
			itr->Update();

			if (!itr->active)
			{
				itr = particles.erase(itr);
			}
			else
			{
				vertMap[activeCount]->position = itr->position;
				vertMap[activeCount]->scale = itr->scale;
				vertMap[activeCount]->color = itr->color;
				
				activeCount++;
				itr++;
			}
		}
	}

	//�G�~�b�^�̍��W
	Float3 position;

	//�o���͈͂̔��a�Ashape��sphere�Ȃ�x�̂ݎg�p
	Float3 radius;

	//�o���͈͂̌`��
	enum class Shape {
		Cube,
		Sphere
	} shape = Shape::Cube;

	//�����ɔ������鐔
	int quantity = 1;

	//�����Ԋu(�t���[��)
	int timeBetweenEmit = 1;
private:
	void Emit() {
		Float3 particlePos = {};

		switch (shape)
		{
		case Shape::Cube:
			particlePos.x = Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * Util::Chance(50) ? 1 : -1;
			particlePos.y = Util::RNG(0, RAND_MAX) / RAND_MAX * radius.y * Util::Chance(50) ? 1 : -1;
			particlePos.z = Util::RNG(0, RAND_MAX) / RAND_MAX * radius.z * Util::Chance(50) ? 1 : -1;
			break;
		case Shape::Sphere:
			do {
				particlePos.x = Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * Util::Chance(50) ? 1 : -1;
				particlePos.y = Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * Util::Chance(50) ? 1 : -1;
				particlePos.z = Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * Util::Chance(50) ? 1 : -1;
			} while (
				((Vec3)particlePos).GetSquaredLength() > radius.x * radius.x
				);
			break;
		default:
			break;
		}

		particles.emplace_back(particlePos);
	}

	vector<ParticleType> particles;

	int timer = 0;
};