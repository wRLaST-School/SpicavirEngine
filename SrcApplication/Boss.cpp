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
	AddComponent<Object3D>("Object3D");

	obj_ = GetComponent<Object3D>("Object3D");

	obj_->model = ModelManager::GetModel("Boss");

	InitBehaviorTree();

	obj_->position = { 0.f, 5.f, 0.f };

	obj_->rotation = Quaternion(Vec3(0, 1, 0), 0);

	col_.scale = { 2.5f, 2.5f, 2.5f };
	col_.pos = obj_->position;
	col_.rot = obj_->rotation;

	GameManager::sScore.Init();
}

void Boss::Update()
{
	static float rotY;

	moveTimer_++;

	//当たり判定を更新
	col_.pos = obj_->position;
	col_.rot = obj_->rotation;

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

		*obj_->brightnessCB.contents = Color::Red.f4;
	}
	else
	{
		*obj_->brightnessCB.contents = Color::White.f4;
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
	obj_->position.x = Util::Clamp(obj_->position.x, -30.f + colR, 30.f - colR);
	obj_->position.z = Util::Clamp(obj_->position.z, -30.f + colR, 30.f - colR);

	obj_->UpdateMatrix();

	GameManager::sScore.Update();

	if (SceneManager::GetScene()->GetComponent<MainTimer>("MainTimer")->timerSec <= 0 || GameManager::sScore.totDamage >= GameManager::sScore.gradeMax)
	{
		Transition::Start<ResultScene>();
	}
}

void Boss::Draw()
{
	obj_->Draw("white");
	DrawMarkers();
	DrawLineAttacks();

	if (gravSphere_ != nullptr)
	{
		gravSphere_->Draw();
	}

	GameManager::sScore.Draw();
	if (GameManager::sShowDebug)
	{
		ShowImGui();
	}
}

void Boss::Damage(int32_t damage)
{
	damageTimer_ = 0;
	damaged_ = true;

	GameManager::sScore.totDamage += damage;
}

void Boss::DrawParams()
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

Boss* Boss::Get()
{
	return sCurrent;
}

void Boss::Set(Boss* boss)
{
	sCurrent = boss;
}

Boss::Boss()
{
	Set(this);
}

void Boss::InitBehaviorTree()
{
	//BTのコンポーネントを追加
	AddComponent<BT::BehaviorTree>("BT::BehaviorTree");
	tree_ = GetComponent<BT::BehaviorTree>("BT::BehaviorTree");

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
	AddComponent<Marker>("Marker")->Init(pos);
}

BT::Status Boss::CastMarkerAim1Rand5()
{
	CastMarker({Player::Get()->obj_->position.x, 0, Player::Get()->obj_->position.z});

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
	auto markers = GetComponents<Marker>("Marker");
	for (auto& m : markers) if (m->IsActive()) m->Draw();
}

void Boss::UpdateMarkers()
{
	auto markers = GetComponents<Marker>("Marker");
	for (auto& m : markers) {
		if (!m->IsActive())
		{
			RemoveComponent(m);
		}
	}
}

BT::Status Boss::CastLineTriple()
{
	Vec3 target = Player::Get()->obj_->position;
	target.y = obj_->position.y;

	obj_->rotation = Quaternion::DirToDir(Vec3(0, 0, 1), target - obj_->position);

	Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	Vec3 side = obj_->rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();

	Vec3 basePos = obj_->position;
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
	AddComponent<LineAttack>("LineAttack")->Init(pos, angle);
}

void Boss::UpdateLineAttacks()
{
	auto lineAttacks = GetComponents<LineAttack>("LineAttack");
	for (auto itr = lineAttacks.begin(); itr != lineAttacks.end(); itr++)
	{
		if (!(*itr)->IsActive())
		{
			RemoveComponent(*itr);
		}
	}
}

void Boss::DrawLineAttacks()
{
	auto lineAttacks = GetComponents<LineAttack>("LineAttack");
	for (auto& la : lineAttacks) la->Draw();
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
		Vec3 target = obj_->position;
		target.y = 2.5f;
		obj_->position = Vec3::Lerp(obj_->position, target, (float)moveTimer_ / (float)prepTime_);

		Vec3 rotTarget = Player::Get()->obj_->position;
		rotTarget.y = obj_->position.y;

		obj_->rotation = Quaternion::DirToDir(Vec3(0, 0, 1), rotTarget - obj_->position);
	}
	else if (moveTimer_ < prepTime_ + afterPrepWaitTime_)
	{
		Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
		front.SetLength(rushDistance_);

		rushTarget_ = front + obj_->position;
	}
	else if (moveTimer_ < prepTime_ + afterPrepWaitTime_ + rushTime_)
	{
		dealDamageOnHit_ = true;
		obj_->rotation *= Quaternion(Vec3(0, 0, 1), PIf / 60.f);

		obj_->position = Vec3::Lerp(obj_->position, rushTarget_,
			(float)(moveTimer_ - prepTime_ - afterPrepWaitTime_) / (float)rushTime_);
	}
	else
	{
		Vec3 target = obj_->position;
		target.y = 5.f;
		obj_->position = Vec3::Lerp(obj_->position, target,
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
	Vec3 bPos = obj_->position;
	Vec3 pPos = Player::Get()->obj_->position;
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
	Vec3 target = Player::Get()->obj_->position;
	target.y = obj_->position.y;

	obj_->rotation = Quaternion::DirToDir(Vec3(0, 0, 1), target - obj_->position);

	//プレイヤー側に1mの位置に生成
	Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
	front.SetLength(3.f);

	//重力弾を生成
	gravSphere_ = AddComponent<GravSphere>( "GravSphere");

	gravSphere_->Init(
		(Vec3)obj_->position + front,
		((Vec3)Player::Get()->obj_->position - obj_->position).GetNorm(),
		0.2f,
		gravR_,
		gravitySpeed_,
		maxHomeRad_,
		gravSphereStayTime_);

	return BT::Status::Success;
}

void Boss::GravSphereEnd()
{
	RemoveComponent(gravSphere_);
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
}
