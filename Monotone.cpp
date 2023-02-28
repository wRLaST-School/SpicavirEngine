#include "stdafx.h"
#include "Monotone.h"

string Monotone::name = "Monotone";

void Monotone::Init()
{
	RegisterRS(name);
	RegisterPipeline(name);
}

void Monotone::Effect(TextureKey baseTex, TextureKey targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name);
}
