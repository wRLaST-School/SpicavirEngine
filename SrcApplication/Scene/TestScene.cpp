#include "TestScene.h"
#include "RTVManager.h"
#include "SpSwapChainManager.h"
#include "Input.h"
#include "RayCollider.h"
#include "TextDrawer.h"

void TestScene::Init()
{
	SpTextureManager::Init();

#pragma region 四分割画面用カメラ設定
	camera.SetRenderSize(640, 360);
	camera.nearZ = 0.1f;
	camera.farZ = 1000.0f;
	camera.fov = PI / 2;

	camera.position.x = -3;
	camera.position.y = 3;
	camera.position.z = -3;
	camera.UpdateMatrix();

	RTVManager::CreateRenderTargetTexture(640, 360, "camSpr");
	cameraSpr = Sprite("camSpr");

	xCam.SetRenderSize(640, 360);
	xCam.nearZ = 0.1f;
	xCam.farZ = 1000.0f;
	xCam.fov = PI / 2;

	xCam.position.x = -10;
	xCam.position.y = 0;
	xCam.position.z = 0;
	xCam.rotation = { 0.0f, PI / 2, 0.0f };
	xCam.UpdateMatrix();

	RTVManager::CreateRenderTargetTexture(640, 360, "xCamSpr");
	xCamSpr = Sprite("xCamSpr");

	yCam.SetRenderSize(640, 360);
	yCam.nearZ = 0.1f;
	yCam.farZ = 1000.0f;
	yCam.fov = PI / 2;

	yCam.position.x = 0;
	yCam.position.y = 10;
	yCam.position.z = 0;
	yCam.rotation = { PI / 2, 0.0f, 0.0f };
	yCam.UpdateMatrix();

	RTVManager::CreateRenderTargetTexture(640, 360, "yCamSpr");
	yCamSpr = Sprite("yCamSpr");

	zCam.SetRenderSize(640, 360);
	zCam.nearZ = 0.1f;
	zCam.farZ = 1000.0f;
	zCam.fov = PI / 2;

	zCam.position.x = 0;
	zCam.position.y = 0;
	zCam.position.z = -10;
	zCam.rotation = { 0.0f, 0.0f, 0.0f };
	zCam.UpdateMatrix();

	RTVManager::CreateRenderTargetTexture(640, 360, "zCamSpr");
	zCamSpr = Sprite("zCamSpr");

	camera.projectionMode = ProjectionMode::Perspective;
	xCam.projectionMode = ProjectionMode::Orthographic;
	yCam.projectionMode = ProjectionMode::Orthographic;
	zCam.projectionMode = ProjectionMode::Orthographic;

	cameraSpr.position = {960, 180, 0.0f};
	xCamSpr.position = {960, 540, 0.0f};
	yCamSpr.position = {320, 180, 0.0f};
	zCamSpr.position = {320, 540, 0.0f};

	finalScene.SetRenderSize(1280, 720);
	finalScene.nearZ = 0.1f;
	finalScene.farZ = 1000.0f;
	finalScene.fov = PI / 2;

	finalScene.position.x = 0;
	finalScene.position.y = 0;
	finalScene.position.z = -10;
	finalScene.rotation = { 0.0f, 0.0f, 0.0f };
	finalScene.UpdateMatrix();
#pragma endregion

	mCube = Model("cube");

	sky = Model("skydome");

	mSphere = Model("sphere");

	skysphere.model = &sky;

	whiteTex = wTextureManager::LoadTexture("Resources/white.png", "white");

	sphere.model = &mSphere;
}

void TestScene::Update()
{
#pragma region 四分割画面用カメラ設定
	cameraSpr.position = { 960, 180 };
	xCamSpr.position = { 960, 540 };
	yCamSpr.position = { 320, 180 };
	zCamSpr.position = { 320, 540 };

	cameraSpr.scale = { 1.0f, 1.0f };
	xCamSpr.scale = { 1.0f, 1.0f };
	yCamSpr.scale = { 1.0f, 1.0f };
	zCamSpr.scale = { 1.0f, 1.0f };

	camera.position = (Vec3)camera.position + Vec3(
		(Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A)) * 0.1f,
		(Input::Key::Down(DIK_E) - Input::Key::Down(DIK_Q)) * 0.1f,
		(Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S)) * 0.1f
	);

	camera.rotation = (Vec3)camera.rotation + Vec3(
		(Input::Key::Down(DIK_1) - Input::Key::Down(DIK_2)) * 0.01f,
		(Input::Key::Down(DIK_3) - Input::Key::Down(DIK_4)) * 0.01f,
		(Input::Key::Down(DIK_5) - Input::Key::Down(DIK_6)) * 0.01f
	);
	camera.UpdateMatrix();
#pragma endregion

	//Lerpで移動
	timer++;
	timer = min(timer, 300);
	sphere.position = Vec3::Lerp(Vec3(0, 0, 0), Vec3(-3, 1, 10), (float)timer / 300);

	//行列更新
	cameraSpr.UpdateMatrix();
	xCamSpr.UpdateMatrix();
	yCamSpr.UpdateMatrix();
	zCamSpr.UpdateMatrix();
	skysphere.UpdateMatrix();
	sphere.UpdateMatrix();
}

void TestScene::DrawBack()
{
}

void TestScene::Draw3D()
{
	RTVManager::SetRenderTargetToTexture("camSpr");
	Camera::Set(camera);
	skysphere.Draw();
	sphere.Draw();

	RTVManager::SetRenderTargetToTexture("xCamSpr");
	Camera::Set(xCam);
	skysphere.Draw();
	sphere.Draw();

	RTVManager::SetRenderTargetToTexture("yCamSpr");
	Camera::Set(yCam);
	skysphere.Draw();
	sphere.Draw();

	RTVManager::SetRenderTargetToTexture("zCamSpr");
	Camera::Set(zCam);
	skysphere.Draw();
	sphere.Draw();

	Camera::Set(finalScene);
	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void TestScene::DrawSprite()
{
	Camera::Set(finalScene);
	cameraSpr.Draw();
	xCamSpr.Draw();
	yCamSpr.Draw();
	zCamSpr.Draw();

	StringOptions arialOpt;
	arialOpt.fontOptions.name = "Arial";
	arialOpt.size = 32;
	arialOpt.fontOptions.resolution = 128;

	string str("");
	str += to_string(sphere.position.x);
	str += ", "; str += to_string(sphere.position.y);
	str += ", "; str += to_string(sphere.position.z);
	str += ", T = "; str += to_string(timer);
	TextDrawer::DrawString( str, 32, 32, Align::TopLeft, arialOpt);

	//TextDrawer::DrawString("0うにゃ～～～～0", 1180, 100, Align::TopRight, udevGothicOpt);
	//TextDrawer::DrawString("亜hogeえオ123", 1180, 132, Align::TopRight);
}
