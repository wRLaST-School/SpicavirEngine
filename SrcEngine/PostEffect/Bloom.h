#pragma once
#include "IPostEffector.h"
#include <SpConstBuffer.h>

struct GaussianWeight
{
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    float g;
    float h;
} GetGaussianWeight(float strength);

//輝度テクスチャ生成
class BloomP1 :
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

//輝度テクスチャをガウスぼかし(X)
class BloomP2 :
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);

    static SpConstBuffer<GaussianWeight> cb;
    static float strength;
};

//輝度テクスチャをガウスぼかし(Y)
class BloomP3 :
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

//できれば上記二つの操作をまとめる
class BloomFin:
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& p3Tex, const TextureKey& targetTex);
};