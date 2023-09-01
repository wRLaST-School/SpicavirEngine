#include "stdafx.h"
#include "BTENode.h"
#include <BehaviorTree.h>
#include <SpImGui.h>
#include <Camera2D.h>

BTENode::BTENode(std::unique_ptr<BT::INode>* node, std::string uniqueName)
{
	node_ = node;
	(*node_)->editorNodePtr = this;

	uniqueName_ = uniqueName;
}

void BTENode::ChangeNodeType(std::string type)
{
	std::unique_ptr<BT::INode>* lastNode = std::move(node_);
	//NodeType‚É‚æ‚Á‚Ä•ªŠò
	if (type == "Action")
	{
		*node_ = std::make_unique<BT::ActionNode>();
	}
	else if (type == "Loop")
	{
		*node_ = std::make_unique<BT::LoopNode>();
	}
	else if (type == "Selector")
	{
		*node_ = std::make_unique<BT::SelectorNode>();
	}
	else if (type == "Sequencer")
	{
		*node_ = std::make_unique<BT::SequencerNode>();
	}
	else
	{
		*node_ = std::make_unique<BT::SequencerNode>();
	}

	(*node_)->SetParam((*lastNode)->GetParam());
	(*node_)->editorNodePtr = this;
	(*node_)->ChangeParent((*lastNode)->GetParent());
	(*node_)->children_ = std::move((*lastNode)->children_);
	(*node_)->parentBT_ = (*lastNode)->parentBT_;
}

void BTENode::Draw()
{
	SpImGui::Command([&] {
		if(ImGui::Begin(uniqueName_.c_str()))
		{
			ImVec2 currentPos = ImGui::GetWindowPos();
			currentPos.x -= Camera2D::Get()->GetDiff().x;
			currentPos.y -= Camera2D::Get()->GetDiff().y;

			ImGui::SetWindowPos(currentPos);

			static std::vector<std::string> itemList{
				"Action",
				"Sequencer",
				"Selector",
				"Loop",
				"Root"
			};

			static const char* currentItem = nullptr;

			if (ImGui::BeginCombo("NodeType", currentItem))
			{
				for (int i = 0; i < itemList.size(); ++i)
				{
					const bool isSelected = (currentItem == itemList[i].c_str());
					if (ImGui::Selectable(itemList[i].c_str(), isSelected))
					{
						currentItem = itemList[i].c_str();

						ChangeNodeType(itemList[i]);
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			char param[256];
			strcpy_s(param, node_->get()->GetParam().c_str());
			ImGui::InputText("Parameter", param, 256);
			
			node_->get()->SetParam(param);
		}

		ImGui::End();
	});
}
