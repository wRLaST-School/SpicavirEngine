#include "stdafx.h"
#include "Player.h"
#include <Input.h>
#include <CameraController.h>
#include <Boss.h>
#include <SpEffekseer.h>
#include <SpImGui.h>
#include <SoundManager.h>
#include <MainTimer.h>
#include <GameManager.h>

void Player::Load()
{
	ModelManager::Register("Cube", "cube");

	ModelManager::Register("Assets/Models/Player/Idle.gltf", "PlayerIdle", false);
	ModelManager::Register("Assets/Models/Player/Roll.gltf", "PlayerRoll", false);
	ModelManager::Register("Assets/Models/Player/Run.gltf", "PlayerRun", false);
	ModelManager::Register("Assets/Models/Player/Slash1.gltf", "PlayerSlash1", false);
	ModelManager::Register("Assets/Models/Player/Slash2.gltf", "PlayerSlash2", false);
	ModelManager::Register("Assets/Models/Player/Slash3.gltf", "PlayerSlash3", false);

	SpEffekseer::Load(L"Assets/Effekseer/Slash1", L"Assets/Effekseer/Slash1/Slash1.efk", "Slash1");
	SpEffekseer::Load(L"Assets/Effekseer/Slash2", L"Assets/Effekseer/Slash2/Slash2.efk", "Slash2");
	SpEffekseer::Load(L"Assets/Effekseer/Slash3", L"Assets/Effekseer/Slash3/Slash3.efk", "Slash3");
}

void Player::Init()
{
	AddComponent<Object3D>("Object3D");
	obj_ = GetComponent<Object3D>("Object3D");

	obj_->model = ModelManager::GetModel("PlayerIdle");
	obj_->position = { 0, 1, -5 };
	obj_->scale = { 0.5, 1.0, 0.5 };

	counterEmitter_.active = false;
	counterEmitter_.emitOnce = true;

	counterEmitter_.quantity = 15;
	counterEmitter_.radius = Vec3(0.2f, 0.2f, 0.2f);
}

void Player::Update()
{
	counterEmitter_.Update();

	if (Input::Key::Triggered(DIK_R) || Input::Pad::Triggered(Button::RStickButton))
	{
		CameraController::Get()->ToggleMode();
	}
	if (CameraController::Get()->GetMode() == CameraController::Mode::Target)
	{
		//Vec3 front = rotation.GetRotMat().ExtractAxisZ();
		////front.y = 0;
		//front.Norm();

		//Vec3 to = ((Vec3)Boss::Get()->position - position);
		//to.y = 0;
		//to.Norm();

		//rotation = Quaternion::DirToDir(Vec3(0, 0, 1), to);
	}
	else
	{
		//フリーカメラ時の処理
	}
	DamageUpdate();

	switch (state_)
	{
	case Player::State::Idle:
	case Player::State::Move:
		IdleMoveUpdate();
		break;
	case Player::State::Dodge:
		DodgeUpdate();
		break;
	case Player::State::Slash1:
		SlashUpdate1();
		break;
	case Player::State::Slash2:
		SlashUpdate2();
		break;
	case Player::State::Slash3:
		SlashUpdate3();
		break;
	default:
		break;
	}

	if (state_ == State::Idle)
	{
		obj_->model = ModelManager::GetModel("PlayerIdle");
	}
	else if (state_ == State::Move)
	{
		obj_->model = ModelManager::GetModel("PlayerRun");
	}

	obj_->model->UpdateAnim();

	obj_->scale = { 1.0f / 16.f ,1.0f / 16.f ,1.0f / 16.f };

	ClampPos();

	obj_->UpdateMatrix();

	col_.pos = (Vec3)obj_->position + Vec3(0, 1.f, 0);
	col_.scale = { .5f, 1.f, .5f };
	col_.rot = obj_->rotation;
}

void Player::DrawParams()
{
	ImGui::DragFloat3("SlashColScale", &slashScale_.x);
}

