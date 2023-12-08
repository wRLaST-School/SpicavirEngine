#include "TestScene.h"
#include "Input.h"
#include <SpDS.h>
#include <SpImGui.h>
#include <SceneManager.h>
#include <ClientScene.h>
#include <ServerScene.h>
#include <GameManager.h>
#include <SceneFromFile.h>
#include <Util.h>
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
		ImGui::Begin("Select Proxy", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
		
		char buf[256];

		strcpy_s(buf, 256 * sizeof(char), GameManager::IP.c_str());

		ImGui::InputText("Server IP", buf, 256);

		GameManager::IP = buf;

		if (ImGui::Button("Client", { 512, 512 }))
		{
			GameManager::isServer = false;
			SceneManager::LoadScene<SceneFromFile>("Assets/Scene/NetworkGame.scene");
		}
		ImGui::SameLine();
		if (ImGui::Button("Server", { 512, 512 }))
		{
			GameManager::isServer = true;
			SceneManager::LoadScene<SceneFromFile>("Assets/Scene/NetworkGame.scene");
		}
		ImVec2 imSiz = ImGui::GetWindowSize();

		Float2 winSiz = Util::GetWinSize();
		ImGui::SetWindowPos({ (winSiz.x - imSiz.x) / 2, (winSiz.y - imSiz.y) / 2 });

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
