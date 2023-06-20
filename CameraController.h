#pragma once
#include <Camera.h>

class CameraController
{
private:
	enum class Mode {
		Target,
		Free
	} mode_;

public:
	void Init();
	void Update();
	void Set();

	void ToggleMode();

	void SetMode(Mode m);

	unique_ptr<Camera> cam;


};

