#include "stdafx.h"
#include "Player.h"
#include <Input.h>
#include <CameraController.h>
#include <Boss.h>
#include <SpEffekseer.h>

void Player::Load()
{
	ModelManager::Register("Cube", "cube");

	ModelManager::Register("Resources/Models/Player/Idle.gltf", "PlayerIdle", false);
	ModelManager::Register("Resources/Models/Player/Roll.gltf", "PlayerRoll", false);
	ModelManager::Register("Resources/Models/Player/Run.gltf", "PlayerRun", false);
	ModelManager::Register("Resources/Models/Player/Slash1.gltf", "PlayerSlash1", false);
	ModelManager::Register("Resources/Models/Player/Slash2.gltf", "PlayerSlash2", false);
	ModelManager::Register("Resources/Models/Player/Slash3.gltf", "PlayerSlash3", false);

	SpEffekseer::Load(L"Resources/Effekseer/Slash1", L"Resources/Effekseer/Slash1/Slash1.efk", "Slash1");
	SpEffekseer::Load(L"Resources/Effekseer/Slash2", L"Resources/Effekseer/Slash2/Slash2.efk", "Slash2");
	SpEffekseer::Load(L"Resources/Effekseer/Slash3", L"Resources/Effekseer/Slash3/Slash3.efk", "Slash3");
}

void Player::Init()
{
	model = ModelManager::GetModel("PlayerIdle");
	position = { 0, 1, -5 };
	scale = { 0.5, 1.0, 0.5 };

	counterEmitter.active = false;
	counterEmitter.emitOnce = true;

	counterEmitter.quantity = 15;
	counterEmitter.radius = Vec3(0.2f, 0.2f, 0.2f);
}

void Player::Update()
{
	counterEmitter.Update();

	if (Input::Key::Triggered(DIK_R) || Input::Pad::Triggered(Button::R))
	{
		CameraController::Get()->ToggleMode();
	}
	if (CameraController::Get()->GetMode() == CameraController::Mode::Target)
	{
		Vec3 front = rotation.GetRotMat().ExtractAxisZ();
		//front.y = 0;
		front.Norm();

		Vec3 to = ((Vec3)Boss::Get()->position - position);
		to.y = 0;
		to.Norm();

		rotation = Quaternion::DirToDir(Vec3(0, 0, 1), to);
	}
	else
	{
		//ƒtƒŠ[ƒJƒƒ‰Žž‚Ìˆ—
	}
	DamageUpdate();
	
	switch (state)
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

	if (state == State::Idle)
	{
		model = ModelManager::GetModel("PlayerIdle");
	}
	else if (state == State::Move)
	{
		model = ModelManager::GetModel("PlayerRun");
	}
	
	model->UpdateAnim();

	scale = { 1.0f / 16.f ,1.0f / 16.f ,1.0f / 16.f };

	UpdateMatrix();

	col.pos = (Vec3)position + Vec3(0, 1.f, 0);
	col.scale = {.5f, 1.f, .5f};
	col.rot = rotation;
}

void Player::Move()
{
	Vec3 vel(0, 0, 0);
	//Matrix rMat = rotation.GetRotMat();
	CameraController* ctrl = CameraController::Get();
	Vec3 front = (Vec3)ctrl->cam->target - ctrl->cam->position;
	Vec3 side = front.Cross(Vec3(0.f, 1.f, 0.f));
	front.y = 0;
	side.y = 0;

	vel = front.SetLength((float)(Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S)));
	vel += -side.SetLength((float)(Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A)));

	front = (Vec3)ctrl->cam->target - ctrl->cam->position;
	side = front.Cross(Vec3(0.f, 1.f, 0.f));

	vel += -side.SetLength(Input::Pad::GetLStick().x);
	vel += front.SetLength(Input::Pad::GetLStick().y);

	vel.y = 0;

	if (vel.GetSquaredLength())
	{	
		state = State::Move;
		rotation = Quaternion::DirToDir(Vec3(0, 0, -1), vel);
		vel *= spd;
	}
	else
	{
		state = State::Idle;
	}

	position += vel;
}

void Player::GravMove()
{
	if (position.y > 0.f)
	{
		vy -= GRAV;
	}

	position.y += vy;

	if (position.y < 0.f)
	{
		vy = 0;
		position.y = 0.f;
	}
}

void Player::DamageUpdate()
{
	damageTimer--;
	damageTimer = std::max(0, damageTimer);

	dodgeSucceededTimer_--;
	dodgeSucceededTimer_ = std::max(0, dodgeSucceededTimer_);

	if (dodgeSucceededTimer_)
	{
		*brightnessCB.contents = { 1.f, 1.f, 0.f, .25f };
	}
	else if (damageTimer)
	{
		*brightnessCB.contents = { 1.f, 0.f, 0.f, .25f };
	}
	else
	{
		*brightnessCB.contents = { 1.f, 1.f, 1.f, 1.f };
	}
}

