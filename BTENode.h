#pragma once
#include <BTNodeBase.h>
class BTENode
{
public:
	BTENode(std::unique_ptr<BT::INode>* node, std::string uniqueName);

	void ChangeNodeType(std::string type);

	void Update();
	void Draw();
private:
	std::unique_ptr<BT::INode>* node_;
	Float2 pos_;

	std::string uniqueName_;
};

