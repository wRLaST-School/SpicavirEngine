#pragma once
struct ColorInt {
	int r;
	int g;
	int b;
	int a;
};

class Color
{
	ColorInt GetInt();
	Float4 GetFloat();
	int GetHex();
	Float3 GetHSV();

	void SetHSV(float, float, float);
	void SetFloat4(Float4);
	void SetFloat4(float, float, float, float);
	void SetInt(int, int, int, int);
	void SetInt(ColorInt);
	void SetHex(int);

	void AddHue(float);

	void ShowImGUIEditor();
}

