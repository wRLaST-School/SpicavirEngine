#include "stdafx.h"
#include "ScriptComponent.h"
#include <SpImGui.h>
#include <Libra/LibraCompiler/LibraCompiler.h>
#include <Libra/LibraCompiler/CPPCompiler.h>
#include <format>

void ScriptComponent::Init()
{
	LoadDLL();

	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Init();
}

void ScriptComponent::Update()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Update();
}

void ScriptComponent::Draw()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Draw();
}

void ScriptComponent::OnInspectorWindowDraw()
{
	if (dllobj_.GetComponent()) dllobj_.GetComponent()->OnInspectorWindowDraw();

	if (SpImGui::InputText("Class Name", &className, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
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
	jsonObj["filePath"] = className;
}

void ScriptComponent::ReadParamJson(const nlohmann::json& jsonObj)
{
	className = jsonObj["filePath"];
}

void ScriptComponent::CompileScript()
{
	if (dllobj_.GetModule()) dllobj_.Free();

	//コンパイル処理
	//Libra::Compiler::Compile(filePath, compileDest);
	CPPCompiler::Compile();

	LoadDLL();
}

void ScriptComponent::LoadDLL()
{
	//読み込み処理
	dllobj_.LoadDLL(className);

	if (dllobj_.GetComponent()) dllobj_.GetComponent()->body = this;
	else OutputDebugStringA(std::format("No Such Class Found. Class Name: {}\n", className).c_str());
}
