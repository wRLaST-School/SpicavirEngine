#include "IScene.h"

void IScene::UpdateAllComponents()
{
	for (auto& component : components_)
	{
		component.second->Update();
	}
}

void IScene::DrawAllComponents()
{
	for (auto& component : components_)
	{
		component.second->Draw();
	}
}
