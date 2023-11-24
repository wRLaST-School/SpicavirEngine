#include "stdafx.h"
#include "SceneRW.h"
#include <fstream>
#include <iostream>
#include <sstream>

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)

using namespace std;
using namespace nlohmann;

void SceneRW::SaveScene(IScene* scene, std::string filePath)
{
	json outputObj;

	//コンポーネントを処理する再起関数
	function<void(json&, IComponent*)> processNode = [&](json& parentJsonObj, IComponent* current) {
		json curObj;

		curObj["Type"] = current->GetClassString();
		current->WriteParamJson(curObj);
		for (auto& c : current->GetAllComponents())
		{
			processNode(curObj["Children"], c.second.get());
		}

		parentJsonObj.push_back(curObj);
	};

	//ルートノードから呼び出し
	processNode(outputObj, scene);

	//ファイル書き込み
	std::ofstream file;

	file.open(filePath);

	file << outputObj.dump(4);

	file.close();
}

void SceneRW::ConfirmLoadScene()
{
	//TODO:新フォーマットに対応
	if (!GetInstance()->waitingForLoad_)
	{
		return;
	}

	//ファイル読み込み
	std::ifstream file;

	file.open(GetInstance()->filePath_);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;
	file >> deserialized;

	//シーンのリセット
	GetInstance()->scene_->ClearAllComponents();

	//ツリーの構築
	//読み込みの再起関数
	function<void(const json& object, IComponent* current)> processNode = [&](const json& object, IComponent* current)
	{
		string componentType = object["ComponentType"].get<string>();

		auto child = ComponentFactory::AddChildComponent(current, componentType, componentType);

		//子ノードの読み込み
		if (object.contains("Children"))
		{
			for (auto& node : object["Children"])
			{
				for (auto& singleNode : node)
				{
					processNode(singleNode, child);
				}
			}
		}
	};

	//読み込み再起関数呼び出し
	if (deserialized["Component0"].contains("Children"))
	{
		for (auto& node : deserialized["Component0"]["Children"])
		{
			processNode(node["Component0"], GetInstance()->scene_);
		}
	}

	file.close();

	IComponent::InitAllChildComponents(GetInstance()->scene_);

	GetInstance()->waitingForLoad_ = false;
}

void SceneRW::LoadScene(IScene* scene, std::string filePath)
{
	GetInstance()->scene_ = scene;
	GetInstance()->filePath_ = filePath;
	GetInstance()->waitingForLoad_ = true;
}

SceneRW* SceneRW::GetInstance()
{
    static SceneRW ins;
    return &ins;
}
