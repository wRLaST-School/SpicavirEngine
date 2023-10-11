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
	ModelManager::Register("Assets/Models/Boss/boss.glb", "Boss", true);
	SpTextureManager::LoadTexture("Assets/Images/white.png", "white");

	Score::Load();
}

void Boss::Init()
{
	model = ModelManager::GetModel("Boss");

	InitBehaviorTree();

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
		ShowImGui();
	}

	//当たり判定を更新
	col_.pos = position;
	col_.rot = rotation;

	col_.DrawBB(Color::Red);

	//BehaviorTreeのTickを行う
	tree_->Tick();

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

	if (gravSphere_ != nullptr)
	{
		gravSphere_->Draw();
	}

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

void Boss::InitBehaviorTree()
{
	//BTのコンポーネントを追加
	AddComponent<BT::BehaviorTree>("BehaviorTree");
	tree_ = GetComponent<BT::BehaviorTree>("BehaviorTree");

	//ファクトリーに関数を登録
	BT::BehaviorTreeFactory factory;

	factory.RegisterAction("CastMarker1", std::bind(&Boss::CastMarkerAim1Rand5, this));
	factory.RegisterAction("CastMarker2", std::bind(&Boss::CastMarkerLine3, this));
	factory.RegisterAction("CastLineAttack", std::bind(&Boss::CastLineTriple, this));
	factory.RegisterAction("CastRush", std::bind(&Boss::Rush, this));
	factory.RegisterAction("CastGravSphere", std::bind(&Boss::CastGravSphere, this));

	factory.RegisterAction("UpdateRush", std::bind(&Boss::RushUpdate, this));
	factory.RegisterAction("UpdateLine", std::bind(&Boss::LineAttackUpdate, this));
	factory.RegisterAction("UpdateMarker", std::bind(&Boss::MarkerUpdate, this));
	factory.RegisterAction("UpdateMarkerAndLine", std::bind(&Boss::MarkerAndLineUpdate, this));
	factory.RegisterAction("UpdateGravSphere", std::bind(&Boss::GravSphereUpdate, this));
	factory.RegisterAction("Wait60Frame", std::bind(&Boss::Wait60Frame, this));

	factory.RegisterCondition("PlayerInsideRushRange", std::bind(&Boss::IsPlayerInsideRushRange, this));

	tree_->SetFactory(factory);

	tree_->LoadJson("Assets/data/BossBehavior.bt");
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

BT::Status Boss::CastMarkerAim1Rand5()
{
	CastMarker({Player::Get()->position.x, 0, Player::Get()->position.z});

	for (size_t i = 0; i < 5; i++)
	{
		CastMarker({ (float)Util::RNG(-30, 30), 0.001f, (float)(Util::RNG(-30, 30)) });
	}

	//SE再生
	SoundManager::Play("marker");

	state_ = State::Marker;
	moveTime_ = 60;
	moveTimer_ = 0;

	return BT::Status::Success;
}

BT::Status Boss::CastMarkerLine3()
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

	state_ = State::Marker;
	moveTime_ = 60;
	moveTimer_ = 0;

	return BT::Status::Success;
}

void Boss::DrawMarkers()
{
	for (auto& m : markers_) if (m.IsActive()) m.Draw();
}

void Boss::UpdateMarkers()
{
	for (auto& m : markers_) if (m.IsActive()) m.Update();
}

BT::Status Boss::CastLineTriple()
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

	state_ = State::Line;
	moveTime_ = 60;
	moveTimer_ = 0;

	return BT::Status::Success;
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

BT::Status Boss::Rush()
{
	state_ = State::Rush;
	moveTime_ = prepTime_ + afterPrepWaitTime_ + rushTime_ + rushAfterTime_;
	moveTimer_ = 0;

	return BT::Status::Success;
}

BT::Status Boss::RushUpdate()
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
		return BT::Status::Completed;
	}

	if (moveTimer_ == prepTime_ + afterPrepWaitTime_)
	{
		//SE再生
		SoundManager::Play("RushImpact");
	}

	return BT::Status::Running;
}

BT::Status Boss::LineAttackUpdate()
{
	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;

		return BT::Status::Completed;
	}

	return BT::Status::Running;
}

BT::Status Boss::MarkerUpdate()
{
	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;

		return BT::Status::Completed;
	}

	return BT::Status::Running;
}

BT::Status Boss::MarkerAndLineUpdate()
{
	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;

		return BT::Status::Completed;
	}

	return BT::Status::Running;
}

