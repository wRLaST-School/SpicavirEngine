#include "TestScene.h"
#include "RTVManager.h"
#include "wSwapChainManager.h"
#include "Input.h"
#include "RayCollider.h"
#include "TextDrawer.h"

void TestScene::Init()
{
	wTextureManager::Init();

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

	mCube = Model("cube");

	ray.model = &mCube;
	ray.scale = {0.1, 0.1, 12.5};
	ray.position = { 0.0, 0.0, 10.0f };
	ray.UpdateMatrix();

	sky = Model("skydome");

	mSphere = Model("sphere");

	sphere.model = &mSphere;

	skysphere.model = &sky;

	whiteTex = wTextureManager::LoadTexture("Resources/white.png", "white");
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

	sphere.position = (Vec3)sphere.position + Vec3(
		(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT)) * 0.1f,
		(Input::Key::Down(DIK_SPACE) - Input::Key::Down(DIK_LSHIFT)) * 0.1f,
		(Input::Key::Down(DIK_UP) - Input::Key::Down(DIK_DOWN)) * 0.1f		
		);

	sphere.position = (Vec3)sphere.position + Vec3(
		(Input::Pad::GetLStick().x) * 0.0001f,
		(Input::Pad::Down(Button::A) - Input::Pad::Down(Trigger::Left)) * 0.1f,
		(Input::Pad::GetLStick().y) * 0.0001f
	);

	camera.UpdateMatrix();

	cameraSpr.UpdateMatrix();
	xCamSpr.UpdateMatrix();
	yCamSpr.UpdateMatrix();
	zCamSpr.UpdateMatrix();
	skysphere.UpdateMatrix();
	sphere.UpdateMatrix();

	RayCollider rayCol = RayCollider(ray.matWorld.ExtractAxisZ(ray.scale.z).SetLength(25), (Vec3)ray.position - Vec3(0,0,12.5));
	SphereCollider sphereCol = SphereCollider(sphere.position, 1.0f);

	if (rayCol.Collide(sphereCol))
	{
		*ray.brightnessCB.contents = Float4{1.0f, 0.0f, 0.0f, 1.0f};
	}
	else
	{
		*ray.brightnessCB.contents = Float4{ 1.0f, 1.0f, 1.0f, 1.0f };
	}
}

void TestScene::DrawBack()
{
}

void TestScene::Draw3D()
{
	RTVManager::SetRenderTargetToTexture("camSpr");
	Camera::Set(camera);
	ray.Draw(whiteTex); skysphere.Draw();
	sphere.Draw();

	RTVManager::SetRenderTargetToTexture("xCamSpr");
	Camera::Set(xCam);
	ray.Draw(whiteTex); skysphere.Draw();
	sphere.Draw();

	RTVManager::SetRenderTargetToTexture("yCamSpr");
	Camera::Set(yCam);
	ray.Draw(whiteTex); skysphere.Draw();
	sphere.Draw();

	RTVManager::SetRenderTargetToTexture("zCamSpr");
	Camera::Set(zCam);
	ray.Draw(whiteTex); skysphere.Draw();
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

	StringOptions udevGothicOpt;
	udevGothicOpt.fontOptions.name = "UDEV Gothic Regular";
	udevGothicOpt.size = 32;

	TextDrawer::DrawString("0Ç§Ç…Ç·Å`Å`Å`Å`0", 1180, 100, Align::TopRight, udevGothicOpt);
	TextDrawer::DrawString("àühogeÇ¶ÉI123", 1180, 132, Align::TopRight);
}
