#pragma once
#include <SceneManager.h>
class Transition
{
public:
	static void Load();
	static void Update();
	static void Draw();

	template <class NextScene, class ...Args>
	static void Start(Args... args) {
		if (!started)
		{
			SceneManager::LoadScene<NextScene>(args...);
			started = true;
			timer = 0;
		}
	}

private:
	static int32_t timer;
	static bool started;
	const static int32_t blackOutTime = 60;
	const static int32_t totalTime = 120;
};

