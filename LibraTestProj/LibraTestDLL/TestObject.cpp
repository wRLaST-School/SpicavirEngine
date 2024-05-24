#include "TestObject.h"
#include <ScriptComponent.h>
#include <Input.h>
#include <Object3D.h>

void TestObject::Init()
{
}

void TestObject::Update()
{
	auto obj = This()->Parent()->CastTo<Object3D>();

	if (obj)
	{
		float move = (float)Input::Key::Down(DIK_UP) - (float)Input::Key::Down(DIK_DOWN);
		move *= 0.1f;

		obj->position.y += move;

		move = (float)(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));

		move *= 0.1f;

		obj->position.z += move;
	}
}

void TestObject::Draw()
{
}
