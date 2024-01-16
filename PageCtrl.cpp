#include "stdafx.h"
#include "PageCtrl.h"
#include <Input.h>
#include <SceneFromFile.h>
#include <SceneManager.h>
#include <imgui.h>

void PageCtrl::Update()
{
	bool change = false;
	if (Input::Key::Triggered(DIK_RETURN) || Input::Mouse::Triggered(Click::Left))
	{
		page++;
		if (page >= files.size())
		{
			page = (int32_t)files.size() - 1;
		}

		change = true;
	}

	if (Input::Key::Triggered(DIK_BACK) || Input::Mouse::Triggered(Click::Right))
	{
		page--;
		if (page < 0)
		{
			page = 0;
		}

		change = true;
	}

	if (change)
	{
		SceneManager::LoadScene<SceneFromFile>(files.at(page));

		SceneManager::WaitForLoadAndTransition();

		change = false;
	}
}

void PageCtrl::DrawParams()
{
	ImGui::InputInt("Page", &page);
}
