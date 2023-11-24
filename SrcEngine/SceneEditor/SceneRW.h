#pragma once
#include <IScene.h>
class SceneRW
{
public:
	static void SaveScene(IScene* scene, std::string filePath);

	static void ConfirmLoadScene();

	static void LoadScene(IScene* scene, std::string filePath);

private:
	IScene* scene_;
	std::string filePath_;

	bool waitingForLoad_ = false;

public://singleton
	SceneRW(const SceneRW&) = delete;
	SceneRW(SceneRW&&) = delete;
	SceneRW& operator=(const SceneRW&) = delete;
	SceneRW& operator=(SceneRW&&) = delete;

	static SceneRW* GetInstance();

private:
	SceneRW() {};
	~SceneRW() = default;
};

