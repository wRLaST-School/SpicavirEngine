#include "stdafx.h"
#include "SceneRW.h"
#include <fstream>
#include <iostream>
#include <sstream>


#include <SpTextureManager.h>
#include <Model.h>
#include <SoundManager.h>
#include <SpEffekseer.h>

using namespace std;
using namespace nlohmann;

void SceneRW::SaveTextures(json& dist)
{
	std::map<TextureKey, std::string> keyAndPath;

	SpTextureManager::GetInstance().texDataMap_.Access([&](auto& map) {
		for (auto& pair : map)
		{
			if (SpTextureManager::IsMasterTexture(pair.first)) continue;

			if (pair.second.filePath == "") continue;

			keyAndPath.emplace(pair.first, pair.second.filePath);
		}
	});

	json textures = keyAndPath;

	dist.push_back(textures);
}

void SceneRW::SaveModels(json& dist)
{
	std::map<ModelKey, std::string> keyAndPath;

	ModelManager::sModels.Access([&](auto& map) {
		for (auto& pair : map)
		{
			keyAndPath.emplace(pair.first, pair.second.filePath);
		}
		});

	json models = keyAndPath;

	dist.push_back(models);
}

void SceneRW::SaveSounds(json& dist)
{
	std::map<SoundKey, std::string> keyAndPath;

	SoundManager::sSndMap.Access([&](auto& map) {
		for (auto& pair : map)
		{
			keyAndPath.emplace(pair.first, pair.second.filePath);
		}
		});

	json sounds = keyAndPath;

	dist.push_back(sounds);
}

void SceneRW::SaveEffekseers(json& dist)
{
	std::map<EffectKey, std::pair<std::string, std::string>> keyAndPath;

	SpEffekseer::sEffects.Access([&](auto& map) {
		for (auto& pair : map)
		{
			keyAndPath.emplace(pair.first, 
				std::pair<std::string, std::string>(pair.second.texFolder, pair.second.filePath));
		}
		});

	json effects = keyAndPath;

	dist.push_back(effects);
}

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

	SaveTextures((*outputObj.begin())["Textures"]);
	SaveModels((*outputObj.begin())["Models"]);
	SaveSounds((*outputObj.begin())["Sounds"]);
	SaveEffekseers((*outputObj.begin())["EfkEffects"]);

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
