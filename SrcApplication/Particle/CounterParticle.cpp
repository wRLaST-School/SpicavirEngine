#include "stdafx.h"
#include "CounterParticle.h"
#include <Util.h>
#include <Player.h>
#include <Easing.h>
#include <Boss.h>

TextureKey CounterParticle::sTexture = "BasicParticle";

CounterParticle::CounterParticle(Float3 pos) : IParticle(pos) {
    firstPos_ = pos;
    secondPos_.x = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);
    secondPos_.y = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);
    secondPos_.z = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);

	secondPos_.SetLength(spread_);

    secondPos_ += Player::Get()->position;
};

void CounterParticle::Update()
{
	IParticle::Update();
	if (time < 30)
	{
		float t = (float)time / 30;
		position = Easing::Out(firstPos_, secondPos_, t, 5);
	}
	else
	{
		float t = (float)(time - 30) / 60;
		position = Vec3::Lerp(position, Boss::Get()->position, t);

		if (time - 30 == 15)
		{
			Boss::Get()->Damage(50);
		}
	}

	color = { 1.f, 1.f, 1.f, 1.f };
}