void Player::Move()
{
	Vec3 vel(0, 0, 0);
	//Matrix rMat = rotation.GetRotMat();
	CameraController* ctrl = CameraController::Get();
	Vec3 front = (Vec3)ctrl->GetCamera().target - ctrl->GetCamera().position;
	Vec3 side = front.Cross(Vec3(0.f, 1.f, 0.f));
	front.y = 0;
	side.y = 0;

	vel = front.SetLength((float)(Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S)));
	vel += -side.SetLength((float)(Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A)));

	front = (Vec3)ctrl->GetCamera().target - ctrl->GetCamera().position;
	side = front.Cross(Vec3(0.f, 1.f, 0.f));

	vel += -side.SetLength(Input::Pad::GetLStick().x);
	vel += front.SetLength(Input::Pad::GetLStick().y);

	vel.y = 0;

	if (vel.GetSquaredLength())
	{	
		state_ = State::Move;
		obj_->rotation = Quaternion::DirToDir(Vec3(0, 0, -1), vel);
		vel *= spd_;
	}
	else
	{
		state_ = State::Idle;
	}

	obj_->position += vel;
}

void Player::GravMove()
{
	if (obj_->position.y > 0.f)
	{
		vy_ -= GRAV;
	}

	obj_->position.y += vy_;

	if (obj_->position.y < 0.f)
	{
		vy_ = 0;
		obj_->position.y = 0.f;
	}
}

void Player::ClampPos()
{
	float colR = ((Vec3)col_.scale).GetLength();
	obj_->position.x = Util::Clamp(obj_->position.x, -30.f + colR, 30.f - colR);
	obj_->position.z = Util::Clamp(obj_->position.z, -30.f + colR, 30.f - colR);
}

void Player::MoveTo(Float3 newPos)
{
	obj_->position = newPos;

	ClampPos();

	col_.pos = (Vec3)obj_->position + Vec3(0, 1.f, 0);
}

void Player::DamageUpdate()
{
	damageTimer_--;
	damageTimer_ = std::max(0, damageTimer_);

	dodgeSucceededTimer_--;
	dodgeSucceededTimer_ = std::max(0, dodgeSucceededTimer_);

	if (dodgeSucceededTimer_)
	{
		*obj_->brightnessCB.contents = { 1.f, 1.f, 0.f, .25f };
	}
	else if (damageTimer_)
	{
		*obj_->brightnessCB.contents = { 1.f, 0.f, 0.f, .25f };
	}
	else
	{
		*obj_->brightnessCB.contents = { 1.f, 1.f, 1.f, 1.f };
	}
}

void Player::Damage()
{
	if (damageTimer_ == 0 && dodgeSucceededTimer_ == 0)
	{
		if (state_ != State::Dodge)
		{
			if (!GameManager::sDebugImmunity)
			{
				damageTimer_ = 60;
				//SE再生
				SoundManager::Play("takeDamage");

				MainTimer::timerSec -= 5;
				MainTimer::Damage();
			}
		}
		else
		{
			dodgeSucceededTimer_ = 30;
			counterEmitter_.Activate();
			//SE再生
			SoundManager::Play("counterSuccess");
		}
	}
}

void Player::Draw()
{
	if(obj_->brightnessCB.contents->w < 1.0f)
		obj_->DrawAlpha("white");
	else
	{
		obj_->Draw();
	}

	counterEmitter_.Draw();
	col_.DrawBB();
}

void Player::DodgeUpdate()
{
	obj_->position += dodgeVec_;

	obj_->rotation = Quaternion::DirToDir(Vec3(0, 0, -1), dodgeVec_.GetNorm());

	dodgeTimer_++;

	if (dodgeTimer_ >= iFrame)
	{
		state_ = State::Idle;
	}
}

void Player::IdleMoveUpdate()
{
	Move();
	
	if (obj_->position.y == 0.f && (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A)))
	{
		vy_ = JUMP_POWER;
	}
	GravMove();

	if (Input::Pad::Triggered(Button::L) ||
		Input::Pad::Triggered(Button::R) ||
		Input::Pad::Triggered(Trigger::Left) ||
		Input::Pad::Triggered(Trigger::Right) ||
		Input::Key::Triggered(DIK_LCONTROL))
	{
		Dodge();
	}

	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::X))
	{
		Slash1();
	}
}

