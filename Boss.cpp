#include "stdafx.h"
#include "Boss.h"
#include <Util.h>
#include <Input.h>
#include <GlobalTimer.h>
#include <Player.h>

Boss* Boss::sCurrent = nullptr;

void Boss::Load()
{
	ModelManager::Register("Resources/Models/Boss/boss.glb", "Boss", true);
	SpTextureManager::LoadTexture("Resources/white.png", "white");

	ModelManager::Register("Resources/Models/Marker/Marker.obj", "Marker", true);
	SpTextureManager::LoadTexture("Resources/Marker.png", "Marker");
}

void Boss::Init()
{
	model = ModelManager::GetModel("Boss");
	position = { 0.f, 5.f, 0.f };

	for (auto& m : markers) m.InitModel();
}

void Boss::Update()
{
	if (GlobalTimer::sTime % 120 == 0)
	{
		if (Util::Chance(50))
		{
			CastMarkerLine3();
		}
		else
		{
			CastMarkerAim1Rand5();
		}
	}
	UpdateMarkers();
	UpdateLineAttacks();
	UpdateMatrix();
}

void Boss::Draw()
{
	Object3D::Draw("white");
	DrawMarkers();
}

Boss* Boss::Get()
{
	return sCurrent;
}

void Boss::Set(Boss* boss)
{
	sCurrent = boss;
}

void Boss::CastMarker(Float3 pos)
{
	for (auto& m : markers)
	{
		if (!m.active) {
			m.Cast(pos);
			break;
		}
	}
}

void Boss::CastMarkerAim1Rand5()
{
	CastMarker({Player::Get()->position.x, 0, Player::Get()->position.z});

	for (size_t i = 0; i < 5; i++)
	{
		CastMarker({ (float)Util::RNG(-30, 30), 0.001f, (float)(Util::RNG(-30, 30)) });
	}
}

void Boss::CastMarkerLine3()
{
	if (Util::Chance(50))
	{
		for (float z = -10.f; z <= 10.f; z += 10.f)
		{
			for (float x = -28.f; x < 28.f; x += 4.f) {
				CastMarker({ x, 0.f, z });
			}
		}
	}
	else
	{
		for (float x = -10.f; x <= 10.f; x += 10.f)
		{
			for (float z = -28.f; z < 28.f; z += 4.f) {
				CastMarker({ x, 0.f, z });
			}
		}
	}
}

void Boss::DrawMarkers()
{
	for (auto& m : markers) if (m.active) m.Draw();
}

void Boss::UpdateMarkers()
{
	for (auto& m : markers) if (m.active) m.Update();
}

void Boss::CastLineTriple()
{
	Vec3 front = rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();

	if (Util::Chance(50))
	{
		for (float x = -10.f; x <= 10.f; x += 10.f)
		{
			CastLine((Vec3)position + front * 10.f + side * x, /*front�̊p�x*/);
		}
	}
	else
	{
		for (float z = -10.f; z <= 10.f; z += 10.f)
		{
		}
	}
}

void Boss::CastLine(Float3 pos, float angle)
{
	lineAttacks.emplace_back(pos, angle);
}

void Boss::UpdateLineAttacks()
{
	for (auto itr = lineAttacks.begin(); itr != lineAttacks.end();)
	{
		itr->Update();

		if (!itr->active)
		{
			itr = lineAttacks.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}
