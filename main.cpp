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
#include <assimp/Importer.hpp>
#include <IPostEffector.h>
#include <SoundManager.h>
#include <SpRenderer.h>
#include <SpDS.h>
#include <TimeManager.h>
#include <Toolbar.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	/*�f�o�b�O�L����*/
#ifdef  _DEBUG
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif //  _DEBUG

	//�E�B���h�E�𐶐�
	{
		SpWindow wwnd;
		wwnd.Create(L"DirectX", 1280, 720);
		RegisterSpWindow(wwnd, "Default");
	}

	/*DirectX����������*/
	InitWDX();

	/*�f�o�b�O�L����*/
#ifdef  _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(GetWDX()->dev->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	}
#endif //  _DEBUG

	//Init Input
	Input::Key::Init();
	Input::Pad::Init();
	Input::Mouse::Init();

	/*Init Draw*/
	//�_�u���o�b�t�@�����O�Ƃ�
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

	//Init Scene
	SceneManager::Init();

	//Init TimeManager
	TimeManager::Init();

	/*���[�v*/
	while (true)
	{
		if (GetWDX()->StartFrame()) break;
		TimeManager::FrameStart();
		Input::Key::Update();
		Input::Pad::Update();
		Input::Mouse::Update();

		/*���t���[������*/

		/*�X�V����*/
		SceneManager::Update();
		/*�X�V���������܂�*/

		SceneManager::DrawBack();

		SceneManager::Draw3D();

		SceneManager::DrawSprite();
		
		GetWDX()->PreDrawCommands();

		try {
			Light::UpdateLightData();
		}
		catch (PointLight::QuantityOverflow& e) {
			OutputDebugStringA((string("Too Many PointLights Registered. Limit: ") + to_string(e.limit) + string(", Used: ") + to_string(e.actual)).c_str());
			assert(false);
		}

		SpRenderer::Render();

		GetWDX()->PostDrawCommands();

		/*���t���[�����������܂�*/
		GetWDX()->EndFrame();

		/*DrawString�p�̃f�[�^�����*/
		TextDrawer::ReleaseDrawStringData();
		LineDrawer::ClearLineData();
	}

	/*���[�v�����܂�*/
	SoundManager::ReleaseAllSounds();
	SpImGui::Shutdown();
	CloseAllSpWindow();

	return 0;
}