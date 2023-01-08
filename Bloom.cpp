#include "stdafx.h"
#include "Bloom.h"


string BloomP1::name = "BloomP1";

string BloomP2::name = "BloomP2";

void BloomP1::Init()
{
	RegisterRS(name);
	RegisterPipeline(name);
}

void BloomP1::Effect(TextureKey baseTex, TextureKey targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name);
}

void BloomP2::Init()
{
	RegisterRS(name);
	RegisterPipeline(name);
}

void BloomP2::Effect(TextureKey baseTex, TextureKey targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name);
}
