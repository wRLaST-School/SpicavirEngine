#pragma once
struct Float4
{
	float x;
	float y;
	float z;
	float w;

	[[maybe_unused]]
	void operator = (const Float4& f4)
	{
		x = f4.x; y = f4.y; z = f4.z; w = f4.w;
	};
};

