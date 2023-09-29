#include "Essentials.h"
#include "SpWindow.h"
#include "SpDirectX.h"
#include "Input.h"
#include "SpSwapChainManager.h"
#include "SpDepth.h"
#include "SpShader.h"
#include "GPipeline.h"
#include "Vec3.h"
#include "SpConstBuffer.h"
#include "Object3D.h"
#include "Matrix.h"
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

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
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

	srand((uint32_t)time(NULL));

	//ウィンドウを生成
	{
		SpWindow wwnd;
		wwnd.Create(L"DirectX", 1920, 1080);
		RegisterSpWindow(wwnd, "Default");
	}

	/*DirectX初期化処理*/
	InitWDX();

	/*デバッグ有効化*/
#ifdef  _DEBUG
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(GetWDX()->dev->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true); 
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif //  _DEBUG

	//Init Input
	Input::Key::Init();
	Input::Pad::Init();
	Input::Mouse::Init();

	/*Init Draw*/
	//ダブルバッファリングとか
	InitWSCM();
	InitWDepth();

	SpTextureManager::Create();

	RootSignatureManager::RegisterAllRS();

	GPipelineManager::CreateAll();

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

	/*ループ*/
	while (true)
	{
		if (GetWDX()->StartFrame()) break;
		Input::Key::Update();
		Input::Pad::Update();
		Input::Mouse::Update();

		/*毎フレーム処理*/

		/*更新処理*/
		SceneManager::Update();
		SpEffekseer::Update();
		/*更新処理ここまで*/

		SceneManager::DrawBack();

		SceneManager::Draw3D();

		SceneManager::DrawSprite();
		
		GetWDX()->PreDrawCommands();

		try {
			Light::UpdateLightData();
		}
		catch (PointLight::QuantityOverflow& e) {
			OutputDebugStringA((std::string("Too Many PointLights Registered. Limit: ") + std::to_string(e.limit) + std::string(", Used: ") + std::to_string(e.actual)).c_str());
			assert(false);
		}

		SpRenderer::Render();

		GetWDX()->PostDrawCommands();

		/*毎フレーム処理ここまで*/
		GetWDX()->EndFrame();

		/*DrawString用のデータを解放*/
		TextDrawer::ReleaseDrawStringData();
		LineDrawer::ClearLineData();
	}

	/*ループここまで*/
	SoundManager::ReleaseAllSounds();
	SpImGui::Shutdown();
	CloseAllSpWindow();

	return 0;
}