#include "stdafx.h"
#include "CounterParticle.h"
#include <Util.h>
#include <Player.h>
#include <Easing.h>
#include <Boss.h>

TextureKey CounterParticle::sTexture = "BasicParticle";

CounterParticle::CounterParticle(Float3 pos) : IParticle(pos) {
    firstPos = pos;
    secondPos.x = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);
    secondPos.y = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);
    secondPos.z = (float)Util::RNG(0, 32767) / 32767 * (Util::Chance(50) ? 1 : -1);

	secondPos.SetLength(spread);

    secondPos += Player::Get()->position;
};

void CounterParticle::Update()
{
	IParticle::Update();
	if (time < 30)
	{
		float t = (float)time / 30;
		position = Easing::Out(firstPos, secondPos, t, 5);
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