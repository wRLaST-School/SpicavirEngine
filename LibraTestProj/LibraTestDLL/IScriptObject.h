#pragma once
class IScriptObject
{
public:
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnInspectorWindowDraw() {};
};

