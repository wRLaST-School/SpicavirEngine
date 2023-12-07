#include "stdafx.h"
#include "ServerPlayer.h"
#include <SpDS.h>
#include <Input.h>
#include <Util.h>

void ServerPlayer::Init()
{
}

void ServerPlayer::Update()
{
	spd = 16.0f;
	auto col = GetComponent<CircleCollider>("CircleCollider");
	if (col)
	{
		Vec2 vMove;

		vMove.x = (float)(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));
		vMove.y = (float)(Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP));

		if (vMove.GetSquaredLength())
		{
			vMove.SetLength(spd);
		}

		col->pos_ = (Vec2)col->pos_ + vMove;

		col->pos_.x = Util::Clamp(col->pos_.x, col->r_, Util::GetWinWidth() - col->r_);
		col->pos_.y = Util::Clamp(col->pos_.y, col->r_, Util::GetWinHeight() - col->r_);
	}
}

void ServerPlayer::Draw()
{
	CircleCollider* col = GetComponent<CircleCollider>("CircleCollider");
	if (col)
	{
		SpDS::DrawBox((int)col->pos_.x, (int)col->pos_.y, 100, 100, 0, Color::Green);
	}
}
