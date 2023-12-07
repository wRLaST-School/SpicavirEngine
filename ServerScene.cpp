#include "stdafx.h"
#include "ServerScene.h"
#include <Input.h>

#include <process.h>
#include <mmsystem.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "winmm.lib")

void ServerScene::LoadResources()
{
}

void ServerScene::Init()
{
	Camera::Set(cam);

	(circleA = AddComponent<CircleCollider>("Circle"))->Init(Vec2(400.f, 400.f), 100.f);
	(circleB = AddComponent<CircleCollider>("Circle"))->Init(Vec2(100.f, 100.f), 50.f);

	static std::thread thread(&ServerScene::Threadfunc, this);
	thread.detach();
}

void ServerScene::Update()
{
	const static float spd = 5.f;

	Vec2 vel;
	vel.x = static_cast<float>(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));
	vel.y = static_cast<float>(Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP));

	if (vel.GetSquaredLength())
	{
		vel.SetLength(spd);
	}

	circleB->pos_ += vel;

	if (circleA->Collide(*circleB))
	{
		circleB->color_ = Color::Blue;
	}
	else
	{
		circleB->color_ = Color::Red;
	}
}

void ServerScene::DrawBack()
{
}

void ServerScene::Draw3D()
{
}

void ServerScene::DrawSprite()
{
}

DWORD ServerScene::Threadfunc()
{
	SOCKET sWait, sConnect;
	WORD wPort = 8000;
	SOCKADDR_IN saConnect, saLocal;
	int iLen, iRecv;

	// リスンソケット
	sWait = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&saLocal, sizeof(saLocal));

	// 8000番に接続待機用ソケット作成
	saLocal.sin_family = AF_INET;
	saLocal.sin_addr.s_addr = INADDR_ANY;
	saLocal.sin_port = htons(wPort);

	if (bind(sWait, (sockaddr*)&saLocal, sizeof(saLocal)) == SOCKET_ERROR) {

		closesocket(sWait);
		OutputDebugStringA("接続待機ソケット失敗");
		return 1;
	}

	if (listen(sWait, 2) == SOCKET_ERROR) {

		closesocket(sWait);
		OutputDebugStringA("接続待機ソケット失敗");
		return 1;
	}

	OutputDebugStringA("接続待機ソケット成功");

	iLen = sizeof(saConnect);

	// 接続受け入れて送受信用ソケット作成
	sConnect = accept(sWait, (sockaddr*)&saLocal, &iLen);

	// 接続待ち用ソケット解放
	closesocket(sWait);

	if (sConnect == INVALID_SOCKET) {

		shutdown(sConnect, 2);
		closesocket(sConnect);
		shutdown(sWait, 2);
		closesocket(sWait);

		OutputDebugStringA("ソケット接続失敗");

		return 1;
	}

	OutputDebugStringA("ソケット接続成功");

	iRecv = 0;

	while (1)
	{
		int nRcv;

		// クライアント側キャラの位置情報を受け取り
		nRcv = recv(sConnect, (char*)&circleA->pos_, sizeof(Vec2), 0);

		if (nRcv == SOCKET_ERROR)break;

		// サーバ側キャラの位置情報を送信
		send(sConnect, (const char*)&circleB->pos_, sizeof(Vec2), 0);
	}

	shutdown(sConnect, 2);
	closesocket(sConnect);

	return 0;
}
