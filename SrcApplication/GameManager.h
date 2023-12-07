#pragma once

class GameManager {
public:
	static bool sShowDebug;
	static bool sShowHitBoxes;
	static bool sDebugImmunity;
	inline static bool sDebugTimeStop = false;
	inline static std::string IP = "";
	inline static bool isServer = false;
};
