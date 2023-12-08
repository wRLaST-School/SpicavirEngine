#include "stdafx.h"
#include "ServerPlayer.h"
#include <SpDS.h>
#include <Input.h>
#include <Util.h>
#include <GameManager.h>
#include <NetworkManager.h>
#include <SceneManager.h>
#include <SceneFromFile.h>

void ServerPlayer::Init()
{
	GameManager::serverReady = false;
	GameManager::clientReady = false;

	auto col = GetComponent<CircleCollider>("CircleCollider");
	col->pos_ = { (float)Util::GetWinWidth() / 4 , (float)Util::GetWinHeight() / 2 };
}

void ServerPlayer::Update()
{
	if (hp <= 0)
	{
		SceneManager::LoadScene<SceneFromFile>("Assets/Scene/back.scene");

		SceneManager::WaitForLoadAndTransition();
	}

	spd = 16.0f;
	auto col = GetComponent<CircleCollider>("CircleCollider");
	if (col)
	{
		col->r_ = 16.f;
		if (GameManager::isServer)
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

			SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
				"server_tutorial.png");

			if (GameManager::gameState == GameManager::GameState::ClientPlacing)
			{
				SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
					"ready_tutorial.png");


				SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
					"Ready.png", Anchor::Center, GameManager::serverReady ? Color::Green : Color::White);
			}
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
		SpDS::DrawCircle(
			static_cast<int32_t>(col->pos_.x),
			static_cast<int32_t>(col->pos_.y),
			static_cast<int32_t>(col->r_),
			Color::White
		);

		if (hp >= 2)
		{
			SpDS::DrawBoxLine((int)col->pos_.x -100, (int)col->pos_.y, 25, 25, Color::Green, 2.f, Anchor::CenterRight);
		}
		if (hp >= 3)
		{
			SpDS::DrawBoxLine((int)col->pos_.x, (int)col->pos_.y + 100, 25, 25, Color::Green, 2.f, Anchor::TopCenter);
		}
		if (hp >= 4)
		{
			SpDS::DrawBoxLine((int)col->pos_.x + 100, (int)col->pos_.y, 25, 25, Color::Green, 2.f, Anchor::CenterLeft);
		}
		if (hp >= 5)
		{
			SpDS::DrawBoxLine((int)col->pos_.x, (int)col->pos_.y - 100, 25, 25, Color::Green, 2.f, Anchor::BottomCenter);
		}
	}
}

void ServerPlayer::Damage()
{
	hp--;
}
