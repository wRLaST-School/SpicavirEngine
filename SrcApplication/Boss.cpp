#include "stdafx.h"
#include "Boss.h"
#include <Util.h>
#include <Input.h>
#include <GlobalTimer.h>
#include <Player.h>
#include <SpImGui.h>
#include <SoundManager.h>
#include <MainTimer.h>
#include <SceneManager.h>
#include <Transition.h>
#include <ResulScene.h>
#include <GameManager.h>

Boss* Boss::sCurrent = nullptr;

void Boss::Load()
{
	ModelManager::Register("Resources/Models/Boss/boss.glb", "Boss", true);
	SpTextureManager::LoadTexture("Resources/white.png", "white");

	ModelManager::Register("Resources/Models/Marker/Marker.obj", "Marker", true);
	SpTextureManager::LoadTexture("Resources/Marker.png", "Marker");

	Score::Load();
}

void Boss::Init()
{
	model = ModelManager::GetModel("Boss");
	position = { 0.f, 5.f, 0.f };

	for (auto& m : markers_) m.InitModel();

	rotation = Quaternion(Vec3(0, 1, 0), 0);

	col_.scale = { 2.5f, 2.5f, 2.5f };
	col_.pos = position;
	col_.rot = rotation;

	GameManager::sScore.Init();
}

void Boss::Update()
{
	static float rotY;

	moveTimer_++;

	if (GameManager::sShowDebug)
	{
		SpImGui::Command([&] {
			if (ImGui::Begin("Boss"))
			{
				ImGui::InputFloat("Marker Line 3 Spacing", &markerLine3Spacing_);
				ImGui::InputFloat("Line Attack Spacing", &lineAttackSpacing_);
				ImGui::DragInt("Damage", &GameManager::sScore.totDamage);

				if (ImGui::Button("Save"))
				{
					Util::SerializeData("Resources/Data/boss.bin", *this);
				}

				if (ImGui::Button("Load"))
				{
					Util::DeserializeData("Resources/Data/boss.bin", *this);
				}
			}
		ImGui::End();
			});
	}

	//当たり判定を更新
	col_.pos = position;
	col_.rot = rotation;

	col_.DrawBB(Color::Red);

	//Stateに合わせたアップデート
	void (Boss::*pFuncTable[])() = {
		&Boss::IdleUpdate,
		&Boss::MarkerUpdate,
		&Boss::LineAttackUpdate,
		&Boss::MarkerAndLineUpdate,
		&Boss::RushUpdate
	};

	(this->*pFuncTable[(int32_t)state_])();

	//ダメージ演出の更新
	if (damaged_)
	{
		damageTimer_++;
		if (damageTimer_ > damageTime)
		{
			damaged_ = false;
		}

		*brightnessCB.contents = Color::Red.f4;
	}
	else
	{
		*brightnessCB.contents = Color::White.f4;
	}

	UpdateMarkers();
	UpdateLineAttacks();

	if (dealDamageOnHit_)
	{
		if (col_.Collide(Player::Get()->GetCollider()))
		{
			Player::Get()->Damage();
		}
	}

	//フィールド内に座標をクランプ
	float colR = ((Vec3)col_.scale).GetLength();
	position.x = Util::Clamp(position.x, -30.f + colR, 30.f - colR);
	position.z = Util::Clamp(position.z, -30.f + colR, 30.f - colR);

	UpdateMatrix();

	GameManager::sScore.Update();

	if (MainTimer::timerSec <= 0 || GameManager::sScore.totDamage >= GameManager::sScore.gradeMax)
	{
		Transition::Start<ResultScene>();
	}
}

void Boss::Draw()
{
	Object3D::Draw("white");
	DrawMarkers();
	DrawLineAttacks();

	GameManager::sScore.Draw();
}

