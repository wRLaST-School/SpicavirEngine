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

	bool IsActive();
private:
	//Object3D outerObj;
	//Object3D innerObj;
	bool active_ = false;

	int timer_ = 0;

	Float3 pos_;

	const static int FIN_TIME = 60;

	const static TextureKey TEX_KEY;

	const static int R = 4;
};