void Player::Dodge()
{
	CameraController* ctrl = CameraController::Get();
	dodgeStartCameraPos_ = ctrl->GetCamera().position;
	Vec3 front = (Vec3)ctrl->GetCamera().target - ctrl->GetCamera().position;
	Vec3 side = front.Cross(Vec3(0.f, 1.f, 0.f));
	front.y = 0;
	side.y = 0;

	if (((Vec2)Input::Pad::GetLStick() + Vec2((float)Input::Key::Down(DIK_D) - (float)Input::Key::Down(DIK_A), (float)Input::Key::Down(DIK_W) - (float)Input::Key::Down(DIK_S))).GetSquaredLength())
	{
		Vec2 input = Input::Pad::GetLStick();
		input += Vec2((float)Input::Key::Down(DIK_D) - (float)Input::Key::Down(DIK_A), (float)Input::Key::Down(DIK_W) - (float)Input::Key::Down(DIK_S));
		Vec3 v;
		v = front * input.y;
		v += side * (-input.x);
		v.SetLength(dodgeSpd_);
		dodgeVec_ = v;
	}
	else
	{
		front.SetLength(dodgeSpd_);
		dodgeVec_ = front;
	}

	dodgeEndPlayerPos_ = (Vec3)obj_->position + (dodgeVec_ * (float)iFrame);

	state_ = State::Dodge;
	dodgeTimer_ = 0;

	obj_->model = ModelManager::GetModel("PlayerRoll");
	obj_->model->ResetAnimTimer();
	obj_->model->aniSpeed = 3.8f;
	//SE再生
	SoundManager::Play("dodge");
}

void Player::SlashUpdate1()
{
	slashTimer_++;

	slashCol_.DrawBB(Color::Blue);

	Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
	front.Norm();

	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::X))
	{
		slashRegistered_ = true;
	}

	if (Input::Pad::Triggered(Button::L) ||
		Input::Pad::Triggered(Button::R) ||
		Input::Pad::Triggered(Trigger::Left) ||
		Input::Pad::Triggered(Trigger::Right) ||
		Input::Key::Triggered(DIK_LCONTROL))
	{
		slashTimer_ = 0;
		Dodge();
	}

	//当たり判定をチェック
	Boss* boss = Boss::Get();
	if (!slashHit_ && slashCol_.Collide(boss->GetCollider()))
	{
		boss->Damage(slashDamage_);
		slashHit_ = true;
	}

	//タイマーが超過していたら
	if (slashTimer_ > slashTime)
	{
		slashTimer_ = 0;
		if (slashRegistered_) //先行入力チェック
		{//あるなら発動
			Slash2();
			slashRegistered_ = false;
		}
		else
		{//ないならIdleに
			state_ = State::Idle;
		}
	}
}

void Player::SlashUpdate2()
{
	slashTimer_++;

	slashCol_.DrawBB(Color::Blue);

	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::X))
	{
		slashRegistered_ = true;
	}

	if (Input::Pad::Triggered(Button::L) ||
		Input::Pad::Triggered(Button::R) ||
		Input::Pad::Triggered(Trigger::Left) ||
		Input::Pad::Triggered(Trigger::Right) || 
		Input::Key::Triggered(DIK_LCONTROL))
	{
		slashTimer_ = 0;
		Dodge();
	}

	//当たり判定をチェック
	Boss* boss = Boss::Get();
	if (!slashHit_ && slashCol_.Collide(boss->GetCollider()))
	{
		boss->Damage(slashDamage_);
		slashHit_ = true;
	}

	//タイマーが超過していたら
	if (slashTimer_ > slashTime)
	{
		slashTimer_ = 0;
		if (slashRegistered_)//先行入力チェック
		{//あるなら発動
			Slash3();
			slashRegistered_ = false;
		}
		else
		{//ないならIdleに
			state_ = State::Idle;
		}
	}
}

