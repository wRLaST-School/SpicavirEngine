#include "stdafx.h"
#include "BTEditor.h"
#include <SpImGui.h>

#include <fstream>
#include <iostream>
#include <sstream>

#pragma warning (push)
#pragma warning (disable:26800)
#pragma warning (push, 1)
#include <SrcExternal/json.hpp>
#pragma warning (pop)
#pragma warning (pop)

using namespace BT;

using namespace nlohmann;
using namespace std;

void BTEditor::Draw()
{
    for (auto& o : GetInstance()->editorObjects)
    {
        o.Draw();
    }


    SpImGui::Command([&] {
		
        if (ImGui::Begin("Master", nullptr, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save")) {
                        GetInstance()->tree_.SaveJson(GetInstance()->filePath);
                    }
                    if (ImGui::MenuItem("Load")) {
						LoadFile(GetInstance()->filePath);
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            char path[256];
            strcpy_s(path, GetInstance()->filePath.c_str());
            ImGui::InputText("FilePath", path, 256);

            GetInstance()->filePath = path;

            if (ImGui::Button("New Node"))
            {
                BTEditor* ins = GetInstance();
                ins->tree_.root->AddNode<BT::ActionNode>("");
                std::string uniqueName = "##";
                uniqueName += std::to_string(ins->id);
                ins->id++;
                ins->editorObjects.emplace_back(ins->tree_.root->LastPtr(), uniqueName, ins);
                ins->tree_.root->Last()->editorNodePtr = &ins->editorObjects.back();
            }
        }
        ImGui::End();
    });
}

BTENode* BTEditor::GetSelected()
{
    return selected_;
}

void BTEditor::ClearSelected()
{
    selected_ = nullptr;
}

void BTEditor::SetSelected(BTENode* node)
{
    selected_ = node;
}

void BTEditor::DeleteNode(BTENode* node)
{
	if (node->node_->get()->GetChildren().size())
	{
		ImGui::OpenPopup("Error Deleting Node");
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));


		if (ImGui::BeginPopupModal("Error Deleting Node", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::SetItemDefaultFocus();
			ImGui::Text("Do Not Delete a Node with Children!\n\n");

			ImGui::Separator();

			if (ImGui::Button("OK")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		return;
	}

	std::list<std::unique_ptr<INode>>* childrenOfParent = &node->node_->get()->GetParent()->children_;

	for (auto itr = childrenOfParent->begin(); itr != childrenOfParent->end(); itr++)
	{
		if (&(*itr) == node->node_)
		{
			childrenOfParent->erase(itr);
		}
	}

	for (auto itr = editorObjects.begin(); itr != editorObjects.end(); itr++)
	{
		if (&(*itr) == node)
		{
			editorObjects.erase(itr);
			break;
		}
	}
}

void BTEditor::LoadFile(std::string filePath)
{
	//インスタンス保存
	BTEditor* ins = GetInstance();

	//ファイルパス変数を更新
	ins->filePath = filePath;

	//ファイル読み込み
	std::ifstream file;

	file.open(filePath);

	if (file.fail())
	{
		assert(0);
	}

	json deserialized;
	file >> deserialized;

	//ツリーのリセット
	ins->tree_.root = make_unique<BT::RootNode>();
	dynamic_cast<BT::RootNode*>(ins->tree_.root.get())->SetRootBT(&ins->tree_);
	ins->editorObjects.clear();

	//Editor Nodeを追加
	std::string uniqueName = "##";
	uniqueName += std::to_string(ins->id);
	ins->id++;
	ins->editorObjects.emplace_back(&ins->tree_.root, uniqueName, ins);
	ins->tree_.root->editorNodePtr = &ins->editorObjects.back();
	ins->tree_.root->editorNodePtr->SetComboBoxItem("Root");

	//Assert
	assert(deserialized.is_object());
	assert(deserialized.contains("Node0"));
	assert(deserialized["Node0"]["NodeType"] == "Root");

	//ツリーの構築
	//読み込みの再起関数
	function<void(const json& object, INode* parent)> processNode = [&](const json& object, INode* parent)
		{
			string nodeType = object["NodeType"].get<string>();
			//NodeTypeによって分岐
			if (nodeType == "Action")
			{
				parent->AddNode<ActionNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Loop")
			{
				parent->AddNode<LoopNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Selector")
			{
				parent->AddNode<SelectorNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Sequencer")
			{
				parent->AddNode<SequencerNode>(object["NodeParam"].get<string>());
			}
			else if (nodeType == "Condition")
			{
				parent->AddNode<ConditionNode>(object["NodeParam"].get<string>());
			}
			else
			{
				parent->AddNode<SequencerNode>("");
			}

			//Editor Nodeを追加
			std::string uniqueName = "##";
			uniqueName += std::to_string(ins->id);
			ins->id++;
			ins->editorObjects.emplace_back(parent->LastPtr(), uniqueName, ins);
			parent->Last()->editorNodePtr = &ins->editorObjects.back();

			if (object.contains("NodePosX"))
			{
				parent->Last()->editorNodePtr->firstPos_.x = std::stof(object["NodePosX"].get<string>());
			}
			if (object.contains("NodePosY"))
			{
				parent->Last()->editorNodePtr->firstPos_.y = std::stof(object["NodePosY"].get<string>());
			}
			parent->Last()->editorNodePtr->SetComboBoxItem(object["NodeType"].get<string>());

			//子ノードの読み込み
			if (object.contains("Children"))
			{

				for (auto& node : object["Children"])
				{
					for (auto& singleNode : node)
					{
						processNode(singleNode, parent->Last());
					}
				}
			}
		};

	//読み込み再起関数呼び出し
	if (deserialized["Node0"].contains("Children"))
	{
		for (auto& node : deserialized["Node0"]["Children"])
		{
			processNode(node["Node0"], ins->tree_.root.get());
		}
	}

	file.close();
}

BTEditor* BTEditor::GetInstance()
{
    static BTEditor ins;
    return &ins;
}

BTEditor::BTEditor()
{
    std::string uniqueName = "##";
    uniqueName += std::to_string(id++);

    editorObjects.emplace_back(&tree_.root, uniqueName, this);
}