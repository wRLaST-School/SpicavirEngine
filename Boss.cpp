#include "stdafx.h"
#include "Boss.h"
#include <Util.h>
#include <Input.h>

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
	/*if(Input::Key::Triggered(DIK_M)) */CastMarker({ (float)Util::RNG(-20, 20), 0.001f, (float)(Util::RNG(-20, 20)) });
	UpdateMarkers();
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

void Boss::DrawMarkers()
{
	for (auto& m : markers) if (m.active) m.Draw();
}

void Boss::UpdateMarkers()
{
	for (auto& m : markers) if (m.active) m.Update();
}
