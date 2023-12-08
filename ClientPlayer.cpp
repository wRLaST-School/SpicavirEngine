#include "stdafx.h"
#include "ClientPlayer.h"
#include <Input.h>
#include <Bullet.h>
#include <SpImGui.h>
#include <GameManager.h>
#include <SpDS.h>
#include <SceneManager.h>
#include <NetworkManager.h>
#include <Util.h>

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
	auto& clnData = SceneManager::GetScene()
		->GetComponent<NetworkManager>("NetworkManager")
		->clnData;

	if (!GameManager::isServer)
	{
		clnData.powGain = powGain;
		clnData.powNumBul = powNumBul;
		clnData.powSpd = powSpd;
		clnData.powSiz = powSiz;
		clnData.isReady = GameManager::clientReady;
	}
	else
	{
		powGain =   clnData.powGain;
		powNumBul = clnData.powNumBul;
		powSpd =    clnData.powSpd;
		powSiz =    clnData.powSiz;
		GameManager::clientReady = clnData.isReady;
	}

	if (GameManager::gameState == GameManager::GameState::ClientPlacing && !GameManager::isServer)
	{
		SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
			"line_tutorial.png"); 
		SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
			"ready_tutorial.png");

		SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
			"Ready.png", Anchor::Center, GameManager::clientReady ? Color::Green : Color::White);

		if (Input::Mouse::Triggered(Click::Left) && curBulletNum < powNumBul)
		{
			line[0] = Input::Mouse::GetPos();

			lineTime++;
		}
		else if (Input::Mouse::Down(Click::Left) && curBulletNum < powNumBul)
		{
			line[lineTime] = Input::Mouse::GetVel();
			lineTime++;
		}

		if ((Input::Mouse::Released(Click::Left) || lineTime >= MAX_LINE_TIME) && curBulletNum < powNumBul)
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

			for (int i = 0; i < 600; i++)
			{
				clnData.bullets[curBulletNum].path[i] = b->path[i];
			}
			clnData.bullets[curBulletNum].active = true;
			curBulletNum++;
		}

		if (Input::Key::Triggered(DIK_SPACE))
		{
			GameManager::clientReady = !GameManager::clientReady;
		}
	}

	if (GameManager::gameState == GameManager::GameState::ServerPlaying)
	{
		Activate();
		curBulletNum = 0;

		if (GetComponents<Bullet>("Bullet").size() == 0)
		{
			GameManager::gameState = GameManager::GameState::ClientPowerUp; for (int i = 0; i < 64; i++)
			{
				clnData.bullets[i].active = false;
			}

			coin += powGain;
		}
	}

	if (GameManager::gameState == GameManager::GameState::ClientPowerUp && !GameManager::isServer)
	{
		GameManager::clientReady = false;
		SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
			"powerup_tutorial.png");
		//TODO: パワーアップ選択UIにする
		if (Input::Key::Triggered(DIK_R))
		{
			GameManager::gameState = GameManager::GameState::ClientPlacing;
		}
	}
	if (GameManager::gameState == GameManager::GameState::ClientPowerUp && GameManager::isServer)
	{
		GameManager::gameState = GameManager::GameState::ClientPlacing;
	}
}

void ClientPlayer::Draw()
{
	if (lineTime)
	{
		Vec2 lastPos = line[0];

		for (int i = 1; i < lineTime; i++)
		{
			SpDS::DrawLine((int32_t)lastPos.x, (int32_t)lastPos.y, (int32_t)((lastPos+line[i]).x), (int32_t)((lastPos+line[i]).y), Color::White, 3);
			lastPos += line[i];
		}
	}
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

void ClientPlayer::ReceiveClientBulletData()
{
	auto& clnData = SceneManager::GetScene()
		->GetComponent<NetworkManager>("NetworkManager")
		->clnData;
	for (int i = 0; i < 64; i++)
	{
		if (clnData.bullets[i].active)
		{
			Bullet* b = AddComponent<Bullet>("Bullet");

			for (int j = 0; j < MAX_LINE_TIME; j++)
			{
				b->path[j] = clnData.bullets[i].path[j];
			}

			b->size = powSiz;
			b->speed = powSpd;
		}
	}
}
