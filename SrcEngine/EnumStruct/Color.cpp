#include "stdafx.h"
#include "Color.h"
#include <SpImGui.h>

const Color Color::White = Color(0xffffff);
const Color Color::Black = Color(0x0);
const Color Color::Red = Color(0xff0000);
const Color Color::Green = Color(0x00ff00);
const Color Color::Blue = Color(0x0000ff);
const Color Color::Yellow = Color(0x00ffff);
const Color Color::Pink = Color(0xff00ff);
const Color Color::Cyan = Color(0xffff00);
const Color Color::Orange = Color(0xffa500);
const Color Color::Purple = Color(0x800080);
const Color Color::Gray = Color(0x888888);

Color::Color()
{
	f4 = {0.f, 0.f, 0.f, 0.f};
}

Color::Color(Float4 rgba)
{
	f4 = rgba;
}

Color::Color(float r, float g, float b, float a)
{
	f4 = { r, g, b, a };
}

Color::Color(int r, int g, int b, int a)
{
	f4 = { (float)r / 255.f,(float)g / 255.f, (float)b / 255.f, (float)a / 255.f };
}

Color::Color(ColorInt rgba)
{
	f4 = { (float)rgba.r / 255.f,(float)rgba.g / 255.f, (float)rgba.b / 255.f, (float)rgba.a / 255.f };
}

Color::Color(int hexcolor)
{
	ColorInt rgba = {0, 0, 0, 0};
	rgba.r = hexcolor & 0xff0000;
	rgba.r = rgba.r >> 16;

	rgba.g = hexcolor & 0xff00;
	rgba.g = rgba.g >> 8;

	rgba.b = hexcolor & 0xff;

	f4 = { (float)rgba.r / 255.f,(float)rgba.g / 255.f, (float)rgba.b / 255.f, 1.f };
}

Color::operator ColorInt()
{
	Float4 t = { 255.f * f4.x, 255.f * f4.y, 255.f * f4.z, 255.f * f4.w };
	return { (int)t.x, (int)t.y, (int)t.z, (int)t.w};
}

Color::operator Float4()
{
	return f4;
}

void Color::ShowImGUIEditor(std::string windowTitle)
{
	SpImGui::Command([&] {
		if (ImGui::Begin(windowTitle.c_str())) {
			ImGui::ColorEdit4(windowTitle.c_str(), &f4.x);
		}
		ImGui::End();
	});
}
