#include "ConnectLevel.h"

#include "Networking/Packet.h"
#include "Math/Vector2.h"
#include "Utils/Utils.h"

#include <iostream>

ConnectLevel::ConnectLevel()
{
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

	while (!client.packets.empty())
	{
		Packet packet = client.packets.front();
		client.packets.pop();

		switch (packet.data[0])
		{
		case 'a' :
			break;
		case 'b':
			break;
		}
	}

	RequestConnect();
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
}

void ConnectLevel::InitUI()
{
	Utils::SetConsoleTextColor(Color::White);
	
	Utils::SetConsolePosition(Vector2(90, 7));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■\n";

	for (int i = 1; i <= 30; ++i)
	{
		Utils::SetConsolePosition(Vector2(90, 38 - i));
		std::cout << "■                                        ■";
	}

	Utils::SetConsolePosition(Vector2(90, 38));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■\n";
}

void ConnectLevel::RecvData()
{
	while (true)
	{
		Packet packet = { };

		bool success = client.Recv(packet.data, 100);
		if (success == false) break;
		
		client.packets.push(packet);
	}
}

void ConnectLevel::SetPlayerInfo(int playerCount)
{

}

void ConnectLevel::RequestConnect()
{
	if (client.isConnected) return;

	bool success = client.Connect();
	if (!success) return;

	logs.push_back("게임 서버에 접속하였습니다.");
}

void ConnectLevel::RequestPlayersInfo()
{
	char buffer[100];
	client.Send(buffer, 100);
}