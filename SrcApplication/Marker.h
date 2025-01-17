#pragma once
#include <Object3D.h>
#include <ComponentFactory.h>
class Marker : public IComponent
{
public:
	ComponentFactoryRegister(Marker)

	Marker();

	void Init(const Float3& pos);

	void Cast(const Float3& pos);

	void Update();
	void Draw();

	bool IsActive();
private:
	//Object3D outerObj;
	//Object3D innerObj;
	bool active_ = false;

	int32_t timer_ = 0;

	Float3 pos_;

	const static int32_t FIN_TIME = 60;

	const static TextureKey TEX_KEY;

	const static int32_t R = 4;
};

