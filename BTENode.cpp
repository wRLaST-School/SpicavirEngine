#include "stdafx.h"
#include "BTENode.h"
#include <BehaviorTree.h>
#include <SpImGui.h>
#include <Camera2D.h>
#include <BTEditor.h>

BTENode::BTENode(std::unique_ptr<BT::INode>* node, std::string uniqueName, BTEditor* master)
{
	node_ = node;
	(*node_)->editorNodePtr = this;

	uniqueName_ = uniqueName;

	master_ = master;
}

void BTENode::ChangeNodeType(std::string type)
{
	std::unique_ptr<BT::INode>* lastNode = std::move(node_);

	BT::INode* last = lastNode->get();

	std::string param = last->GetParam();
	BT::INode* parent = last->parent_;
	auto children = std::move(last->children_);
	auto parentBT = last->parentBT_;

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
		*node_ = std::make_unique<BT::RootNode>();
	}

	(*node_)->SetParam(param);
	(*node_)->editorNodePtr = this;
	(*node_)->parentBT_ = parentBT;
	(*node_)->children_ = std::move(children);
	(*node_)->parent_ = parent;

	for (auto& c : (*node_)->children_)
	{
		c->parent_ = node_->get();
	}
}

void BTENode::Draw()
{
	SpImGui::Command([&] {
		std::string wName = std::string("Node") + uniqueName_;
		if(ImGui::Begin(wName.c_str()))
		{
			ImVec2 currentPos = ImGui::GetWindowPos();
			currentPos.x -= Camera2D::Get()->GetDiff().x;
			currentPos.y -= Camera2D::Get()->GetDiff().y;

			ImGui::SetWindowPos(currentPos);
			pos_ = { currentPos.x, currentPos.y };

			ImVec2 currentSize = ImGui::GetWindowSize();
			size_ = { currentSize.x, currentSize.y };

			static std::vector<std::string> itemList{
				"Action",
				"Sequencer",
				"Selector",
				"Loop",
				"Root"
			};

			if (master_->GetSelected())
			{
				if (ImGui::Button("Select"))
				{
					node_ = node_->get()->ChangeParent(master_->GetSelected()->node_->get());
					master_->ClearSelected();
				}
			}

			std::string comboName = std::string("NodeType") + uniqueName_;
			if (ImGui::BeginCombo(comboName.c_str(), currentItem))
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

			if (ImGui::Button("Select Child"))
			{
				master_->SetSelected(this);
			}
		}

		ImGui::End();
	});

	if (node_->get()->parent_)
	{
		auto parent = node_->get()->parent_->editorNodePtr;
		SpDS::DrawLine((int32_t)parent->pos_.x + (int32_t)(parent->size_.x / 2), (int32_t)parent->pos_.y + (int32_t)parent->size_.y, (int32_t)pos_.x + (int32_t)(size_.x / 2), (int32_t)pos_.y, Color::White);
	}
}
