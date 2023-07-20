#pragma once
#include <Object3D.h>
class Marker
{
public:
	Marker();

	void Cast(const Float3& pos);

	void InitModel();

	void Update();
	void Draw();

	Float3 pos;
	bool active = false;

private:
	//Object3D outerObj;
	//Object3D innerObj;

	int timer_ = 0;

	const static int FIN_TIME = 60;

	const static TextureKey TEX_KEY;

	const static int R = 4;
};

