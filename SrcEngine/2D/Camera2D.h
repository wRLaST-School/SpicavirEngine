#pragma once
#include <SpDS.h>
class Camera2D
{
public:
	int32_t x = 0;
	int32_t y = 0;

	void DrawRotaGraphCam(int32_t x, int32_t y, float dx, float dy, float rot, const TextureKey& key, const Anchor& anchor = Anchor::Center, const Color& brightness = Color(0xffffff));
	static Camera2D* Get();
	static void Set(Camera2D* camera);
	Float2 GetDiff();

	static Matrix GetViewProjMat();
	static void OnFrameStart();

private:
	static Camera2D* sCurrent;

	int32_t lastX = 0;
	int32_t lastY = 0;
};