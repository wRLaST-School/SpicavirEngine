#pragma once
#include "IScriptObject.h"
class TestPlayer :
    public IScriptObject
{
public:
    void Init();
    void Update();
    void Draw();

private:
    int count = 0;
};

extern "C" __declspec(dllexport) TestPlayer * Create();

