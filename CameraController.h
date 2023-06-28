#pragma once
#include <Camera.h>

class CameraController
{
public:
	enum class Mode {
		Target,
		Free
	};

private:
	Mode mode_ = Mode::Free;

public:
	void Init();
	void Update();
	void Set();
	Mode GetMode();

	void ToggleMode();

	void SetMode(Mode m);

	unique_ptr<Camera> cam;

public:
	static CameraController* Get();
	static void Set(CameraController* cctrl);

	static float GetCamSpd();

private:
	static CameraController* sCurrent;

	const float CAM_DIST = 7.5f;

	float camSpeed = PIf / 180;
};
