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
	stringstream jsonStr;

	jsonStr << "{" << endl;

	//コンポーネントを処理する再起関数
	function<void(IComponent* currentNode, std::string indent, std::string nodeNum)> processNode = [&](IComponent* current, std::string indent, std::string nodeNum = "") {
		jsonStr << indent << "\"Component" << nodeNum << "\":{\n";
		string firstIndent = indent;
		indent += "  ";
		jsonStr << indent << "\"ComponentType\":\"" << current->GetClassString() << "\",\n";

		int32_t nodeNumInt = 0;
		if (current->GetAllConponents().size())
		{
			jsonStr << indent << "\"Children\":[\n" << indent << "{\n";

			for (auto& c : current->GetAllConponents())
			{
				processNode(c.second.get(), indent + "  ", to_string(nodeNumInt));
				nodeNumInt++;
			}
			//最後のカンマを削除
			string tempStr = jsonStr.str();

			tempStr.pop_back();
			tempStr.pop_back();

			tempStr += string("\n");

			jsonStr.str("");
			jsonStr.clear(stringstream::goodbit);

			jsonStr << tempStr;

			jsonStr << indent << "}],\n";
		}

		jsonStr << indent << "\"NumChildren\":\"" << nodeNumInt << "\"\n";

		jsonStr << firstIndent << "},\n";
	};

	//ルートノードから呼び出し
	processNode(scene, "", "0");

	//最後のカンマを削除
	string tempStr = jsonStr.str();

	tempStr.pop_back();
	tempStr.pop_back();

	tempStr += string("\n");

	jsonStr.str("");
	jsonStr.clear(stringstream::goodbit);

	jsonStr << tempStr;
	jsonStr << "\n";

	jsonStr << "}" << endl;


	//ファイル書き込み
	std::ofstream file;

	file.open(filePath);

	file << jsonStr.str() << endl;

	file.close();
}

void SceneRW::ConfirmLoadScene()
{
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
