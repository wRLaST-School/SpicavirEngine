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

void SceneRW::SaveEfkEffects(json& dist)
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
	SaveEfkEffects((*outputObj.begin())["EfkEffects"]);

	//ファイル書き込み
	std::ofstream file;

	file.open(filePath);

	file << outputObj.dump(4);

	file.close();
}

void SceneRW::LoadScene(IScene* scene, std::string filePath)
{
	//ファイル読み込み
	std::ifstream file;

	file.open(filePath);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;
	file >> deserialized;

	LoadResources(deserialized);

	//ツリーの構築
	//読み込みの再起関数
	function<void(const json& object, IComponent* current)> processNode = [&](const json& object, IComponent* current)
	{
		string componentType = object["Type"].get<string>();

		auto child = ComponentFactory::AddChildComponent(current, componentType, componentType);
		child->ReadParamJson(object);

		//子ノードの読み込み
		if (object.contains("Children"))
		{
			for (auto& node : object["Children"])
			{
				processNode(node, child);
			}
		}
	};

	//読み込み再起関数呼び出し
	if (deserialized.front().contains("Children"))
	{
		for (auto& node : deserialized.front()["Children"])
		{
			processNode(node, scene);
		}
	}

	file.close();
}

void SceneRW::LoadResources(nlohmann::json& root)
{
	SpTextureManager::PreLoadNewScene();
	ModelManager::PreLoadNewScene();
	SoundManager::PreLoadNewScene();
	SpEffekseer::PreLoadNewScene();

	LoadTextures(root.front()["Textures"]);
	LoadModels(root.front()["Models"]);
	LoadSounds(root.front()["Sounds"]);
	LoadEfkEffects(root.front()["EfkEffects"]);
}

void SceneRW::LoadTextures(nlohmann::json& textures)
{
	std::map<TextureKey, std::string> tempMap = textures.front().get<std::map<TextureKey, std::string>>();

	for (auto& tex : tempMap)
	{
		SpTextureManager::LoadTexture(tex.second, tex.first);
	}
}

void SceneRW::LoadModels(nlohmann::json& models)
{
	std::map<ModelKey, std::string> tempMap = models.front().get<std::map<ModelKey, std::string>>();

	for (auto& model : tempMap)
	{
		ModelManager::Register(model.second, model.first, true);
	}
}

void SceneRW::LoadSounds(nlohmann::json& sounds)
{
	std::map<SoundKey, std::string> tempMap = sounds.front().get<std::map<SoundKey, std::string>>();

	for (auto& sound : tempMap)
	{
		SoundManager::LoadWave(sound.second, sound.first);
	}
}

void SceneRW::LoadEfkEffects(nlohmann::json& efks)
{
	std::map<EffectKey, std::pair<std::string, std::string>> tempMap 
		= efks.front().get<std::map<EffectKey, std::pair<std::string, std::string>>>();

	for (auto& effect : tempMap)
	{
		SpEffekseer::Load(effect.second.first, effect.second.second, effect.first);
	}
}

SceneRW* SceneRW::GetInstance()
{
    static SceneRW ins;
    return &ins;
}
