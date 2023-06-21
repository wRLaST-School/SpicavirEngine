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
	Mode mode_ = Mode::Target;

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

private:
	static CameraController* sCurrent;

	const float CAM_DIST = 7.5f;
};

