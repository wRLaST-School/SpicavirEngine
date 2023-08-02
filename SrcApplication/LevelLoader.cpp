#include "stdafx.h"
#include "LevelLoader.h"
#include <LevelManager.h>
#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

void LevelLoader::Load(std::string path)
{
	//�t�@�C���ǂݍ���
	std::ifstream file;

	file.open(path);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;

	file >> deserialized;

	//���K�f�[�^���`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	string name = deserialized["name"].get<string>();

	assert(name.compare("scene") == 0);

	//��������
	LevelManager::Clear();

	//�ċA�֐��ɂ���
	function<void(json& object, Object3D* parent)> ReadRec = [&ReadRec](json& obj, Object3D* parent) {
		assert(obj.contains("type"));

		string type = obj["type"].get<string>();

		//��ނ��Ƃ̏���
		if (type.compare("MESH") == 0)
		{
			LevelManager::objects_.emplace_back();
			auto& objdata = LevelManager::objects_.back();

			if (obj.contains("file_name")) {
				objdata.model = ModelManager::GetModel(obj["file_name"]);

				//TODO: �폜���ăe�N�X�`����\��
				if (obj["file_name"] == "floor")
				{
					*objdata.brightnessCB.contents = { 0.3f, 0.3f, 0.3f, 1.0f };
				}
				if (obj["file_name"] == "Sky")
				{
					*objdata.brightnessCB.contents = { 0.f, 0.f, 0.f, 1.0f };
				}
			}

			json& transform = obj["transform"];

			objdata.position.x = (float)transform["translation"][1];
			objdata.position.y = (float)transform["translation"][2];
			objdata.position.z = -(float)transform["translation"][0];

			objdata.rotationE.x = -(float)transform["rotation"][1];
			objdata.rotationE.y = -(float)transform["rotation"][2];
			objdata.rotationE.z = (float)transform["rotation"][0];
			objdata.rotMode = Object3D::RotMode::Euler;

			objdata.scale.x = (float)transform["scaling"][1];
			objdata.scale.y = (float)transform["scaling"][2];
			objdata.scale.z = (float)transform["scaling"][0];

			if (parent)
			{
				objdata.parent = parent;
			}

			//�ċA����
			if (obj.contains("children"))
			{
				for (auto& child : obj["children"])
				{
					ReadRec(child, &objdata);
				}
			}
		}
	};

	for (auto& obj : deserialized["objects"]) {
		ReadRec(obj, nullptr);
	}
}