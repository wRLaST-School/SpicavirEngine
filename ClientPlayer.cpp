#include "stdafx.h"
#include "ClientPlayer.h"
#include <Input.h>
#include <Bullet.h>
#include <SpImGui.h>
#include <GameManager.h>

void ClientPlayer::Init()
{
	GameManager::gameState = GameManager::GameState::ClientPlacing;
	for (size_t i = 0; i < MAX_LINE_TIME; i++)
	{
		line[i] = Vec2(-1000.f, -1000.f);
	}
}

void ClientPlayer::Update()
{
	if (GameManager::gameState == GameManager::GameState::ClientPlacing && !GameManager::isServer)
	{
		if (Input::Mouse::Triggered(Click::Left))
		{
			line[0] = Input::Mouse::GetPos();

			lineTime++;
		}
		else if (Input::Mouse::Down(Click::Left))
		{
			line[lineTime] = Input::Mouse::GetVel();
			lineTime++;
		}

		if (Input::Mouse::Released(Click::Left) || lineTime >= MAX_LINE_TIME)
		{
			lineTime = 0;

			Bullet* b = AddComponent<Bullet>("Bullet");

			for (int i = 0; i < MAX_LINE_TIME; i++)
			{
				b->path[i] = line[i];
				line[i] = Vec2(-1000.f, -1000.f);
			}

			b->size = powSiz;
			b->speed = powSpd;
		}

		if (Input::Key::Triggered(DIK_SPACE))
		{
			GameManager::clientReady = !GameManager::clientReady;
		}
	}

}

void ClientPlayer::Draw()
{
}

void ClientPlayer::UpgradeSize()
{
	coin -= costSiz;

	costSiz += 50;

	powSiz *= 1.15f;
}

void ClientPlayer::UpgradeSpd()
{
	coin -= costSpd;

	costSpd += 50;

	powSpd *= 1.15f;
}

void ClientPlayer::UpgradeNum()
{
	coin -= costNum;

	costNum += 100;

	powNumBul++;
}

void ClientPlayer::UpgradeGain()
{
	coin -= costGain;

	costGain += 50;

	powGain += 25;
}

void ClientPlayer::DrawParams()
{
	ImGui::InputInt("Coin", &coin);

	ImGui::InputFloat("Bullet Size", &powSiz);
	ImGui::InputFloat("Bullet Speed", &powSpd);
	ImGui::InputInt("Bullet Num", &powNumBul);
	ImGui::InputInt("Gains", &powGain);

	ImGui::InputInt("Cost Size", &costSiz);
	ImGui::InputInt("Cost Speed", &costSpd);
	ImGui::InputInt("Cost Num", &costNum);
	ImGui::InputInt("Cost Gains", &costGain);

	if (ImGui::Button("Upgrade Size"))
	{
		UpgradeSize();
	}
	if (ImGui::Button("Upgrade Spd"))
	{
		UpgradeSpd();
	}
	if (ImGui::Button("Upgrade Num"))
	{
		UpgradeNum();
	}
	if (ImGui::Button("Upgrade Gain"))
	{
		UpgradeGain();
	}

	if (ImGui::Button("Activate"))
	{
		Activate();
	}
}

void ClientPlayer::Activate()
{
	auto bullets = GetComponents<Bullet>("Bullet");

	for (auto& b : bullets)
	{
		b->Activate();
	}
}
