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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	/*デバッグ有効化*/
#ifdef  _DEBUG
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif //  _DEBUG

	//ウィンドウを生成
	{
		wWindow wwnd;
		wwnd.Create(L"DirectXTest", 1280, 720);
		RegisterwWindow(wwnd, "Default");
	}

	/*DirectX初期化処理*/
	InitWDX();
	HRESULT result;

	//Init Input
	InitInput();

	/*Init Draw*/
	//ダブルバッファリングとか
	InitWSCM();
	InitWDepth();

	//Objects
	Model model = Model("Resources/Models/square.obj");

	list<Object3D> objects;

	for (int i = 0; i < 1; i++)
	{
		objects.push_back(Object3D());
	}

	for (auto itr = objects.begin(); itr != objects.end(); itr++)
	{
		itr->model = &model;
	}

	//定数バッファここまで

	//PSO
	InitTextureBuff();
	RegisterAndInitShader("def");
	RegisterAndInitPSO("def", GetShader("def"));

	GetPSODesc("def")->InputLayout.pInputElementDescs = inputLayout;
	GetPSODesc("def")->InputLayout.NumElements = _countof(inputLayout);

	CreateAndInitRootSignature();
	/*Init Draw End*/
	Matrix wMat = Matrix::Identity();
	/*ループ*/
	while (true)
	{
		if(GetWDX()->StartFrame()) break;
		/*毎フレーム処理*/
		/*更新処理*/
		/*更新処理ここまで*/

		/*描画処理*/
		//変換
		Matrix pMat = Matrix::Projection(
			PI/4,
			(float)GetwWindow()->width / GetwWindow()->height,
			0.1f, 1000.0f
		);

		Matrix vMat;
		Float3 eye{ 0, 0, -300 };
		Float3 target{ 0, 0, 0 };
		Float3 up{ 0, 1.0f, 0 };

		vMat = Matrix::ViewLookAt(eye, target, up);

		objects.begin()->rotation.x += 0.01;
		objects.begin()->UpdateMatrix();

		Matrix vproj = vMat * pMat;
		/*描画前処理ここまで*/

		GetWDX()->PreDrawCommands();

		for (auto itr = objects.begin(); itr != objects.end(); itr++)
		{
			itr->Draw(vproj);
		}

		GetWDX()->PostDrawCommands();

		/*毎フレーム処理ここまで*/
		GetWDX()->EndFrame();
	}
	/*ループここまで*/
	CloseAllwWindow();

	return 0;
}