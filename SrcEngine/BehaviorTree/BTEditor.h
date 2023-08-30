#pragma once
#include <IBTENode.h>
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

	eastl::list<IBTENode*> editorObjects;

//singleton
public:
	BTEditor(const BTEditor&) = delete;
	BTEditor(BTEditor&&) = delete;
	BTEditor& operator=(const BTEditor&) = delete;
	BTEditor& operator=(BTEditor&&) = delete;

	static BTEditor* GetInstance();

private:
	BTEditor() = default;
	~BTEditor() = default;
};

