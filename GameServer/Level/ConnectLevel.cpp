#include "ConnectLevel.h"

#include "Networking/Packet.h"
#include "Math/Vector2.h"
#include "Utils/Utils.h"

#include <iostream>

ConnectLevel::ConnectLevel()
{
	server.InitSocket();
	server.Bind();
	server.Listen();

	FD_ZERO(&server.readSet);
	FD_SET(server.listenSocket, &server.readSet);
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

	if (playerCount < MAX_PLAYER_COUNT)
	{
		bool isPlayerJoin = server.Accept();

		if (isPlayerJoin) PlayerJoin();
	}

	server.Recv();

	while (!server.packets.empty())
	{
		Packet packet = server.packets.front();
		server.packets.pop();

		// TODO : 패킷 처리
	}

	// TODO : 패킷 전송
}

void ConnectLevel::Render()
{
	super::Render();

	Utils::SetConsoleTextColor(Color::White);

	for (int i = 1; i <= 20; ++i)
	{
		if (logs.size() < i) break;

		std::string log = logs[logs.size() - i];
		Utils::SetConsolePosition(Vector2(92, 38 - i));
		
		std::cout << log;
	}

	Utils::SetConsolePosition(Vector2(20, 15));
	std::cout << "플레이어 수 : " << playerCount;
}

void ConnectLevel::InitUI()
{
	Utils::SetConsoleTextColor(Color::White);

	Utils::SetConsolePosition(Vector2(90, 7));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";

	for (int i = 1; i <= 30; ++i)
	{
		Utils::SetConsolePosition(Vector2(90, 38 - i));
		std::cout << "■                                        ■";
	}

	Utils::SetConsolePosition(Vector2(90, 38));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
}

void ConnectLevel::PlayerJoin()
{
	playerCount++;
	logs.push_back("새로운 플레이어가 접속하였습니다.");
	
	char buffer[100] = { 'n', playerCount };
	server.SendAll(buffer, 100);
}

void ConnectLevel::PlayerLeave()
{

}