#include "stdafx.h"
#include "GameManager.h"
#include <SceneManager.h>
#include <ClientPlayer.h>

bool GameManager::sShowDebug = true;
bool GameManager::sShowHitBoxes = false;
bool GameManager::sDebugImmunity = false;

void GameManager::ManageGameState()
{
	switch (gameState)
	{
	case GameManager::GameState::NotInGame:
		break;
	case GameManager::GameState::ClientPlacing:
		if (serverReady && clientReady)
		{
			gameState = GameState::ServerPlaying;
			if (isServer)
			{
				serverReady = false;
				SceneManager::GetScene()
					->GetComponent<ClientPlayer>("ClientPlayer")
					->ReceiveClientBulletData();
			}
		}
		break;
	case GameManager::GameState::ServerPlaying:
		break;
	case GameManager::GameState::ClientPowerUp:
		break;
	default:
		break;
	}
}
