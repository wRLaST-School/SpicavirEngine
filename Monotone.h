#pragma once
#include "IPostEffector.h"
class Monotone :
    public IPostEffector
{
public:
    static string name;
    static void Init();
    static void Effect(TextureKey baseTex, TextureKey targetTex);
};

