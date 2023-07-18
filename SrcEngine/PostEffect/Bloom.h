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

//�P�x�e�N�X�`������
class BloomP1 :
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

//�P�x�e�N�X�`�����K�E�X�ڂ���(X)
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

//�P�x�e�N�X�`�����K�E�X�ڂ���(Y)
class BloomP3 :
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& targetTex);
};

//�ł���Ώ�L��̑�����܂Ƃ߂�
class BloomFin:
    public IPostEffector
{
public:
    static std::string name;
    static void Init();
    static void Effect(const TextureKey& baseTex, const TextureKey& p3Tex, const TextureKey& targetTex);
};