#pragma once
#include "SpMath.h"
class DLLExport DirectionalLight
{
public:
	Vec3 direction = Vec3(1,0,0);
	Float3 color = { 1.0f, 1.0f, 1.0f };
};

