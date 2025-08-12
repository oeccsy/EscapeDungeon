#include "ConnectLevel.h"

#include "Networking/Client.h"
#include "Networking/Command.h"
#include "Math/Vector2.h"
#include "Utils/Utils.h"
#include "Game/Game.h"

#include <iostream>

ConnectLevel::ConnectLevel() : commandHandler(*this)
{
	Client& client = Client::Get();
	client.InitSocket();

	FD_ZERO(&client.readSet);
	FD_SET(client.clientSocket, &client.readSet);

	uiSystem.InitLogArea();
}

ConnectLevel::~ConnectLevel() { }

void ConnectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	Client& client = Client::Get();
	
	RecvData();

	while (!client.readQueue.empty())
	{
		Command command = client.readQueue.front();
		client.readQueue.pop();

		commandHandler.Execute(command);
		if (command.data[0] == static_cast<char>(CommandType::GameStart)) return;
	}

	RequestConnect();
	if (Input::Get().GetKeyDown(VK_RETURN)) RequestGameStart();

	while(!client.writeQueue.empty())
	{
		Command command = client.writeQueue.front();
		client.writeQueue.pop();
		
		client.Send(command.data, sizeof(command.data));
	}
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
		Command command;

		bool success = client.Recv(command.data, sizeof(command.data));
		if (success == false) break;
		
		client.readQueue.push(command);
	}
}

void ConnectLevel::NewPlayerJoined(int playerCount)
{
	this->playerCount = playerCount;
	Logs::Get().AddLog({ "새로운 플레이어가 접속했습니다." });
}

void ConnectLevel::GameStart()
{
	Player::playerCount = playerCount - 1;
	Game::Get().LoadDungeonLevel();
}

void ConnectLevel::RequestConnect()
{
	Client& client = Client::Get();
	if (client.isConnected) return;

	bool success = client.Connect();
	if (!success) return;

	Logs::Get().AddLog({ "게임 서버에 접속했습니다." });
}

void ConnectLevel::RequestReady()
{
	Client& client = Client::Get();
	if (!client.isConnected) return;

	Command command;
	command.data[0] = static_cast<char>(CommandType::Ready);

	client.writeQueue.push(command);
}

void ConnectLevel::RequestGameStart()
{
	Client& client = Client::Get();
	if (!client.isConnected) return;

	Command command;
	command.data[0] = static_cast<char>(CommandType::GameStart);
	
	client.writeQueue.push(command);
}