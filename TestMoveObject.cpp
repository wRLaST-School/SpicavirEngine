#include "stdafx.h"
#include "TestMoveObject.h"
#include <Object3D.h>

#include <SpImGui.h>
#include <TextDrawer.h>

void TestMoveObject::Update()
{
	Object3D* parent = Parent()->CastTo<Object3D>();

	parent;
}

void TestMoveObject::Draw()
{
}

void TestMoveObject::DrawParams()
{
}
