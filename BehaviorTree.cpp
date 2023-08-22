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

	SaveJson("Resources/data/BTTest.json");
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
}

void BT::BehaviorTree::SaveJson(std::string path)
{
	//json形式のstringを作成
	
	//作成イメージ
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

	//ノードを処理する再起関数
	function<void(INode* currentNode, std::string indent)> processNode = [&](INode* current, std::string indent) {
		jsonStr << indent << "{\"Node\":{\n";
		string firstIndent = indent;
		indent += "\t";
		jsonStr << indent << "\"NodeType\":" << current->GetNodeType() << ",\n";
		jsonStr << indent << "\"NodeParam\":" << current->GetParam() << ",\n";

		if (current->GetChildren().size())
		{
			jsonStr << indent << "\"Children\":[\n";
			for (auto& c : current->GetChildren())
			{
				processNode(c.get(), indent + "\t");
			}
			jsonStr << indent << "]\n";
		}

		jsonStr << firstIndent << "},\n";
	};

	//ルートノードから呼び出し
	processNode(root.get(), "\t");

	jsonStr << "}" << endl;


	//ファイル書き込み
	std::ofstream file;

	file.open(path);

	if (file.fail())
	{
		assert(0);
	}

	file << jsonStr.str() << endl;

	file.close();
}
