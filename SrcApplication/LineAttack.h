#pragma once
#include <OBBCollider.h>
class LineAttack
{
public:
	LineAttack(const Float3& pos, float angle);

	void Update();
	void Draw();

	bool IsActive();

	Float3 pos{};

private:
	//Object3D outerObj;
	//Object3D innerObj;
	bool active_ = false;

	int timer_ = 0;

	const static int FIN_TIME = 60;

	const static int R = 4;

	float angle_;

	OBBCollider col_;
};

