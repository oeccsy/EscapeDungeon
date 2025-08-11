#include "ConnectLevel.h"

#include "Networking/Client.h"
#include "Networking/Command.h"
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

	uiSystem.InitLogArea();
	commandHandler = CommandHandler(*this);
}

ConnectLevel::~ConnectLevel() { }

void ConnectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	Client& client = Client::Get();
	
	RecvData();

	while (!client.readQueue.empty())
	{
		Command packet = client.readQueue.front();
		client.readQueue.pop();

		switch (packet.data[0])
		{
		case 'n':
			Logs::Get().AddLog({ "새로운 플레이어가 접속하였습니다." });
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

	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(20, 15));
	std::cout << "플레이어 수 : " << playerCount;
}

void ConnectLevel::RecvData()
{
	Client& client = Client::Get();

	while (true)
	{
		Command packet = { };

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

	Logs::Get().AddLog({ "게임 서버에 접속했습니다." });
}

void ConnectLevel::RequestStart()
{
	Client& client = Client::Get();

	if (!client.isConnected) return;

	char buffer[100] = "s";
	client.Send(buffer, sizeof(buffer));
}