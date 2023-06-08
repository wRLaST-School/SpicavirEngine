#pragma once
#include <SpDS.h>
class Camera2D
{
public:
	int32_t x = 0;
	int32_t y = 0;

	void DrawRotaGraphCam(int32_t x, int32_t y, float dx, float dy, float rot, TextureKey key, Anchor anchor = Anchor::Center, Color brightness = Color(0xffffff));
	static Camera2D* Get();
	static void Set(Camera2D* camera);

	static Matrix GetViewProjMat();

private:
	static Camera2D* current;
};