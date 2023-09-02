#pragma once
#include "IRenderStage.h"
#include <Object3D.h>
class SrAlphaStage :
    public IRenderStage
{
public:
    void Init();
    void PreDraw();
    void PostDraw();

    void Render();
    void DrawCommands(std::function<void(void)> cmd);

    //Render‚ÌÅ‰‚ÉŒÄ‚Ô
    void SortObjects();
    void RegisterAlphaObject(Object3D* obj);

private:
    std::list<Object3D*> objects_;
    std::list<Object3D*> sortedObj_;
};

