#include "stdafx.h"
#include "ScriptComponent.h"
#include <SpImGui.h>
#include <Libra/LibraCompiler/LibraCompiler.h>
#include <Libra/LibraCompiler/CPPCompiler.h>
#include <format>
#include <functional>
#include <SceneManager.h>

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
	//スクリプトコンポーネントを全て保持してdllobjをfree
	eastl::vector<ScriptComponent*> scriptcomps;

	std::function<void(eastl::vector<ScriptComponent*>&, IComponent*)> GetAllScriptCompsRecursive =
		[&](eastl::vector<ScriptComponent*>& list, IComponent* parent) {
		auto& components = parent->GetAllComponents();
		if (components.size())
		{
			for (auto itr = components.begin(); itr != components.end(); itr++)
			{
				GetAllScriptCompsRecursive(list, itr->second.get());
			}
		}

		auto castedParent = parent->CastTo<ScriptComponent>();
		if (castedParent) {
			list.push_back(castedParent);
		}
	};

	GetAllScriptCompsRecursive(scriptcomps, SceneManager::currentScene.get());

	for (auto& c : scriptcomps) {
		if(c->dllobj_.GetModule())
			c->dllobj_.Free();
	}

	//コンパイル処理
	//Libra::Compiler::Compile(filePath, compileDest);
	CPPCompiler::Compile();

	//スクリプトコンポーネントを全て保持してdllobjをfree
	for (auto& c : scriptcomps) {
		c->LoadDLL();
	}
}

void ScriptComponent::LoadDLL()
{
	//読み込み処理
	dllobj_.LoadDLL(className);

	if (dllobj_.GetComponent()) dllobj_.GetComponent()->body = this;
	else OutputDebugStringA(std::format("No Such Class Found. Class Name: {}\n", className).c_str());

	if (dllobj_.GetComponent()) dllobj_.GetComponent()->Init();
}