void Player::SlashUpdate3()
{
	slashTimer_++;

	slashCol_.DrawBB(Color::Blue);

	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::X))
	{
		slashRegistered_ = true;
	}

	if (Input::Pad::Triggered(Button::L) ||
		Input::Pad::Triggered(Button::R) ||
		Input::Pad::Triggered(Trigger::Left) ||
		Input::Pad::Triggered(Trigger::Right) || 
		Input::Key::Triggered(DIK_LCONTROL))
	{
		slashTimer_ = 0;
		Dodge();
	}

	//当たり判定をチェック
	Boss* boss = Boss::Get();
	if (!slashHit_ && slashCol_.Collide(boss->GetCollider()))
	{
		boss->Damage(slash3Damage_);
		slashHit_ = true;
	}

	//タイマーが超過していたら
	if (slashTimer_ > slash3Time)
	{
		slashTimer_ = 0;
		if (slashRegistered_)//先行入力チェック
		{//あっても一旦Idleにしよう
			state_ = State::Idle;
			slashRegistered_ = false;
		}
		else
		{//ないならIdleに
			state_ = State::Idle;
		}
	}
}

void Player::Slash1()
{
	//正面の角度を計算
	Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	/*Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();*/
	float angle = atan2f(front.x, front.z);

	SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("Slash1", obj_->position), { 0, 1, 0 }, angle + PIf);

	obj_->model = ModelManager::GetModel("PlayerSlash2");
	obj_->model->ResetAnimTimer();
	obj_->model->aniSpeed = 1.5f;

	state_ = State::Slash1;

	//当たり判定の設定
	slashCol_.pos = front * -slashDist + obj_->position + Vec3(0, 1, 0);
	slashCol_.rot = Quaternion(Vec3(0, 1, 0), angle);

	//既に当たっているフラグをリセット
	slashHit_ = false;
	//SE再生
	SoundManager::Play("Slash12");
}

void Player::Slash2()
{
	//正面の角度を計算
	Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	/*Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();*/
	float angle = atan2f(front.x, front.z) + PIf;

	SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("Slash2", obj_->position), { 0, 1, 0 }, angle);

	obj_->model = ModelManager::GetModel("PlayerSlash1");
	obj_->model->ResetAnimTimer();
	obj_->model->aniSpeed = 1.5f;

	state_ = State::Slash2;

	//当たり判定の設定
	slashCol_.pos = front * -slashDist + obj_->position + Vec3(0, 1, 0);
	slashCol_.rot = Quaternion(Vec3(0, 1, 0), angle);

	slashCol_.scale = slashScale_;

	//既に当たっているフラグをリセット
	slashHit_ = false;
	//SE再生
	SoundManager::Play("Slash12");
}

void Player::Slash3()
{
	//正面の角度を計算
	Vec3 front = obj_->rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	/*Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();*/
	float angle = atan2f(front.x, front.z) + PIf;

	SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("Slash3", obj_->position), { 0, 1, 0 }, angle);

	obj_->model = ModelManager::GetModel("PlayerSlash3");
	obj_->model->ResetAnimTimer();
	obj_->model->aniSpeed = 1.5f;

	state_ = State::Slash3;

	//当たり判定の設定
	slashCol_.pos = front * -slashDist + obj_->position + Vec3(0, 1, 0);
	slashCol_.rot = Quaternion(Vec3(0, 1, 0), angle);

	//既に当たっているフラグをリセット
	slashHit_ = false;

	//SE再生
	SoundManager::Play("Slash3");
}

const OBBCollider& Player::GetCollider()
{
	return col_;
}

Player::State Player::GetState()
{
	return state_;
}

Player::DodgeData Player::GetDodgeData()
{
	return { dodgeTimer_, iFrame, dodgeStartCameraPos_, dodgeEndPlayerPos_ };
}

Player* Player::Get()
{
	return sCurrent;
}

void Player::Set(Player* player)
{
	sCurrent = player;
}

Player* Player::sCurrent = nullptr;