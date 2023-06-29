#include "stdafx.h"
#include "CameraController.h"
#include <Boss.h>
#include <Player.h>
#include <Input.h>

void CameraController::Init()
{
	cam = make_unique<Camera>();

	cam->UseDefaultParams();

	cam->position = { 0, 3, -10 };
}

void CameraController::Update()
{
	switch (mode_)
	{
	case CameraController::Mode::Target:
	{
		cam->target = Boss::Get()->position;
		cam->targetMode = CameraTargetMode::LookAt;

		Vec3 front = Player::Get()->rotation.GetRotMat().ExtractAxisZ();
		front.y = 0;
		front.Norm();

		cam->position = (Vec3)Player::Get()->position - front.SetLength(CAM_DIST);
		cam->position.y = 3;
		break;
	}

	case CameraController::Mode::Free:
	{
		freeCamRot_.x += CameraController::GetCamSpd() * (Input::Key::Down(DIK_UP) - Input::Key::Down(DIK_DOWN) - Input::Pad::GetRStick().y);
		freeCamRot_.y += CameraController::GetCamSpd() * (Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT) + Input::Pad::GetRStick().x);

		Matrix rotation = Matrix::RotX(freeCamRot_.x);
		rotation *= Matrix::RotY(freeCamRot_.y);


		cam->targetMode = CameraTargetMode::LookAt;
		//cam->rotation = Player::Get()->rotation;

		cam->target = Player::Get()->position;
		cam->target.y += 2.0f;

		Vec3 front = rotation.ExtractAxisZ();
		//front.y = 0;
		front.Norm();

		cam->position = (Vec3)Player::Get()->position - front.SetLength(CAM_DIST);
		//cam->position.y = 3;

		cam->UpdateMatrix();
		break;
	}
	
	default:
		break;
	}
}

void CameraController::Set()
{
	Camera::Set(*cam.get());
}

CameraController::Mode CameraController::GetMode()
{
	return mode_;
}

void CameraController::ToggleMode()
{
	if (mode_ == Mode::Target)
	{
		cam->rotation = Player::Get()->rotation;
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
	return sCurrent->camSpeed;
}

CameraController* CameraController::sCurrent = nullptr;