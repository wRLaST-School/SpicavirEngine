#include "stdafx.h"
#include "LevelLoader.h"
#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

void LevelLoader::Load(std::string path, LevelManager* lm)
{
	//ファイル読み込み
	std::ifstream file;

	file.open(path);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;

	file >> deserialized;

	//正規データかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	string name = deserialized["name"].get<string>();

	assert(name.compare("scene") == 0);

	//走査処理
	lm->Clear();

	//再帰関数にする
	function<void(json& object, Object3D* parent)> ReadRec = [&ReadRec, &lm](json& obj, Object3D* parent) {
		assert(obj.contains("type"));

		string type = obj["type"].get<string>();

		//種類ごとの処理
		if (type.compare("MESH") == 0)
		{
			auto objdata = lm->AddComponent<Object3D>("Object3D");

			if (obj.contains("file_name")) {
				objdata->model = ModelManager::GetModel(obj["file_name"]);

				//TODO: 削除してテクスチャを貼る
				if (obj["file_name"] == "floor")
				{
					*objdata->brightnessCB.contents = { 0.22f, 0.22f, 0.22f, 1.0f };
				}
			}

			if (obj.contains("texture_file"))
			{
				objdata->texture = obj["texture_file"];
			}

			json& transform = obj["transform"];

			objdata->position.x = (float)transform["translation"][1];
			objdata->position.y = (float)transform["translation"][2];
			objdata->position.z = -(float)transform["translation"][0];

			objdata->rotationE.x = -(float)transform["rotation"][1];
			objdata->rotationE.y = -(float)transform["rotation"][2];
			objdata->rotationE.z = (float)transform["rotation"][0];
			objdata->rotMode = Object3D::RotMode::Euler;

			objdata->scale.x = (float)transform["scaling"][1];
			objdata->scale.y = (float)transform["scaling"][2];
			objdata->scale.z = (float)transform["scaling"][0];

			if (parent)
			{
				objdata->parent = parent;
			}

			//再帰処理
			if (obj.contains("children"))
			{
				for (auto& child : obj["children"])
				{
					ReadRec(child, objdata);
				}
			}
		}
	};

	for (auto& obj : deserialized["objects"]) {
		ReadRec(obj, nullptr);
	}

	file.close();
}