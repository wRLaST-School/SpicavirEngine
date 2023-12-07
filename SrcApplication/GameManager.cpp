#include "stdafx.h"
#include "GameManager.h"

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
