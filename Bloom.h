#pragma once
#include "IPostEffector.h"
//�P�x�e�N�X�`������
class BloomP1 :
    public IPostEffector
{
public:
    static string name;
    static void Init();
    static void Effect(TextureKey baseTex, TextureKey targetTex);
};

//�P�x�e�N�X�`�����K�E�X�ڂ���
class BloomP2 :
    public IPostEffector
{
public:
    static string name;
    static void Init();
    static void Effect(TextureKey baseTex, TextureKey targetTex);
};

//�ł���Ώ�L��̑�����܂Ƃ߂�
class Bloom
{
};

