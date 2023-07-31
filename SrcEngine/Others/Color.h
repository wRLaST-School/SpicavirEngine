#pragma once
#include <string>
struct ColorInt {
	int32_t r;
	int32_t g;
	int32_t b;
	int32_t a;
};

class Color
{
public:
	Color(); //çïÇ≈èâä˙âª
	Color(const Float4& rgba);
	Color(float r, float g, float b, float a = 1.0f);
	Color(int32_t r, int32_t g, int32_t b, int32_t a = 255);
	Color(const ColorInt& rgba);
	Color(int32_t hexcolor);

	operator ColorInt();
	operator Float4();

	Float4 f4;

	void ShowImGUIEditor(std::string windowTitle);

	//à»â∫ñ¢é¿ëï
	operator int32_t();
	Float3 GetHSV();

	void SetHSV(float h, float s, float v);
	void Set(Float4 rgba);
	void Set(float r, float g, float b, float a = 1.0f);
	void Set(int32_t r, int32_t g, int32_t b, int32_t a = 255);
	void Set(ColorInt rgba);
	void Set(int32_t hexcolor);

	void AddHue(float h);

public:
	const static Color White;
	const static Color Black;
	const static Color Red;
	const static Color Green;
	const static Color Blue;
	const static Color Yellow;
	const static Color Pink;
	const static Color Cyan;
	const static Color Orange;
	const static Color Purple;
	const static Color Gray;
};