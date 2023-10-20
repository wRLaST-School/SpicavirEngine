#pragma once
#include "IPostEffector.h"
class NoEffect :
    public IPostEffector
{
public:
    inline static std::string name = "Finalize";

    static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