void Boss::Damage(int32_t damage)
{
	damageTimer_ = 0;
	damaged_ = true;

	GameManager::sScore.totDamage += damage;
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
	for (auto& m : markers_)
	{
		if (!m.IsActive()) {
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

	//SE再生
	SoundManager::Play("marker");
}

void Boss::CastMarkerLine3()
{
	if (Util::Chance(50))
	{
		for (float z = -markerLine3Spacing_; z <= markerLine3Spacing_; z += markerLine3Spacing_)
		{
			for (float x = -28.f; x < 28.f; x += 4.f) {
				CastMarker({ x, 0.f, z });
			}
		}
	}
	else
	{
		for (float x = -markerLine3Spacing_; x <= markerLine3Spacing_; x += markerLine3Spacing_)
		{
			for (float z = -28.f; z < 28.f; z += 4.f) {
				CastMarker({ x, 0.f, z });
			}
		}
	}

	//SE再生
	SoundManager::Play("marker");
}

void Boss::DrawMarkers()
{
	for (auto& m : markers_) if (m.IsActive()) m.Draw();
}

void Boss::UpdateMarkers()
{
	for (auto& m : markers_) if (m.IsActive()) m.Update();
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
		for (float x = -lineAttackSpacing_; x <= lineAttackSpacing_; x += lineAttackSpacing_)
		{
			
			CastLine(basePos + front * 20.f + side * x, atan2f(front.x, front.z));
		}
	}
	else
	{
		for (float z = -lineAttackSpacing_; z <= lineAttackSpacing_; z += lineAttackSpacing_)
		{
			CastLine(basePos + front * (20.f + z), atan2f(front.x, front.z) + PIf / 2);
		}
	}

	//SE再生
	SoundManager::Play("LineAttack");
}

void Boss::CastLine(Float3 pos, float angle)
{
	lineAttacks_.emplace_back(pos, angle);
}

void Boss::UpdateLineAttacks()
{
	for (auto itr = lineAttacks_.begin(); itr != lineAttacks_.end();)
	{
		itr->Update();

		if (!itr->IsActive())
		{
			itr = lineAttacks_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Boss::DrawLineAttacks()
{
	for (auto& la : lineAttacks_) la.Draw();
}

void Boss::Rush()
{
	state_ = State::Rush;
	moveTime_ = prepTime_ + afterPrepWaitTime_ + rushTime_ + rushAfterTime_;
	moveTimer_ = 0;
}

void Boss::RushUpdate()
{
	if (moveTimer_ < prepTime_)
	{
		Vec3 target = position;
		target.y = 2.5f;
		position = Vec3::Lerp(position, target, (float)moveTimer_ / (float)prepTime_);

		Vec3 rotTarget = Player::Get()->position;
		rotTarget.y = position.y;

		rotation = Quaternion::DirToDir(Vec3(0, 0, 1), rotTarget - position);
	}
	else if (moveTimer_ < prepTime_ + afterPrepWaitTime_)
	{
		Vec3 front = rotation.GetRotMat().ExtractAxisZ();
		front.SetLength(rushDistance_);

		rushTarget_ = front + position;
	}
	else if (moveTimer_ < prepTime_ + afterPrepWaitTime_ + rushTime_)
	{
		dealDamageOnHit_ = true;
		rotation *= Quaternion(Vec3(0, 0, 1), PIf / 60.f);

		position = Vec3::Lerp(position, rushTarget_,
			(float)(moveTimer_ - prepTime_ - afterPrepWaitTime_) / (float)rushTime_);
	}
	else
	{
		Vec3 target = position;
		target.y = 5.f;
		position = Vec3::Lerp(position, target, 
			(float)(moveTimer_ - prepTime_ - afterPrepWaitTime_ - rushTime_)/ (float)rushAfterTime_);
	}

	if (moveTimer_ >= moveTime_)
	{
		RushEnd();
	}

	if (moveTimer_ == prepTime_ + afterPrepWaitTime_)
	{
		//SE再生
		SoundManager::Play("RushImpact");
	}
}

void Boss::LineAttackUpdate()
{
	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;
	}
}

void Boss::MarkerUpdate()
{
	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;
	}
}

void Boss::MarkerAndLineUpdate()
{
	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;
	}
}

void Boss::RushEnd()
{
	state_ = State::Idle;
	moveTimer_ = 0;
	dealDamageOnHit_ = false;
}

void Boss::IdleUpdate()
{
	if (timesAttacked_ < 3)
	{
		if (moveTimer_ > 60)
		{
			SelectMove();
			timesAttacked_++;
		}
	}
	else
	{
		if (moveTimer_ > 240)
		{
			SelectMove();
			timesAttacked_ = 0;
		}
	}
}

void Boss::SelectMove()
{
	int32_t rng = Util::RNG(0, 6);
	if (rng <= 1)
	{
		CastLineTriple();
		state_ = State::Line;
		moveTime_ = 60;
		moveTimer_ = 0;
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

		state_ = State::Marker;
		moveTime_ = 60;
		moveTimer_ = 0;
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

		state_ = State::MarkerAndLine;
		moveTime_ = 60;
		moveTimer_ = 0;
	}
}

const OBBCollider& Boss::GetCollider()
{
	return col_;
}
