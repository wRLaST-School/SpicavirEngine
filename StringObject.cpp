#include "stdafx.h"
#include "StringObject.h"
#include <SpImGui.h>

void StringObject::Draw()
{
	if(str_.length())
		TextDrawer::DrawString(str_, (int32_t)pos_.x, (int32_t)pos_.y, Align::Center, color_, opt_);
}

void StringObject::DrawParams()
{
	SpImGui::InputText("Text", &str_, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::InputFloat2("Pos", &pos_.x);

	ImGui::ColorEdit4("Color", &color_.f4.x);

	ImGui::InputInt("Resolution", &opt_.fontOptions.resolution);

	ImGui::InputInt("FontSize", &opt_.size);
}

void StringObject::WriteParamJson(nlohmann::json& jsonObj)
{
	jsonObj["Text"] = str_;
	jsonObj["PosX"] = pos_.x;
	jsonObj["PosY"] = pos_.y;
	jsonObj["ColorR"] = color_.f4.x;
	jsonObj["ColorG"] = color_.f4.y;
	jsonObj["ColorB"] = color_.f4.z;
	jsonObj["ColorA"] = color_.f4.w;
	jsonObj["Res"] = opt_.fontOptions.resolution;
	jsonObj["FontSize"] = opt_.size;
}

void StringObject::ReadParamJson(const nlohmann::json& jsonObj)
{
	str_ = jsonObj["Text"];
	pos_.x = jsonObj["PosX"];
	pos_.y = jsonObj["PosY"];
	color_.f4.x = jsonObj["ColorR"];
	color_.f4.y = jsonObj["ColorG"];
	color_.f4.z = jsonObj["ColorB"];
	color_.f4.w = jsonObj["ColorA"];
	opt_.fontOptions.resolution = jsonObj["Res"];
	opt_.size = jsonObj["FontSize"];
}
