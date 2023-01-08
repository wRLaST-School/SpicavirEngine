#pragma once
#include "IPostEffector.h"
//輝度テクスチャ生成
class BloomP1 :
    public IPostEffector
{
public:
    static string name;
    static void Init();
    static void Effect(TextureKey baseTex, TextureKey targetTex);
};

//輝度テクスチャをガウスぼかし
class BloomP2 :
    public IPostEffector
{
public:
    static string name;
    static void Init();
    static void Effect(TextureKey baseTex, TextureKey targetTex);
};

//できれば上記二つの操作をまとめる
class Bloom
{
};

