#pragma once

class GameManager {
public:
	static bool sShowDebug;
	static bool sShowHitBoxes;
	static bool sDebugImmunity;
	inline static bool sDebugTimeStop = false;
	inline static std::string IP = "";
	inline static bool isServer = false;

	inline static enum class GameState {
		NotInGame,
		ClientPlacing,
		ServerPlaying,
		ClientPowerUp
	} gameState = GameState::NotInGame;

	inline static bool serverReady = false;
	inline static bool clientReady = false;

	inline static int gameTimer = 0;

	static void ManageGameState();

	inline static bool endGame = false;
};
