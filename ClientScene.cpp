#include "stdafx.h"
#include "ClientScene.h"
#include <Input.h>
#include <GameManager.h>

void ClientScene::LoadResources()
{
}

void ClientScene::Init()
{
	Camera::Set(cam);

	(circleA = AddComponent<CircleCollider>("Circle"))->Init(Vec2(400.f, 400.f), 100.f);
	(circleB = AddComponent<CircleCollider>("Circle"))->Init(Vec2(100.f, 100.f), 50.f);

	static std::thread thread(&ClientScene::Threadfunc, this);
	thread.detach();
}

void ClientScene::Update()
{
	const static float spd = 5.f;

	Vec2 vel;
	vel.x = static_cast<float>(Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT));
	vel.y = static_cast<float>(Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP));

	if (vel.GetSquaredLength())
	{
		vel.SetLength(spd);
	}

	circleA->pos_ += vel;

	if (circleA->Collide(*circleB))
	{
		circleB->color_ = Color::Blue;
	}
	else
	{
		circleB->color_ = Color::Red;
	}
}

void ClientScene::DrawBack()
{
}

void ClientScene::Draw3D()
{
}

void ClientScene::DrawSprite()
{
}

DWORD ClientScene::Threadfunc()
{
	SOCKET sWait;
	WORD wPort = 8000;
	SOCKADDR_IN saConnect, saLocal;
	int iLen, iRecv;
	char szServer[1024];

	strcpy_s(szServer, 1024 * sizeof(char), GameManager::IP.c_str());

	// リスンソケット
	sWait = socket(AF_INET, SOCK_STREAM, 0);

	//サーバー名から
	auto lpHost = gethostbyname(szServer);

	if (lpHost == NULL) {
		/* サーバーをIPアドレスで取得する */
		auto addr = inet_addr(szServer);
		lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (lpHost == NULL)
	{
		return 1;
	}

	ZeroMemory(&saLocal, sizeof(saLocal));
	// 8000番に接続待機用ソケット作成
	saLocal.sin_family = lpHost->h_addrtype;
	saLocal.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
	saLocal.sin_port = htons(wPort);

	if (connect(sWait, (sockaddr*)&saLocal, sizeof(saLocal)) == SOCKET_ERROR) {

		closesocket(sWait);
		OutputDebugStringA("接続待機ソケット失敗");
		return 1;
	}

	OutputDebugStringA("接続待機ソケット成功");

	iLen = sizeof(saConnect);

	iRecv = 0;

	while (1)
	{
		int nRcv;

		// クライアント側キャラの位置情報を送信
		send(sWait, (const char*)&circleA->pos_, sizeof(Vec2), 0);

		// サーバ側キャラの位置情報を受け取り
		nRcv = recv(sWait, (char*)&circleB->pos_, sizeof(Vec2), 0);

		if (nRcv == SOCKET_ERROR)break;
	}

	shutdown(sWait, 2);
	closesocket(sWait);

	return 0;
}
