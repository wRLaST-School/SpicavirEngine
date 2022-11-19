#include "TestScene.h"
#include "RTVManager.h"
#include "SpSwapChainManager.h"
#include "Input.h"
#include "RayCollider.h"
#include "TextDrawer.h"

void TestScene::LoadResources()
{
	RTVManager::CreateRenderTargetTexture(640, 360, "camSpr");
	RTVManager::CreateRenderTargetTexture(640, 360, "xCamSpr");
	RTVManager::CreateRenderTargetTexture(640, 360, "yCamSpr");
	RTVManager::CreateRenderTargetTexture(640, 360, "zCamSpr");

	SpTextureManager::LoadTexture("Resources/white.png", "white");

	ModelManager::Register("cube", "Cube");

	ModelManager::Register("skydome", "Sky");

	ModelManager::Register("sphere", "Sphere");

	ModelManager::Register("square", "Square");
}

void TestScene::Init()
{
#pragma region élï™äÑâÊñ ópÉJÉÅÉâê›íË
	camera.SetRenderSize(640, 360);
	camera.nearZ = 0.1f;
	camera.farZ = 1000.0f;
	camera.fov = PI / 2;

	camera.position.x = -3;
	camera.position.y = 3;
	camera.position.z = -3;
	camera.UpdateMatrix();

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

	pane.model = ModelManager::GetModel("Square");
	pane2.model = ModelManager::GetModel("Square");

	skysphere.model = ModelManager::GetModel("Sky");

	whiteTex = "white";

	emitter.position = { 0, 0, 0 };
	emitter.radius = { 3.f, 3.f, 3.f };

	emitter.quantity = 1;

	pane.camera = &camera;
	pane2.camera = &camera;
}

void TestScene::Update()
{
	cameraSpr.position = { 960, 180 };
	xCamSpr.position = { 960, 540 };
	yCamSpr.position = { 320, 180 };
	zCamSpr.position = { 320, 540 };

	cameraSpr.scale = { 1.0f, 1.0f };
	xCamSpr.scale = { 1.0f, 1.0f };
	yCamSpr.scale = { 1.0f, 1.0f };
	zCamSpr.scale = { 1.0f, 1.0f };

	pane.position = (Vec3)pane.position + Vec3(
		(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT)) * 0.1f,
		(Input::Key::Down(DIK_SPACE) - Input::Key::Down(DIK_LSHIFT)) * 0.1f,
		(Input::Key::Down(DIK_UP) - Input::Key::Down(DIK_DOWN)) * 0.1f
	);

	pane.position = (Vec3)pane.position + Vec3(
		(Input::Pad::GetLStick().x) * 0.0001f,
		(Input::Pad::Down(Button::A) - Input::Pad::Down(Trigger::Left)) * 0.1f,
		(Input::Pad::GetLStick().y) * 0.0001f
	);

	pane.scale = {1.f / 30, 1.f / 30, 1.f / 30};

	pane2.position = (Vec3)pane.position + Vec3(
		(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT)) * 0.1f,
		(Input::Key::Down(DIK_SPACE) - Input::Key::Down(DIK_LSHIFT)) * 0.1f,
		(Input::Key::Down(DIK_UP) - Input::Key::Down(DIK_DOWN)) * 0.1f
	);

	pane2.position = (Vec3)pane.position + Vec3(
		(Input::Pad::GetLStick().x) * 0.0001f,
		(Input::Pad::Down(Button::A) - Input::Pad::Down(Trigger::Left)) * 0.1f,
		(Input::Pad::GetLStick().y) * 0.0001f
	);

	pane2.position.x -= 6;

	pane2.scale = { 1.f / 30, 1.f / 30, 1.f / 30 };
	//camera.rotation = (Vec3)camera.rotation + Vec3(0.01, 0, 0);

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

	emitter.Update();

	cameraSpr.UpdateMatrix();
	xCamSpr.UpdateMatrix();
	yCamSpr.UpdateMatrix();
	zCamSpr.UpdateMatrix();
	skysphere.UpdateMatrix();
	pane.UpdateMatrix();
	pane2.UpdateMatrix();
}

void TestScene::DrawBack()
{
}

void TestScene::Draw3D()
{
	RTVManager::SetRenderTargetToTexture("camSpr");
	Camera::Set(camera);
	skysphere.Draw();
	pane.Draw();
	pane2.Draw();
	emitter.Draw();

	RTVManager::SetRenderTargetToTexture("xCamSpr");
	Camera::Set(xCam);
	skysphere.Draw();
	pane.Draw();
	pane2.Draw();
	emitter.Draw();

	RTVManager::SetRenderTargetToTexture("yCamSpr");
	Camera::Set(yCam);
	skysphere.Draw();
	pane.Draw();
	pane2.Draw();
	emitter.Draw();

	RTVManager::SetRenderTargetToTexture("zCamSpr");
	Camera::Set(zCam);
	skysphere.Draw();
	pane.Draw();
	pane2.Draw();
	emitter.Draw();

	Camera::Set(finalScene);
	Camera::UseCurrent();
	RTVManager::SetRenderTargetToBackBuffer(GetSCM()->swapchain->GetCurrentBackBufferIndex());
}

void TestScene::DrawSprite()
{
	cameraSpr.Draw();
	xCamSpr.Draw();
	yCamSpr.Draw();
	zCamSpr.Draw();

	StringOptions udevGothicOpt;
	udevGothicOpt.fontOptions.name = "UDEV Gothic Regular";
	udevGothicOpt.size = 32;

	//TextDrawer::DrawString("0Ç§Ç…Ç·Å`Å`Å`Å`0", 1180, 100, Align::TopRight, udevGothicOpt);
	//TextDrawer::DrawString("àühogeÇ¶ÉI123", 1180, 132, Align::TopRight);
}
