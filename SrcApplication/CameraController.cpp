#include "stdafx.h"
#include "CameraController.h"
#include <Boss.h>
#include <Player.h>
#include <Input.h>

void CameraController::Init()
{
	cam_ = std::make_unique<Camera>();

	cam_->UseDefaultParams();

	cam_->position = { 0, 3, -10 };

	freeCamRot_ = {PIf / 4, 0.f};
}

void CameraController::Update()
{
	switch (mode_)
	{
	case CameraController::Mode::Target:
	{
		cam_->target = Boss::Get()->position;
		cam_->targetMode = CameraTargetMode::LookAt;

		Vec3 front = (Vec3)Boss::Get()->position - Player::Get()->position;
		front.y = 0;

		//ƒ[ƒƒxƒNƒgƒ‹‚Å”ò‚Î‚È‚¢‚æ‚¤‚É
		if (front.GetSquaredLength())
		{
			lastPlayerPos_ = Player::Get()->position;
		}
		else
		{
			front = (Vec3)Boss::Get()->position - lastPlayerPos_;
		}

		front.Norm();

		cam_->position = (Vec3)Player::Get()->position - front.SetLength(CAM_DIST);
		cam_->position.y = 3;
		break;
	}

	case CameraController::Mode::Free:
	{
		Float2 lastFreeCamRot = freeCamRot_;

		freeCamRot_.x += CameraController::GetCamSpd() * (Input::Key::Down(DIK_UP) - Input::Key::Down(DIK_DOWN) - Input::Pad::GetRStick().y);
		freeCamRot_.y += CameraController::GetCamSpd() * (Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT) + Input::Pad::GetRStick().x);

		if (freeCamRot_.x >= PIf / 3.f)
		{
			freeCamRot_.x = PIf / 3.f;
		}

		Matrix rotation = Matrix::RotX(freeCamRot_.x);
		rotation *= Matrix::RotY(freeCamRot_.y);


		cam_->targetMode = CameraTargetMode::LookAt;
		//cam->rotation = Player::Get()->rotation;

		cam_->target = Player::Get()->position;
		cam_->target.y += 2.0f;

		Vec3 front = rotation.ExtractAxisZ();
		//front.y = 0;
		front.Norm();

		cam_->position = (Vec3)Player::Get()->position - front.SetLength(CAM_DIST);
		//cam->position.y = 3;

		if (cam_->position.y < 1.f)
		{
			cam_->position.y = 1.0f;
			freeCamRot_ = lastFreeCamRot;
		}

		cam_->UpdateMatrix();
		break;
	}
	
	default:
		break;
	}
}

void CameraController::Set()
{
	Camera::Set(*cam_.get());
}

CameraController::Mode CameraController::GetMode()
{
	return mode_;
}

void CameraController::ToggleMode()
{
	if (mode_ == Mode::Target)
	{
		cam_->rotation = Player::Get()->rotation;
		mode_ = Mode::Free;
	}
	else
	{
		mode_ = Mode::Target;
	}
}

void CameraController::SetMode(Mode m)
{
	mode_ = m;
}

const Camera& CameraController::GetCamera()
{
	return *cam_.get();
}

CameraController* CameraController::Get()
{
	return sCurrent;
}

void CameraController::Set(CameraController* cctrl)
{
	sCurrent = cctrl;
}

float CameraController::GetCamSpd()
{
	return sCurrent->camSpeed_;
}

CameraController* CameraController::sCurrent = nullptr;