#include "DungeonLevel.h"

#include "Engine.h"

#include "Networking/Server.h"
#include "Networking/Command.h"

#include "Actor/Actor.h"
#include "Actor/Player.h"
#include "Actor/Monster.h"
#include "Actor/Road.h"
#include "Actor/Task.h"
#include "Actor/Exit.h"

#include "Utils/Utils.h"
#include "Utils/Logs.h"

#include <iostream>
#include <vector>

DungeonLevel::DungeonLevel() : commandHandler(*this)
{
	ReadDungeonFile("Map_2.txt");
	BindActorID();

	Logs::Get().AddLog({ "==== 게임 시작 ====" });
	
	uiSystem.InitLogArea();
}

DungeonLevel::~DungeonLevel() {}


void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	Server& server = Server::Get();

	server.Recv();

	while (!server.readQueue.empty())
	{
		Command command = server.readQueue.front();
		server.readQueue.pop();
		
		commandHandler.Execute(command);
	}

	std::vector<Task*> tasks;
	std::vector<Player*> players;
	std::vector<Exit*> exits;
	Monster* monster = nullptr;

	for (Actor* const actor : actors)
	{
		Task* task = actor->As<Task>();
		if (task)
		{
			tasks.emplace_back(task);
			continue;
		}

		Player* player = actor->As<Player>();
		if (player)
		{
			players.emplace_back(player);
			continue;
		}

		Exit* exit = actor->As<Exit>();
		if (exit)
		{
			exits.emplace_back(exit);
			continue;
		}

		monster = actor->As<Monster>();
	}

	interactionSystem.ProgressTask(tasks, players, deltaTime);
	interactionSystem.CheckOpenExit(*this, 1);
	interactionSystem.EscapePlayer(exits, players);
	interactionSystem.KillPlayer(monster, players);

	gameOverSystem.CheckGameOver();

	for(auto player : players)
	{
		if (player->GetActorID() <= 0) continue;

		Vector2 pos = player->GetPosition();

		Command posCommand;
		posCommand.data[0] = static_cast<char>(CommandType::Position);
		posCommand.data[1] = player->GetActorID();
		posCommand.data[2] = pos.x;
		posCommand.data[3] = pos.y;

		server.writeQueue.push(posCommand);

		Command staminaCommand;
		staminaCommand.data[0] = static_cast<char>(CommandType::Stamina);
		staminaCommand.data[1] = player->GetActorID();
		staminaCommand.data[2] = player->GetStamina();

		server.writeQueue.push(staminaCommand);
	}

	if (monster && monster->GetActorID() > 0)
	{
		Vector2 pos = monster->GetPosition();

		Command posCommand;
		posCommand.data[0] = static_cast<char>(CommandType::Position);
		posCommand.data[1] = monster->GetActorID();
		posCommand.data[2] = pos.x;
		posCommand.data[3] = pos.y;

		server.writeQueue.push(posCommand);

		Command staminaCommand;
		staminaCommand.data[0] = static_cast<char>(CommandType::Stamina);
		staminaCommand.data[1] = monster->GetActorID();
		staminaCommand.data[2] = monster->GetStamina();

		server.writeQueue.push(staminaCommand);
	}

	while (!server.writeQueue.empty())
	{
		Command packet = server.writeQueue.front();
		server.writeQueue.pop();

		if (packet.dest == INVALID_SOCKET)
		{
			server.SendAll(packet.data, sizeof(packet.data));
		}
		else
		{
			server.Send(packet.dest, packet.data, sizeof(packet.data));
		}
	}
}

void DungeonLevel::Render()
{
	super::Render();

	std::vector<Player*> players;
	Monster* monster = nullptr;

	for (Actor* const actor : actors)
	{
		Player* player = actor->As<Player>();
		if (player)
		{
			players.emplace_back(player);
			continue;
		}

		monster = actor->As<Monster>();
	}

	uiSystem.RenderStaminaUI(players, monster);
}

