#include "stdafx.h"
#include "ScriptComponent.h"
#include <SpImGui.h>
#include <LibraCompiler.h>

void ScriptComponent::Init()
{
}

void ScriptComponent::Update()
{
}

void ScriptComponent::Draw()
{
}

void ScriptComponent::OnInspectorWindowDraw()
{
	SpImGui::InputText("FilePath", &filePath, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);
}

void ScriptComponent::WriteParamJson(nlohmann::json& jsonObj)
{
	jsonObj["filePath"] = filePath;
}

void ScriptComponent::ReadParamJson(const nlohmann::json& jsonObj)
{
	filePath = jsonObj["filePath"];
}

void ScriptComponent::CompileScript()
{
	std::string compileDest = compileFolder + filePath;

	//コンパイル処理
	Libra::Compiler::Compile(filePath, compileDest);
}

void ScriptComponent::ReloadDLL()
{
	std::string dllPath = compileFolder + filePath;

	//再読み込み処理
}
