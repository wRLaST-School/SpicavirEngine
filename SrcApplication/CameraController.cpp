#include "stdafx.h"
#include "CameraController.h"
#include <Boss.h>
#include <Player.h>
#include <Input.h>
#include <Easing.h>

void CameraController::Init()
{
	cam_ = std::make_unique<Camera>();

	cam_->UseDefaultParams();

	cam_->position = { 0, 3, -10 };

	freeCamRot_ = {PIf / 4, 0.f};
}

void CameraController::Update()
{
	Player* pl = Player::Get();
	
	switch (mode_)
	{
	case CameraController::Mode::Target:
	{
		if (pl->GetState() != Player::State::Dodge)
		{
			cam_->target = Boss::Get()->obj_->position;
			cam_->targetMode = CameraTargetMode::LookAt;

			Vec3 front = (Vec3)Boss::Get()->obj_->position - pl->obj_->position;
			front.y = 0;

			//ゼロベクトルで飛ばないように
			if (front.GetSquaredLength())
			{
				lastPlayerPos_ = pl->obj_->position;
			}
			else
			{
				front = (Vec3)Boss::Get()->obj_->position - lastPlayerPos_;
			}

			front.Norm();

			cam_->position = (Vec3)pl->obj_->position - front.SetLength(CAM_DIST);
			cam_->position.y = 3;
		}
		else
		{
			cam_->target = Boss::Get()->obj_->position;
			cam_->targetMode = CameraTargetMode::LookAt;

			Vec3 front = (Vec3)Boss::Get()->obj_->position - pl->obj_->position;
			front.y = 0;

			//ゼロベクトルで飛ばないように
			if (front.GetSquaredLength())
			{
				lastPlayerPos_ = pl->obj_->position;
			}
			else
			{
				front = (Vec3)Boss::Get()->obj_->position - lastPlayerPos_;
			}

			front.Norm();

			Player::DodgeData dodgeData = pl->GetDodgeData();

			Float3 cameraEndPos = (Vec3)dodgeData.dodgeEndPlayerPos - front.SetLength(CAM_DIST);

			float t = (float)dodgeData.dodgeTimer / (float)dodgeData.iFrame;

			cam_->position = Easing::In(dodgeData.startCameraPos, cameraEndPos, t, 2.f);
			cam_->position.y = 3;
		}

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

		cam_->target = pl->obj_->position;
		cam_->target.y += 2.0f;

		Vec3 front = rotation.ExtractAxisZ();
		//front.y = 0;
		front.Norm();

		cam_->position = (Vec3)pl->obj_->position - front.SetLength(CAM_DIST);
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
		cam_->rotation = Player::Get()->obj_->rotation;
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