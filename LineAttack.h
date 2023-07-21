#pragma once
class LineAttack
{
public:
	LineAttack(const Float3& pos, float angle);

	void Update();
	void Draw();

	Float3 pos{};
	bool active = false;

private:
	//Object3D outerObj;
	//Object3D innerObj;

	int timer_ = 0;

	const static int FIN_TIME = 60;

	const static int R = 4;

	float angle;
};

