#pragma once
#include <BTNodeBase.h>
#include <SpWindow.h>

class BTEditor;
class BehaviorTree;

class BTENode
{
public:
	friend BTEditor;

	BTENode(std::unique_ptr<BT::INode>* node, std::string uniqueName, BTEditor* master);

	void ChangeNodeType(std::string type);

	void Update();
	void Draw();

	const Float2& GetPos();

	void SetComboBoxItem(std::string nodeType);
private:
	std::unique_ptr<BT::INode>* node_;
	Float2 pos_;
	Float2 size_;

	Float2 firstPos_ = { (float)GetSpWindow()->width / 2 - 150, (float)GetSpWindow()->height / 2 - 60 };

	const char* currentItem = nullptr;

	std::string uniqueName_;

	BTEditor* master_;
};

