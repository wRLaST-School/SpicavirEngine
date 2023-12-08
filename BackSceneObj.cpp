#include "stdafx.h"
#include "BackSceneObj.h"
#include <Input.h>
#include <SceneFromFile.h>
#include <SceneManager.h>
#include <GameManager.h>
#include <SpDS.h>
#include <Util.h>

void BackSceneObj::Update()
{
	if (Input::Key::Triggered(DIK_SPACE))
	{
		GameManager::endGame = true;
	}
}

void BackSceneObj::Draw()
{
	SpDS::DrawRotaGraph((int)Util::GetWinWidth() / 2, (int)Util::GetWinHeight() / 2, 1.f, 1.f, 0.f,
		"result.png");
}
