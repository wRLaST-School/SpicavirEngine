#pragma once
#include <string>
struct ColorInt {
	int r;
	int g;
	int b;
	int a;
};

class Color
{
public:
	Color(); //çïÇ≈èâä˙âª
	Color(Float4 rgba);
	Color(float r, float g, float b, float a = 1.0f);
	Color(int r, int g, int b, int a = 255);
	Color(ColorInt rgba);
	Color(int hexcolor);

	operator ColorInt();
	operator Float4();

	Float4 f4;

	void ShowImGUIEditor(std::string windowTitle);

	//à»â∫ñ¢é¿ëï
	operator int();
	Float3 GetHSV();

	void SetHSV(float h, float s, float v);
	void Set(Float4 rgba);
	void Set(float r, float g, float b, float a = 1.0f);
	void Set(int r, int g, int b, int a = 255);
	void Set(ColorInt rgba);
	void Set(int hexcolor);

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