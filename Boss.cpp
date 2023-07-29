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

	rotation = Quaternion(Vec3(0, 1, 0), 0);

	col.scale = { 2.5f, 2.5f, 2.5f };
	col.pos = position;
	col.rot = rotation;
}

void Boss::Update()
{
	static float rotY;

	moveTimer++;

	SpImGui::Command([&] {
		if (ImGui::Begin("Boss"))
		{
			ImGui::InputFloat("Marker Line 3 Spacing", &markerLine3Spacing);
			ImGui::InputFloat("Line Attack Spacing", &lineAttackSpacing);
		}
		ImGui::End();
	});

	//当たり判定を更新
	col.pos = position;
	col.rot = rotation;

	col.DrawBB(Color::Red);

	//Stateに合わせたアップデート
	void (Boss::*pFuncTable[])() = {
		&Boss::IdleUpdate,
		&Boss::MarkerUpdate,
		&Boss::LineAttackUpdate,
		&Boss::MarkerAndLineUpdate,
		&Boss::RushUpdate
	};

	(this->*pFuncTable[(int32_t)state])();

	//ダメージ演出の更新
	if (damaged)
	{
		damageTimer++;
		if (damageTimer > damageTime)
		{
			damaged = false;
		}

		*brightnessCB.contents = Color::Red.f4;
	}
	else
	{
		*brightnessCB.contents = Color::White.f4;
	}

	UpdateMarkers();
	UpdateLineAttacks();

	if (dealDamageOnHit)
	{
		if (col.Collide(Player::Get()->GetCollider()))
		{
			Player::Get()->Damage();
		}
	}

	UpdateMatrix();
}

void Boss::Draw()
{
	Object3D::Draw("white");
	DrawMarkers();
	DrawLineAttacks();
}

void Boss::Damage()
{
	damageTimer = 0;
	damaged = true;
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
	Vec3 target = Player::Get()->position;
	target.y = position.y;

	rotation = Quaternion::DirToDir(Vec3(0, 0, 1), target - position);

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

void Boss::DrawLineAttacks()
{
	for (auto& la : lineAttacks) la.Draw();
}

void Boss::Rush()
{
	state = State::Rush;
	moveTime = prepTime + afterPrepWaitTime + rushTime + rushAfterTime;
	moveTimer = 0;
}

void Boss::RushUpdate()
{
	if (moveTimer < prepTime)
	{
		Vec3 target = position;
		target.y = 2.5f;
		position = Vec3::Lerp(position, target, (float)moveTimer / (float)prepTime);

		Vec3 rotTarget = Player::Get()->position;
		rotTarget.y = position.y;

		rotation = Quaternion::DirToDir(Vec3(0, 0, 1), rotTarget - position);
	}
	else if (moveTimer < prepTime + afterPrepWaitTime)
	{
		Vec3 front = rotation.GetRotMat().ExtractAxisZ();
		front.SetLength(rushDistance);

		rushTarget = front + position;
	}
	else if (moveTimer < prepTime + afterPrepWaitTime + rushTime)
	{
		dealDamageOnHit = true;
		rotation *= Quaternion(Vec3(0, 0, 1), PIf / 60.f);

		position = Vec3::Lerp(position, rushTarget,
			(float)(moveTimer - prepTime - afterPrepWaitTime) / (float)rushTime);
	}
	else
	{
		Vec3 target = position;
		target.y = 5.f;
		position = Vec3::Lerp(position, target, 
			(float)(moveTimer - prepTime - afterPrepWaitTime - rushTime)/ (float)rushAfterTime);
	}

	if (moveTimer >= moveTime)
	{
		RushEnd();
	}
}

void Boss::LineAttackUpdate()
{
	if (moveTimer >= moveTime)
	{
		state = State::Idle;
		moveTimer = 0;
	}
}

void Boss::MarkerUpdate()
{
	if (moveTimer >= moveTime)
	{
		state = State::Idle;
		moveTimer = 0;
	}
}

void Boss::MarkerAndLineUpdate()
{
	if (moveTimer >= moveTime)
	{
		state = State::Idle;
		moveTimer = 0;
	}
}

void Boss::RushEnd()
{
	state = State::Idle;
	moveTimer = 0;
	dealDamageOnHit = false;
}

void Boss::IdleUpdate()
{
	if (timesAttacked < 3)
	{
		if (moveTimer > 60)
		{
			SelectMove();
			timesAttacked++;
		}
	}
	else
	{
		if (moveTimer > 240)
		{
			SelectMove();
			timesAttacked = 0;
		}
	}
}

void Boss::SelectMove()
{
	int32_t rng = Util::RNG(0, 6);
	if (rng <= 1)
	{
		CastLineTriple();
		state = State::Line;
		moveTime = 60;
		moveTimer = 0;
	}
	else if (rng <= 3)
	{
		if (Util::Chance(50))
		{
			CastMarkerLine3();
		}
		else
		{
			CastMarkerAim1Rand5();
		}

		state = State::Marker;
		moveTime = 60;
		moveTimer = 0;
	}
	else if (rng <= 5)
	{
		Rush();
	}
	else
	{
		if (Util::Chance(50))
		{
			CastMarkerLine3();
		}
		else
		{
			CastMarkerAim1Rand5();
		}
		CastLineTriple();

		state = State::MarkerAndLine;
		moveTime = 60;
		moveTimer = 0;
	}
}

const OBBCollider& Boss::GetCollider()
{
	return col;
}
