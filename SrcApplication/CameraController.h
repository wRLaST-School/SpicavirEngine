#pragma once
#include <Camera.h>

class CameraController : public IComponent
{
public:
	ComponentFactoryRegister(CameraController)

	enum class Mode {
		Target,
		Free
	};

private:
	Mode mode_ = Mode::Target;

	std::unique_ptr<Camera> cam_;

public:
	void Init();
	void Update();
	void Set();
	Mode GetMode();

	void ToggleMode();

	void SetMode(Mode m);

	const Camera& GetCamera();

public:
	static CameraController* Get();
	static void Set(CameraController* cctrl);
	
	static float GetCamSpd();

private:
	static CameraController* sCurrent;

	const float CAM_DIST = 7.5f;

	float camSpeed_ = PIf / 180;

	Float2 freeCamRot_ = {PIf / 4, 0.f};

	Float3 lastPlayerPos_ = {};
};

