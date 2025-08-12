#include "ConnectLevel.h"

#include "Networking/Server.h"
#include "Networking/Command.h"
#include "Math/Vector2.h"
#include "Utils/Utils.h"
#include "Game/Game.h"

#include <iostream>

ConnectLevel::ConnectLevel() : commandHandler(*this)
{
	Server& server = Server::Get();
	server.InitSocket();
	server.Bind();
	server.Listen();

	FD_ZERO(&server.readSet);
	FD_SET(server.listenSocket, &server.readSet);

	Logs::Get().AddLog({ "==== 서버 시작 ====" });

	uiSystem.InitLogArea();
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
		if (command.data[0] == static_cast<char>(CommandType::GameStart)) return;
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

	uiSystem.RenderPlayerCount(playerCount, ConnectLevel::MAX_PLAYER_COUNT);
}

void ConnectLevel::Connect()
{
	playerCount++;
	Logs::Get().AddLog({ "새로운 플레이어가 접속했습니다." });

	Server& server = Server::Get();

	Command command;
	command.dest = INVALID_SOCKET;
	command.data[0] = static_cast<char>(CommandType::NewPlayer);
	command.data[1] = playerCount;

	server.writeQueue.push(command);
}

void ConnectLevel::Disconnect()
{
	Logs::Get().AddLog({ "어떤 플레이어의 접속이 끊겼습니다." });
}

void ConnectLevel::Ready()
{
	
}

void ConnectLevel::GameStart()
{
	if (playerCount < MAX_PLAYER_COUNT) return;
	
	int index = Utils::Random(1, 10);

	Server& server = Server::Get();
	
	Command command;
	command.dest = INVALID_SOCKET;
	command.data[0] = static_cast<char>(CommandType::GameStart);
	command.data[1] = index;

	server.writeQueue.push(command);
	
	Player::playerCount = playerCount - 1;
	Game::Get().LoadDungeonLevel(index);
}