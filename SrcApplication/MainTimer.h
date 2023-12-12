#pragma once
#include <SpTextureManager.h>
#include <ComponentFactory.h>
class MainTimer : public IComponent
{
public:
	ComponentFactoryRegister(MainTimer)

	void Load();
	void Init() override;

	void Update();
	void Draw();

	void Damage();

	int32_t timerSec = 120;

private:
	int32_t frameTimer_ = 60;
	const int32_t FRAME_TIME = 60;
	std::vector<TextureKey> numbers_ = {
		"num_0",
		"num_1",
		"num_2",
		"num_3",
		"num_4",
		"num_5",
		"num_6",
		"num_7",
		"num_8",
		"num_9"
	};
	int32_t damageTimer_ = 0;
};