bool DungeonLevel::Movable(const Vector2& targetPos)
{
	return dungeon[targetPos.y][targetPos.x] == '#';
}

Actor* DungeonLevel::GetActorByClient(SOCKET socket)
{
	if (clientToActor.find(socket) != clientToActor.end())
	{
		return clientToActor[socket];
	}
	else
	{
		return nullptr;
	}
}

Actor* DungeonLevel::GetActorByID(int id)
{
	if(idToActor.find(id) != idToActor.end())
	{
		return idToActor[id];
	}
	else
	{
		return nullptr;
	}
}

void DungeonLevel::MoveUp(Actor* actor)
{
	if (actor == nullptr) return;
	if (actor->As<Player>()) actor->As<Player>()->Move({ 0, -1 });
	if (actor->As<Monster>()) actor->As<Monster>()->Move({ 0, -1 });
}

void DungeonLevel::MoveDown(Actor* actor)
{
	if (actor == nullptr) return;
	if (actor->As<Player>()) actor->As<Player>()->Move({ 0, 1 });
	if (actor->As<Monster>()) actor->As<Monster>()->Move({ 0, 1 });
}

void DungeonLevel::MoveLeft(Actor* actor)
{
	if (actor == nullptr) return;
	if (actor->As<Player>()) actor->As<Player>()->Move({ -1, 0 });
	if (actor->As<Monster>()) actor->As<Monster>()->Move({ -1, 0 });
}

void DungeonLevel::MoveRight(Actor* actor)
{
	if (actor == nullptr) return;
	if (actor->As<Player>()) actor->As<Player>()->Move({ 1, 0 });
	if (actor->As<Monster>()) actor->As<Monster>()->Move({ 1, 0 });
}

void DungeonLevel::ReadDungeonFile(const char* fileName)
{
	char fullPath[32];
	const char* path = "../Assets/";

	sprintf_s(fullPath, sizeof(fullPath), "%s%s", path, fileName);

	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rt");

	if (file == nullptr)
	{
		std::cout << "던전 파일 읽기 실패" << '\n';
		__debugbreak();
		return;
	}

	int curRow = 0;

	while (!feof(file))
	{
		char buffer[100 + 1] = { };
		memset(buffer, 0, 101);

		fgets(buffer, 100 + 1, file);
		memcpy(dungeon[curRow], buffer, 100);

		curRow++;
	}

	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			switch (dungeon[i][j])
			{
			case '#':
				AddActor(new Road({ j, i }));
				break;
			case 'P':
				AddActor(new Road({ j, i }));
				AddActor(new Player({ j, i }, this));
				break;
			case 'M':
				AddActor(new Road({ j, i }));
				AddActor(new Monster({ j, i }, this));
				break;
			case 'T':
				AddActor(new Road({ j, i }));
				AddActor(new Task({ j, i }));
				break;
			case 'E':
				AddActor(new Road({ j, i }));
				AddActor(new Exit({ j, i }));
				break;
			}
		}
		std::cout << '\n';
	}

	fclose(file);
}

void DungeonLevel::BindActorID()
{
	int id = 1;
	auto it = Server::Get().clientSockets.begin();

	for (Actor* const actor : addRequestedActors)
	{
		Player* player = actor->As<Player>();
		if (player)
		{
			actor->SetActorID(id);
			idToActor.insert({ id, actor });
			clientToActor.insert({ *it, actor });

			Command command;
			command.dest = *it;
			command.data[0] = static_cast<char>(CommandType::ID);
			command.data[1] = id;

			Server::Get().writeQueue.push(command);

			++id;
			++it;
			continue;
		}

		Monster* monster = actor->As<Monster>();
		if (monster)
		{
			actor->SetActorID(id);
			idToActor.insert({ id, actor });
			clientToActor.insert({ *it, actor });

			Command command = { };
			command.dest = *it;
			command.data[0] = static_cast<char>(CommandType::ID);
			command.data[1] = id;

			Server::Get().writeQueue.push(command);

			++id;
			++it;
			continue;
		}
	}
}