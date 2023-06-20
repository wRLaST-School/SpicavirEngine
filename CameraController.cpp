#include "stdafx.h"
#include "CameraController.h"

void CameraController::Init()
{
	cam = make_unique<Camera>();

	cam->UseDefaultParams();

	cam->position = { 0, 0, 0 };
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
