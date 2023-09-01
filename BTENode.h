#pragma once
#include <BTNodeBase.h>

class BTEditor;

class BTENode
{
public:
	BTENode(std::unique_ptr<BT::INode>* node, std::string uniqueName, BTEditor* master);

	void ChangeNodeType(std::string type);

	void Update();
	void Draw();
private:
	std::unique_ptr<BT::INode>* node_;
	Float2 pos_;
	Float2 size_;

	const char* currentItem = nullptr;

	std::string uniqueName_;

	BTEditor* master_;
};

