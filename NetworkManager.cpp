#include "stdafx.h"
#include "NetworkManager.h"
#include <GameManager.h>

void NetworkManager::Init()
{
	if (GameManager::isServer)
	{
		static std::thread thread(&NetworkManager::ServerThreadFunc, this);
	}
	else
	{
		static std::thread thread(&NetworkManager::ClientThreadFunc, this);
	}
}

void NetworkManager::ClientThreadFunc()
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
		return;
	}

	ZeroMemory(&saLocal, sizeof(saLocal));
	// 8000番に接続待機用ソケット作成
	saLocal.sin_family = lpHost->h_addrtype;
	saLocal.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
	saLocal.sin_port = htons(wPort);

	if (connect(sWait, (sockaddr*)&saLocal, sizeof(saLocal)) == SOCKET_ERROR) {

		closesocket(sWait);
		OutputDebugStringA("接続待機ソケット失敗");
		return;
	}

	OutputDebugStringA("接続待機ソケット成功");

	iLen = sizeof(saConnect);

	iRecv = 0;

	while (1)
	{
		int nRcv;

		// クライアント側キャラの位置情報を送信
		send(sWait, (const char*)&clnData, sizeof(ClientSideData), 0);

		// サーバ側キャラの位置情報を受け取り
		nRcv = recv(sWait, (char*)&srvData, sizeof(ServerSideData), 0);

		if (nRcv == SOCKET_ERROR)break;
	}

	shutdown(sWait, 2);
	closesocket(sWait);

	return;
}

void NetworkManager::ServerThreadFunc()
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
		return;
	}

	if (listen(sWait, 2) == SOCKET_ERROR) {

		closesocket(sWait);
		OutputDebugStringA("接続待機ソケット失敗");
		return;
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

		return;
	}

	OutputDebugStringA("ソケット接続成功");

	iRecv = 0;

	while (1)
	{
		int nRcv;

		// クライアント側キャラの位置情報を受け取り
		nRcv = recv(sConnect, (char*)&clnData, sizeof(ClientSideData), 0);

		if (nRcv == SOCKET_ERROR)break;

		// サーバ側キャラの位置情報を送信
		send(sConnect, (const char*)&srvData, sizeof(ServerSideData), 0);
	}

	shutdown(sConnect, 2);
	closesocket(sConnect);

	return;
}
