#pragma once
#include <BTNodeBase.h>
class BTENode
{
public:
	BTENode();

	void ChangeNodeType(std::string type);

	void Update();
	void Draw();
private:
	std::unique_ptr<BT::INode> node_;
	Float2 pos_;
};

