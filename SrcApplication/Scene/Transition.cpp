#include "stdafx.h"
#include "Transition.h"
#include <SpDS.h>
#include <Easing.h>

int32_t Transition::timer = 0;
bool Transition::started = false;

void Transition::Load()
{
	//SpTextureManager::LoadTexture("Resources/Loading.png", "Loading");
}

void Transition::Update()
{
	if (timer >= totalTime)
	{
		started = false;
	}
	
	if (started)
	{
		if (timer == blackOutTime && SceneManager::GetLoadState() == SceneManager::LoadState::Loaded)
		{
			SceneManager::Transition();
		}

		if (timer < blackOutTime || SceneManager::GetLoadState() != SceneManager::LoadState::Loading)
			timer++;
	}
}

void Transition::Draw()
{
	if (started)
	{
		if (timer <= blackOutTime)
		{
			SpDS::DrawBox(0, 0, GetSpWindow()->width, GetSpWindow()->height,
				Color(0.f, 0.f, 0.f, Easing::In((float)timer / blackOutTime)));
		}
		else
		{
			float t = ((float)timer - (float)blackOutTime) / ((float)totalTime - (float)blackOutTime);
			t = Easing::In(t);
			
			SpDS::DrawBox(0, 0, GetSpWindow()->width, GetSpWindow()->height,
				Color(0.f, 0.f, 0.f, 1.0f - t));
		}

		if (SceneManager::GetLoadState() == SceneManager::LoadState::Loading)
		{
			//読み込み中の描画処理
		}
	}
}
