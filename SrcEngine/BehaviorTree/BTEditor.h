#pragma once
#include <BTENode.h>
#include <BehaviorTree.h>
#include <Camera2D.h>
#include <EASTL/list.h>

class BTEditor final 
{
public:
	static void Update();
	static void Draw();

private:
	BT::BehaviorTree tree_;
	Camera2D cam_;

	eastl::list<BTENode> editorObjects;

	uint64_t id = 0;

//singleton
public:
	BTEditor(const BTEditor&) = delete;
	BTEditor(BTEditor&&) = delete;
	BTEditor& operator=(const BTEditor&) = delete;
	BTEditor& operator=(BTEditor&&) = delete;

	static BTEditor* GetInstance();

private:
	BTEditor();
	~BTEditor() = default;
};

