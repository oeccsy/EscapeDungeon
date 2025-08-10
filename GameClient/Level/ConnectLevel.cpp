#include "ConnectLevel.h"

#include "Networking/Packet.h"
#include "Math/Vector2.h"
#include "Utils/Utils.h"
#include "Game/Game.h"

#include <iostream>

ConnectLevel::ConnectLevel()
{
	Client& client = Client::Get();
	client.InitSocket();

	FD_ZERO(&client.readSet);
	FD_SET(client.clientSocket, &client.readSet);
}

ConnectLevel::~ConnectLevel()
{
}

void ConnectLevel::BeginPlay()
{
	super::BeginPlay();

	InitUI();
}

void ConnectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	RecvData();

	Client& client = Client::Get();

	while (!client.readQueue.empty())
	{
		Packet packet = client.readQueue.front();
		client.readQueue.pop();

		switch (packet.data[0])
		{
		case 'n':
			logs.push_back("새로운 플레이어가 접속하였습니다.");
			playerCount = packet.data[1];
			break;
		case 'd':
			Game::Get().LoadDungeonLevel();
			return;
		}
	}

	RequestConnect();
	if (Input::Get().GetKeyDown(VK_RETURN)) RequestStart();
}

void ConnectLevel::Render()
{
	super::Render();

	for (int i = 1; i <= 20; ++i)
	{
		if (logs.size() < i) break;

		std::string log = logs[logs.size() - i];
		Utils::SetConsolePosition(Vector2(92, 38 - i));
		Utils::SetConsoleTextColor(Color::White);
		std::cout << log;
	}

	Utils::SetConsolePosition(Vector2(20, 15));
	std::cout << "플레이어 수 : " << playerCount;
}

void ConnectLevel::InitUI()
{
	Utils::SetConsoleTextColor(Color::White);
	
	Utils::SetConsolePosition(Vector2(90, 7));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << '\n';

	for (int i = 1; i <= 30; ++i)
	{
		Utils::SetConsolePosition(Vector2(90, 38 - i));
		std::cout << "■                                        ■" << '\n';
	}

	Utils::SetConsolePosition(Vector2(90, 38));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << '\n';
}

void ConnectLevel::RecvData()
{
	Client& client = Client::Get();

	while (true)
	{
		Packet packet = { };

		bool success = client.Recv(packet.data, 100);
		if (success == false) break;
		
		client.readQueue.push(packet);
	}
}

void ConnectLevel::RequestConnect()
{
	Client& client = Client::Get();

	if (client.isConnected) return;

	bool success = client.Connect();
	if (!success) return;

	logs.push_back("게임 서버에 접속하였습니다.");
}

void ConnectLevel::RequestStart()
{
	Client& client = Client::Get();

	if (!client.isConnected) return;

	char buffer[100] = "s";
	client.Send(buffer, sizeof(buffer));
}