#pragma once
#include <SpDS.h>
class Camera2D
{
public:
	int x = 0;
	int y = 0;

	void DrawRotaGraphCam(int x, int y, float dx, float dy, float rot, TextureKey key, Anchor anchor = Anchor::Center, Color brightness = Color(0xffffff));
	static Camera2D* Get();
	static void Set(Camera2D* camera);

	static Matrix GetViewProjMat();

private:
	static Camera2D* current;
};