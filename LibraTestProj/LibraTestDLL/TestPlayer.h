#pragma once
#include "IScriptObject.h"
#include <string>

extern "C" __declspec(dllexport) void SetDrawSpriteFunc( void( *dsfunc)(
    int32_t, int32_t, float, float, float, std::string
    ));

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

