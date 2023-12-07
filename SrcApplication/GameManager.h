#pragma once
#include <Score.h>

class GameManager {
public:
	static Score sScore;
	static bool sShowDebug;
	static bool sShowHitBoxes;
	static bool sDebugImmunity;
	inline static bool sDebugTimeStop = false;
	inline static std::string IP = "";
	inline static bool isServer = false;
};
