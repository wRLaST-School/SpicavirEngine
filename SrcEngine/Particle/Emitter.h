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
	//エミッタの座標
	Float3 position;

	//出現範囲の半径、shapeがsphereならxのみ使用
	Float3 radius;

	//出現範囲の形状
	enum class Shape {
		Cube,
		Sphere
	} shape = Shape::Cube;

	//同時に発生する数
	int32_t quantity = 1;

	//発生間隔(フレーム)
	int32_t timeBetweenEmit = 1;

	//trueならパーティクルを生成する
	bool active = false;

	//trueならパーティクルを一度生成すると自動的にactiveをfalseに
	bool emitOnce = false;

public:
	Emitter() {
		this->texture = ParticleType::sTexture;
	}

	void Activate() { active = true; };
	void Deactivate() { active = false; };

	void Update() {
		timer_++;
		if (timer_ > timeBetweenEmit) {
			timer_ = 0;

			for (int32_t i = 0; i < quantity; i++) { Emit(); }

			if (emitOnce) { active = false; }
		}

		activeCount = 0;
		for (auto itr = particles_.begin(); itr != particles_.end();)
		{
			itr->Update();

			if (!itr->active)
			{
				itr = particles_.erase(itr);
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

private:
	void Emit() {
		if (!active)
		{
			return;
		}

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

		particles_.emplace_back(particlePos);
	}

	std::vector<ParticleType> particles_;

	int32_t timer_ = 0;
};