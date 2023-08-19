#include "stdafx.h"
#include "BTNodeBase.h"

void BT::INode::OnAbort()
{
	OnEnd();
}

BT::INode* BT::INode::Last()
{
	return children_.back();
}
