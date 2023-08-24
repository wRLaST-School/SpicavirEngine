#include "stdafx.h"
#include "BehaviorTree.h"
#include <BTRootNode.h>
#include <fstream>
#include <iostream>
#include <sstream>

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)

using namespace nlohmann;
using namespace std;

BT::BehaviorTree::BehaviorTree()
{
	root = std::make_unique<RootNode>();
	dynamic_cast<RootNode*>(root.get())->SetRootBT(this);
}

void BT::BehaviorTree::SetFactory(const BehaviorTreeFactory& factory)
{
	funcFactory_ = factory;
}

BT::BehaviorTreeFactory* BT::BehaviorTree::GetFactory()
{
	return &funcFactory_;
}

void BT::BehaviorTree::Tick()
{
	root->Update();
}

void BT::BehaviorTree::LoadJson(std::string path)
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

	//�c���[�̃��Z�b�g

	//�c���[�̍\�z

	//�ǂݍ��݂̍ċN�֐�

	//�ǂݍ��ݍċN�֐��Ăяo��
}

void BT::BehaviorTree::SaveJson(std::string path)
{
	//json�`����string���쐬
	
	//�쐬�C���[�W
	// {
	// "Node":{
	//	"NodeType":"NODETYPE",
	//	"NodeParam":"NODEPARAM",
	//  "Children":[
	//			{"Node": {
	//				}
	//			},
	//			{"Node": {
	//				}
	//			},
	//		]
	//	}
	// }

	stringstream jsonStr;

	jsonStr << "{" << endl;

	//�m�[�h����������ċN�֐�
	function<void(INode* currentNode, std::string indent, std::string nodeNum)> processNode = [&](INode* current, std::string indent, std::string nodeNum = "") {
		jsonStr << indent << "\"Node" << nodeNum << "\":{\n";
		string firstIndent = indent;
		indent += "  ";
		jsonStr << indent << "\"NodeType\":\"" << current->GetNodeType() << "\",\n";
		jsonStr << indent << "\"NodeParam\":\"" << current->GetParam() << "\",\n";

		int nodeNumInt = 0;
		if (current->GetChildren().size())
		{
			jsonStr << indent << "\"Children\":[\n" << indent << "{\n";

			for (auto& c : current->GetChildren())
			{
				processNode(c.get(), indent + "  ", to_string(nodeNumInt));
				nodeNumInt++;
			}
			//�Ō�̃J���}���폜
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

	//���[�g�m�[�h����Ăяo��
	processNode(root.get(), "", "0");

	//�Ō�̃J���}���폜
	string tempStr = jsonStr.str();

	tempStr.pop_back();
	tempStr.pop_back();

	tempStr += string("\n");

	jsonStr.str("");
	jsonStr.clear(stringstream::goodbit);

	jsonStr << tempStr;
	jsonStr << "\n";

	jsonStr << "}" << endl;


	//�t�@�C����������
	std::ofstream file;

	file.open(path);

	if (file.fail())
	{
		assert(0);
	}

	file << jsonStr.str() << endl;

	file.close();
}