BT::Status Boss::GravSphereUpdate()
{
	gravSphere_->Update();

	if (moveTimer_ >= moveTime_)
	{
		state_ = State::Idle;
		moveTimer_ = 0;
		GravSphereEnd();

		return BT::Status::Completed;
	}

	return BT::Status::Running;
}

BT::Status Boss::Wait60Frame()
{
	if (moveTimer_ >= 60)
	{
		state_ = State::Idle;
		moveTimer_ = 0;

		return BT::Status::Completed;
	}

	return BT::Status::Running;
}

bool Boss::IsPlayerInsideRushRange()
{
	Vec3 bPos = position;
	Vec3 pPos = Player::Get()->position;
	bPos.y = 0;
	pPos.y = 0;

	return (pPos - bPos).GetSquaredLength() < rushDistance_ * rushDistance_;
}

void Boss::RushEnd()
{
	state_ = State::Idle;
	moveTimer_ = 0;
	dealDamageOnHit_ = false;
}

BT::Status Boss::CastGravSphere()
{
	state_ = State::GravSphere;
	moveTime_ = gravSphereTime_;
	moveTimer_ = 0;

	//プレイヤーの方向を向く
	Vec3 target = Player::Get()->position;
	target.y = position.y;

	rotation = Quaternion::DirToDir(Vec3(0, 0, 1), target - position);

	//プレイヤー側に1mの位置に生成
	Vec3 front = rotation.GetRotMat().ExtractAxisZ();
	front.SetLength(3.f);

	//重力弾を生成
	gravSphere_ = std::make_unique<GravSphere>((Vec3)position + front,
		((Vec3)Player::Get()->position - position).GetNorm(),
		0.2f,
		gravR_,
		gravitySpeed_,
		maxHomeRad_,
		gravSphereStayTime_
	);

	return BT::Status::Success;
}

void Boss::GravSphereEnd()
{
	gravSphere_.release();
	gravSphere_ = nullptr;
}

void Boss::SelectMove()
{ 
	int32_t rng = Util::RNG(0, 9);
	if (rng <= 1)
	{
		CastLineTriple();
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
	}
	else if (rng <= 5)
	{
		Rush();
	}
	else if (rng <= 8)
	{
		CastGravSphere();
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

void Boss::ShowImGui()
{
	SpImGui::Command([&] {
		if (ImGui::Begin("Boss", nullptr, ImGuiWindowFlags_MenuBar))
		{
			//セーブとロード
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save")) {
						Util::SerializeData("Assets/Data/boss.bin", *this);
					}
					if (ImGui::MenuItem("Load")) {
						Util::DeserializeData("Assets/Data/boss.bin", *this);
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			//ステータス関連
			if (ImGui::TreeNode("Basic Stats"))
			{

				ImGui::TreePop();
			}

			ImGui::Separator();

			//マーカー攻撃
			if (ImGui::TreeNode("Marker Attack"))
			{
				ImGui::InputFloat("Marker Line 3 Spacing", &markerLine3Spacing_);

				ImGui::TreePop();
			}

			ImGui::Separator();

			//直線攻撃
			if (ImGui::TreeNode("Line Attack"))
			{
				ImGui::InputFloat("Line Attack Spacing", &lineAttackSpacing_);

				ImGui::TreePop();
			}

			ImGui::Separator();

			//突進攻撃
			if (ImGui::TreeNode("Rush"))
			{
				ImGui::InputInt("Prepare Time", &prepTime_);
				ImGui::InputInt("After Prepare Wait Time", &afterPrepWaitTime_);
				ImGui::InputInt("Rush Time", &rushTime_);
				ImGui::InputInt("After Rush Wait Time", &rushAfterTime_);
				ImGui::InputFloat("Rush Distance", &rushDistance_);

				ImGui::TreePop();
			}

			ImGui::Separator();

			//重力弾攻撃
			if (ImGui::TreeNode("Gravity Sphere"))
			{
				ImGui::InputFloat("Gravity Sphere Speed", &gravSphereSpd_);

				ImGui::InputFloat("Gravity Affect Range", &gravR_);

				ImGui::InputFloat("Gravity Power", &gravitySpeed_);

				ImGui::InputFloat("Max Homing Radian per Frame", &maxHomeRad_);

				ImGui::InputInt("Sphere Generate Time", &gravSphereStayTime_);

				ImGui::TreePop();
			}

			ImGui::Separator();
			
			ImGui::DragInt("Damage", &GameManager::sScore.totDamage);
		}
		ImGui::End();
		});
}
