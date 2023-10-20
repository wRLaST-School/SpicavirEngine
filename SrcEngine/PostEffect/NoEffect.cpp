#include "stdafx.h"
#include "NoEffect.h"

void NoEffect::Effect(const TextureKey& baseTex, const TextureKey& targetTex)
{
	IPostEffector::Effect(baseTex, targetTex, name);
}
