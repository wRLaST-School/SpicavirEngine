#include "stdafx.h"
#include "ScriptComponent.h"
#include <SpImGui.h>
#include <LibraCompiler.h>

void ScriptComponent::Init()
{
	LoadDLL();

	if (dllobj_.GetModule()) dllobj_.GetComponent()->Init();
}

void ScriptComponent::Update()
{
	if (dllobj_.GetModule()) dllobj_.GetComponent()->Update();
}

void ScriptComponent::Draw()
{
	if (dllobj_.GetModule()) dllobj_.GetComponent()->Draw();
}

void ScriptComponent::OnInspectorWindowDraw()
{
	if (dllobj_.GetModule()) dllobj_.GetComponent()->OnInspectorWindowDraw();

	if (SpImGui::InputText("FilePath", &filePath, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Init();
	};

	if (ImGui::Button("Compile"))
	{
		CompileScript();
		LoadDLL();
	}
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
	if (dllobj_.GetModule()) dllobj_.Free();

	std::string compileDest = compileFolder + filePath;

	//コンパイル処理
	Libra::Compiler::Compile(filePath, compileDest);

	Libra::Compiler::TestCPPCompile();
}

void ScriptComponent::LoadDLL()
{
	std::string dllPath = compileFolder + filePath;

	//読み込み処理
	dllobj_.LoadDLL(dllPath);
}
