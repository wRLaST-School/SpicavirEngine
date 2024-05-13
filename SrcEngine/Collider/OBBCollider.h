#pragma once
#include <Color.h>
class DLLExport OBBCollider
{
public:
	bool Collide(const OBBCollider& other);

	void DrawBB(Color color = Color::White);

public:
	Float3 pos;
	Quaternion rot;
	Float3 scale;
};

