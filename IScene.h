#pragma once
class IScene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DrawBack() = 0;
	virtual void Draw3D() = 0;
	virtual void DrawSprite() = 0;

	virtual ~IScene() {};
};

