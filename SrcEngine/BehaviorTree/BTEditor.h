#pragma once
#include <IBTENode.h>

class BTEditor final 
{
public:
	static void Update();
	static void Draw();

private:
	

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

