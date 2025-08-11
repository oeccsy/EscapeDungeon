#include "ConnectLevel.h"

#include "Networking/Server.h"
#include "Networking/Command.h"
#include "Math/Vector2.h"
#include "Utils/Utils.h"
#include "Game/Game.h"

#include <iostream>

ConnectLevel::ConnectLevel()
{
	Server& server = Server::Get();
	server.InitSocket();
	server.Bind();
	server.Listen();

	FD_ZERO(&server.readSet);
	FD_SET(server.listenSocket, &server.readSet);

	Logs::Get().AddLog({ "==== 서버 시작 ====" });

	uiSystem.InitLogArea();
	commandHandler = CommandHandler(*this);
}

ConnectLevel::~ConnectLevel() { }

void ConnectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	Server& server = Server::Get();

	if (playerCount < MAX_PLAYER_COUNT) server.Accept();
	
	server.Recv();

	while (!server.readQueue.empty())
	{
		Command command = server.readQueue.front();
		server.readQueue.pop();

		commandHandler.Execute(command);
	}

	while (!server.writeQueue.empty())
	{
		Command command = server.writeQueue.front();
		server.writeQueue.pop();

		if (command.dest == INVALID_SOCKET)
		{
			server.SendAll(command.data, sizeof(command.data));
		}
		else
		{
			server.Send(command.dest, command.data, sizeof(command.data));
		}
	}
}

void ConnectLevel::Render()
{
	super::Render();

	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(20, 15));
	std::cout << "플레이어 수 : " << playerCount;
}