void Player::Damage()
{
	if (state != State::Dodge)
	{
		damageTimer = 60;
	}
	else
	{
		dodgeSucceededTimer_ = 30;
		counterEmitter.Activate();
	}
}

void Player::Draw()
{
	if(brightnessCB.contents->w < 1.0f)
		Object3D::DrawAlpha("white");
	else
	{
		Object3D::Draw();
	}

	counterEmitter.Draw();
	col.DrawBB();
}

void Player::DodgeUpdate()
{
	position += dodgeVec_;

	rotation = Quaternion::DirToDir(Vec3(0, 0, -1), dodgeVec_.GetNorm());

	dodgeTimer_++;

	if (dodgeTimer_ >= iFrame)
	{
		state = State::Idle;
	}
}

void Player::IdleMoveUpdate()
{
	Move();
	
	if (position.y == 0.f && (Input::Key::Triggered(DIK_SPACE) || Input::Pad::Triggered(Button::A)))
	{
		vy = JUMP_POWER;
	}
	GravMove();

	if (Input::Pad::Triggered(Button::X) || Input::Key::Triggered(DIK_LCONTROL))
	{
		Dodge();
	}

	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::L))
	{
		Slash1();
	}
}

void Player::Dodge()
{
	CameraController* ctrl = CameraController::Get();
	Vec3 front = (Vec3)ctrl->cam->target - ctrl->cam->position;
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

	state = State::Dodge;
	dodgeTimer_ = 0;

	model = ModelManager::GetModel("PlayerRoll");
	model->ResetAnimTimer();
	model->aniSpeed = 3.8f;
}

void Player::SlashUpdate1()
{
	slashTimer++;

	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::L))
	{
		slashRegistered = true;
	}

	if (Input::Pad::Triggered(Button::X) || Input::Key::Triggered(DIK_LCONTROL))
	{
		slashTimer = 0;
		Dodge();
	}

	if (slashTimer > slashTime)
	{
		slashTimer = 0;
		if (slashRegistered)
		{
			Slash2();
			slashRegistered = false;
		}
		else
		{
			state = State::Idle;
		}
	}
}

void Player::SlashUpdate2()
{
	slashTimer++;
	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::L))
	{
		slashRegistered = true;
	}

	if (Input::Pad::Triggered(Button::X) || Input::Key::Triggered(DIK_LCONTROL))
	{
		slashTimer = 0;
		Dodge();
	}

	if (slashTimer > slashTime)
	{
		slashTimer = 0;
		if (slashRegistered)
		{
			Slash3();
			slashRegistered = false;
		}
		else
		{
			state = State::Idle;
		}
	}
}

void Player::SlashUpdate3()
{
	slashTimer++;
	if (Input::Mouse::Triggered(Click::Left) || Input::Pad::Triggered(Button::L))
	{
		slashRegistered = true;
	}

	if (Input::Pad::Triggered(Button::X) || Input::Key::Triggered(DIK_LCONTROL))
	{
		slashTimer = 0;
		Dodge();
	}

	if (slashTimer > slash3Time)
	{
		slashTimer = 0;
		if (slashRegistered)
		{
			state = State::Idle;
			slashRegistered = false;
		}
		else
		{
			state = State::Idle;
		}
	}
}

void Player::Slash1()
{
	//³–Ê‚ÌŠp“x‚ðŒvŽZ
	Vec3 front = rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	/*Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();*/
	float angle = atan2f(front.x, front.z) + PIf;

	SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("Slash1", position), { 0, 1, 0 }, angle);

	model = ModelManager::GetModel("PlayerSlash2");
	model->ResetAnimTimer();
	model->aniSpeed = 1.5f;

	state = State::Slash1;
}

void Player::Slash2()
{
	//³–Ê‚ÌŠp“x‚ðŒvŽZ
	Vec3 front = rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	/*Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();*/
	float angle = atan2f(front.x, front.z) + PIf;

	SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("Slash2", position), { 0, 1, 0 }, angle);

	model = ModelManager::GetModel("PlayerSlash1");
	model->ResetAnimTimer();
	model->aniSpeed = 1.5f;

	state = State::Slash2;
}

void Player::Slash3()
{
	//³–Ê‚ÌŠp“x‚ðŒvŽZ
	Vec3 front = rotation.GetRotMat().ExtractAxisZ();
	front.y = 0;
	front.Norm();

	/*Vec3 side = rotation.GetRotMat().ExtractAxisX();
	side.y = 0;
	side.Norm();*/
	float angle = atan2f(front.x, front.z) + PIf;

	SpEffekseer::Manager()->SetRotation(SpEffekseer::Play("Slash3", position), { 0, 1, 0 }, angle);

	model = ModelManager::GetModel("PlayerSlash3");
	model->ResetAnimTimer();
	model->aniSpeed = 1.5f;

	state = State::Slash3;
}

OBBCollider Player::GetCollider()
{
	return col;
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