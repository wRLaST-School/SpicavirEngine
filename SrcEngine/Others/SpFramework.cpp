#include "stdafx.h"
#include "SpFramework.h"
#include "Essentials.h"
#include "SpWindow.h"
#include "SpDirectX.h"
#include "Input.h"
#include "SpSwapChainManager.h"
#include "SpDepth.h"
#include "SpShader.h"
#include "GPipeline.h"
#include "SpRootSignature.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "TextDrawer.h"
#include "GPipelineManager.h"
#include <RootSignatureManager.h>
#include <LineDrawer.h>
#include <SpImGui.h>
#include <IPostEffector.h>
#include <SoundManager.h>
#include <SpRenderer.h>
#include <SpDS.h>
#include <SpEffekseer.h>
#include <AssetBrowser.h>
#include <GameManager.h>
#include <HierarchyPanel.h>
#include <DockPanel.h>
#include <InspectorWindow.h>
#include <ResourceWindow.h>
#include <CustomComponentRegisterer.h>

void SpFramework::Init()
{
	/*デバッグ有効化*/
#ifdef  _DEBUG
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif //  _DEBUG

	//ウィンドウを生成
	{
		SpWindow wwnd;
		wwnd.Create(L"DirectX", 1920, 1080);
		RegisterSpWindow(wwnd, "Default");
	}

	/*DirectX初期化処理*/
	InitSpDX();

	/*デバッグ有効化*/
#ifdef  _DEBUG
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(GetSpDX()->dev->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif //  _DEBUG

	//Init Input
	Input::Key::Init();
	Input::Pad::Init();
	Input::Mouse::Init();

	/*Init Draw*/
	//ダブルバッファリングとか
	InitSpSCM();

	SpTextureManager::Create();
	InitSpDepth();

	RootSignatureManager::RegisterAllRS();

	GPipelineManager::CreateAll();

	Light::GetInstance();

	/*Init Draw End*/
	Sprite::InitCommon();

	//Init PostEffecter
	PostEffectCommon::Init();

	//Init Textures
	SpTextureManager::Init();

	//Init Line Drawer
	LineDrawer::Init();

	//Init ImGui
	SpImGui::Init();

	//Init Renderer
	SpRenderer::Init();

	//Init DS
	SpDS::CreateBuffers();

	//Init Sounds
	SoundManager::Init();

	//Init Effects
	SpEffekseer::Init();

	//Init Scene
	SceneManager::Init();

	//Load Asset Browser Resources
	AssetBrowser::LoadResources();

	//Register Components to Factory
	CustomComponentRegisterer::CallRegisters();
}

void SpFramework::Run()
{
	while (true)
	{
		if (GetSpDX()->StartFrame()) break;
		Input::Key::Update();
		Input::Pad::Update();
		Input::Mouse::Update();

		/*毎フレーム処理*/
		SceneManager::currentScene->CheckDelete();

		DockPanel::EnableScreenDock();

		SceneManager::DrawBack();

		/*更新処理*/
		SceneManager::Update();
		SpEffekseer::Update();
		/*更新処理ここまで*/;

		SceneManager::Draw3D();

		SceneManager::DrawSprite();

		if (GameManager::sShowDebug)
		{
			AssetBrowser::SDraw();

			HierarchyPanel::SDraw();

			InspectorWindow::SDraw();

			ResourceWindow::SDraw();

			SpImGui::Command([&] {
				if (ImGui::Begin("Game Scene"))
				{
					ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
					ImGui::Checkbox("Show Hit Box", &GameManager::sShowHitBoxes);
					ImGui::Checkbox("Debug Immunity", &GameManager::sDebugImmunity);
					ImGui::Checkbox("Pause Game", &GameManager::sDebugTimeStop);
				}
				ImGui::End();
				});
		}

		GetSpDX()->PreDrawCommands();

		try {
			Light::UpdateLightData();
		}
		catch (PointLight::ReachedLightLimit& e) {
			OutputDebugStringA((std::string("Too Many PointLights Registered. Limit: ") + std::to_string(e.limit) + std::string(", Used: ") + std::to_string(e.actual)).c_str());
			assert(false);
		}

		SpRenderer::Render();

		SpImGui::EndFrame();

		GetSpDX()->PostDrawCommands();

		/*毎フレーム処理ここまで*/
		GetSpDX()->EndFrame();

		/*DrawString用のデータを解放*/
		TextDrawer::ReleaseDrawStringData();
		LineDrawer::ClearLineData();
	}

	/*ループここまで*/
	SoundManager::ReleaseAllSounds();
	SceneManager::ReleaseScene();
	SpImGui::Shutdown();
	CloseAllSpWindow();
}
