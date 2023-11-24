#include "TestScene.h"
#include "Input.h"
#include <SpDS.h>
#include <SpImGui.h>
#include <SceneManager.h>
#include <ClientScene.h>
#include <ServerScene.h>
#include <GameManager.h>
void TestScene::LoadResources()
{
	
}

void TestScene::Init()
{
	Camera::Set(cam);
}

void TestScene::Update()
{
	SpImGui::Command([&] {
		ImGui::SetNextWindowPos({ 500.f, 200.f });
		ImGui::Begin("Select Proxy", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		
		char buf[256];

		strcpy_s(buf, 256 * sizeof(char), GameManager::IP.c_str());

		ImGui::InputText("Server IP", buf, 256);

		GameManager::IP = buf;

		if (ImGui::Button("Client", { 200, 400 }))
		{
			SceneManager::LoadScene<ClientScene>();
		}
		ImGui::SameLine();
		if (ImGui::Button("Server", { 200, 400 }))
		{
			SceneManager::LoadScene<ServerScene>();
		}

		ImGui::End();
	});

	SceneManager::Transition();
}

void TestScene::DrawBack()
{

}

void TestScene::Draw3D()
{
	
}

void TestScene::DrawSprite()
{

}
