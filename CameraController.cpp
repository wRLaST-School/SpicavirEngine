#include "stdafx.h"
#include "CameraController.h"

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

		break;
	}

	case CameraController::Mode::Free:
	{
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

CameraController* CameraController::sCurrent = nullptr;