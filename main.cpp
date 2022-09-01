#include "Essentials.h"
#include "wWindow.h"
#include "wDirectX.h"
#include "Input.h"
#include "wSwapChainManager.h"
#include "wDepth.h"
#include "wShader.h"
#include "wPSO.h"
#include "wTexture.h"
#include "Vec3.h"
#include "wConstBuffer.h"
#include "Object3D.h"
#include "Matrix.h"
#include "wRootSignature.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "TextDrawer.h"

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
		wWindow wwnd;
		wwnd.Create(L"DirectX", 1280, 720);
		RegisterwWindow(wwnd, "Default");
	}

	/*DirectX����������*/
	InitWDX();
	HRESULT result;

	//Init Input
	Input::Key::Init();
	Input::Pad::Init();

	/*Init Draw*/
	//�_�u���o�b�t�@�����O�Ƃ�
	InitWSCM();
	InitWDepth();

	wTextureManager::Create();

	//PSO
	RegisterAndInitShader("def", "BasicVS.hlsl", "BasicPS.hlsl");
	RegisterAndInitPSO("def", GetShader("def"));

	//2DPSO
	RegisterAndInitShader("2d", "SpriteVS.hlsl", "SpritePS.hlsl");
	RegisterAndInitPSOfor2D("2d", GetShader("2d"));

	GetPSODesc("def")->InputLayout.pInputElementDescs = inputLayout;
	GetPSODesc("def")->InputLayout.NumElements = _countof(inputLayout);

	GetPSODesc("2d")->InputLayout.pInputElementDescs = inputLayout2D;
	GetPSODesc("2d")->InputLayout.NumElements = _countof(inputLayout2D);

	CreateAndInitRootSignature();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	GetPSODesc("def")->pRootSignature = GetRootSignature()->rootsignature.Get();
	GetPSODesc("2d")->pRootSignature = GetRootSignature()->rootsignature.Get();
	GetWPSO("def")->Create();
	GetWPSO("2d")->Create();
	/*Init Draw End*/
	Sprite::InitCommon();

	//Init Scene
	SceneManager sceneManager = SceneManager();
	sceneManager.Init();

	/*�f�o�b�O�L����*/
#ifdef  _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(GetWDX()->dev->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	}
#endif //  _DEBUG
	/*���[�v*/
	while (true)
	{
		if (GetWDX()->StartFrame()) break;
		Input::Key::Update();
		Input::Pad::Update();

		/*���t���[������*/

		/*�X�V����*/
		sceneManager.Update();
		/*�X�V���������܂�*/

		GetWDX()->PreDrawCommands();

		try{
			Light::UpdateLightData();
		}
		catch (PointLight::QuantityOverflow& e) {
			assert(false);
		}

		Light::Use();

		Sprite::PreSpriteDraw();

		sceneManager.DrawBack();

		Sprite::PostSpriteDraw();

		sceneManager.Draw3D();

		Sprite::PreSpriteDraw();

		sceneManager.DrawSprite();

		Sprite::PostSpriteDraw();

		GetWDX()->PostDrawCommands();

		/*���t���[�����������܂�*/
		GetWDX()->EndFrame();

		/*DrawString�p�̃f�[�^�����*/
		TextDrawer::ReleaseDrawStringData();
	}
	/*���[�v�����܂�*/
	CloseAllwWindow();

	return 0;
}