#include "stdafx.h"
#include "ServerPlayer.h"
#include <SpDS.h>
#include <Input.h>
#include <Util.h>
#include <GameManager.h>
#include <NetworkManager.h>
#include <SceneManager.h>

void ServerPlayer::Init()
{
	GameManager::serverReady = false;
	GameManager::clientReady = false;
}

void ServerPlayer::Update()
{
	spd = 16.0f;
	auto col = GetComponent<CircleCollider>("CircleCollider");
	if (col)
	{
		col->r_ = 16.f;
		if (true  && GameManager::isServer)
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

			SceneManager::currentScene->GetComponent<NetworkManager>("NetworkManager")->srvData.playerPos = col->pos_;
			SceneManager::currentScene->GetComponent<NetworkManager>("NetworkManager")->srvData.playerHealth = hp; 
			
			if (Input::Key::Triggered(DIK_SPACE))
			{
				GameManager::serverReady = !GameManager::serverReady;
			}
			SceneManager::currentScene->GetComponent<NetworkManager>("NetworkManager")->srvData.isReady = GameManager::serverReady;

		}
		else
		{
			col->pos_ = SceneManager::currentScene->GetComponent<NetworkManager>("NetworkManager")->srvData.playerPos;
			hp = SceneManager::currentScene->GetComponent<NetworkManager>("NetworkManager")->srvData.playerHealth;
			GameManager::serverReady = SceneManager::currentScene->GetComponent<NetworkManager>("NetworkManager")->srvData.isReady;
		}
	}

	GameManager::ManageGameState();
}

void ServerPlayer::Draw()
{
	CircleCollider* col = GetComponent<CircleCollider>("CircleCollider");
	if (col)
	{
		SpDS::DrawBoxLine((int)col->pos_.x, (int)col->pos_.y, 100, 100, Color::Green, 3.f);
	}
}

void ServerPlayer::Damage()
{
	hp--;
}
