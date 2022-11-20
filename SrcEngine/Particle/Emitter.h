#pragma once
#include "ParticleManager.h"
#include <IParticle.h>
#include "Essentials.h"
#include <Util.h>
#include <LineDrawer.h>

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

		activeCount = 0;
		for (auto itr = particles.begin(); itr != particles.end();)
		{
			itr->Update();

			if (!itr->active)
			{
				itr = particles.erase(itr);
			}
			else
			{
				vertMap[activeCount].position = itr->position;
				vertMap[activeCount].scale = itr->scale;
				vertMap[activeCount].color = itr->color;

				activeCount++;
				itr++;
			}
		}
	}

	void DrawEmitArea()
	{
		switch (shape)
		{
		case Shape::Cube:
			LineDrawer::DrawCube(position, radius, { 1.f, 1.f, 1.f, 1.f });
			break;

		case Shape::Sphere:
			LineDrawer::DrawCube(position, { radius.x, radius.x, radius.x }, { 1.f, 1.f, 1.f, 1.f });
			break;
		default:
			break;
		}
	};

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
			particlePos.x = (float)Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * (Util::Chance(50) ? 1 : -1);
			particlePos.y = (float)Util::RNG(0, RAND_MAX) / RAND_MAX * radius.y * (Util::Chance(50) ? 1 : -1);
			particlePos.z = (float)Util::RNG(0, RAND_MAX) / RAND_MAX * radius.z * (Util::Chance(50) ? 1 : -1);

			particlePos = (Vec3)particlePos + position;
			break;
		case Shape::Sphere:
			do {
				particlePos.x = (float)Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * (Util::Chance(50) ? 1 : -1);
				particlePos.y = (float)Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * (Util::Chance(50) ? 1 : -1);
				particlePos.z = (float)Util::RNG(0, RAND_MAX) / RAND_MAX * radius.x * (Util::Chance(50) ? 1 : -1);
			} while (
				((Vec3)particlePos).GetSquaredLength() > radius.x * radius.x
				);

			particlePos = (Vec3)particlePos + position;
			break;
		default:
			break;
		}

		particles.emplace_back(particlePos);
	}

	vector<ParticleType> particles;

	int timer = 0;
};