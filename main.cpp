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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
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
		wwnd.Create(L"DirectX", 1280, 720);
		RegisterSpWindow(wwnd, "Default");
	}

	/*DirectX初期化処理*/
	InitWDX();
	HRESULT result;

	//Init Input
	Input::Key::Init();
	Input::Pad::Init();

	/*Init Draw*/
	//ダブルバッファリングとか
	InitWSCM();
	InitWDepth();

	SpTextureManager::Create();

	RootSignatureManager::RegisterAllRS();

	GPipelineManager::CreateAll();

	/*Init Draw End*/
	Sprite::InitCommon();

	//Init Scene
	SceneManager sceneManager = SceneManager();
	sceneManager.Init();

	/*デバッグ有効化*/
#ifdef  _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(GetWDX()->dev->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	}
#endif //  _DEBUG
	/*ループ*/
	while (true)
	{
		if (GetWDX()->StartFrame()) break;
		Input::Key::Update();
		Input::Pad::Update();

		/*毎フレーム処理*/

		/*更新処理*/
		sceneManager.Update();
		/*更新処理ここまで*/

		GetWDX()->PreDrawCommands();

		Sprite::PreSpriteDraw();
		sceneManager.DrawBack();

		Sprite::PostSpriteDraw();

		GetWDX()->PreDraw3D();

		try {
			Light::UpdateLightData();
		}
		catch (PointLight::QuantityOverflow& e) {
			assert(false);
		}

		Light::Use();

		sceneManager.Draw3D();

		Sprite::PreSpriteDraw();

		sceneManager.DrawSprite();

		Sprite::PostSpriteDraw();

		GetWDX()->PostDrawCommands();

		/*毎フレーム処理ここまで*/
		GetWDX()->EndFrame();

		/*DrawString用のデータを解放*/
		TextDrawer::ReleaseDrawStringData();
	}
	/*ループここまで*/
	CloseAllSpWindow();

	return 0;
}