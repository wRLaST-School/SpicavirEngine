#include "stdafx.h"
#include "Boss.h"
#include <Util.h>
#include <Input.h>
#include <GlobalTimer.h>
#include <Player.h>
#include <SpImGui.h>

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
	static float rotY;

	SpImGui::Command([&] {
		if (ImGui::Begin("Boss"))
		{
			ImGui::DragFloat("RotY", &rotY);
			ImGui::InputFloat("Marker Line 3 Spacing", &markerLine3Spacing);
			ImGui::InputFloat("Line Attack Spacing", &lineAttackSpacing);
		}
		ImGui::End();
	});

	rotation = Quaternion(Vec3(0, 1, 0), rotY);

	if (GlobalTimer::sTime % 120 == 0)
	{
		if (Util::Chance(50))
		{
			CastLineTriple();
		}
		else if (Util::Chance(50))
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
		for (float z = -markerLine3Spacing; z <= markerLine3Spacing; z += markerLine3Spacing)
		{
			for (float x = -28.f; x < 28.f; x += 4.f) {
				CastMarker({ x, 0.f, z });
			}
		}
	}
	else
	{
		for (float x = -markerLine3Spacing; x <= markerLine3Spacing; x += markerLine3Spacing)
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

	Vec3 basePos = position;
	basePos.y = 0;

	if (Util::Chance(50))
	{
		for (float x = -lineAttackSpacing; x <= lineAttackSpacing; x += lineAttackSpacing)
		{
			
			CastLine(basePos + front * 20.f + side * x, atan2f(front.x, front.z));
		}
	}
	else
	{
		for (float z = -lineAttackSpacing; z <= lineAttackSpacing; z += lineAttackSpacing)
		{
			CastLine(basePos + front * (20.f + z), atan2f(front.x, front.z) + PIf / 2);
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
