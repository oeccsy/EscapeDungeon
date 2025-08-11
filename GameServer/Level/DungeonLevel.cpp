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

DungeonLevel::DungeonLevel()
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
		Command packet = server.readQueue.front();
		server.readQueue.pop();

		SOCKET client = packet.src;
		Actor* actor = clientToActor[client];
		if (actor == nullptr) continue;

		Command sendPacket = { };

		switch (packet.data[0])
		{
		case 'u':
			if (actor->As<Player>()) actor->As<Player>()->Move({ 0, -1 });
			if (actor->As<Monster>()) actor->As<Monster>()->Move({ 0, -1 });
			break;
		case 'd':
			if (actor->As<Player>()) actor->As<Player>()->Move({ 0, 1 });
			if (actor->As<Monster>()) actor->As<Monster>()->Move({ 0, 1 });
			break;
		case 'r':
			if (actor->As<Player>()) actor->As<Player>()->Move({ 1, 0 });
			if (actor->As<Monster>()) actor->As<Monster>()->Move({ 1, 0 });
			break;
		case 'l':
			if (actor->As<Player>()) actor->As<Player>()->Move({ -1, 0 });
			if (actor->As<Monster>()) actor->As<Monster>()->Move({ -1, 0 });
			break;
		}
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

	for (auto actor : actors)
	{
		if (actor->GetActorID() > 0)
		{
			Vector2 pos = actor->GetPosition();
			
			Command packet = { };
			packet.data[0] = 'p';
			packet.data[1] = actor->GetActorID();
			packet.data[2] = pos.x;
			packet.data[3] = pos.y;

			server.writeQueue.push(packet);
		}
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

			Command packet = { };
			packet.dest = *it;
			packet.data[0] = 'i';
			packet.data[1] = id;

			Server::Get().writeQueue.push(packet);

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

			Command packet = { };
			packet.dest = *it;
			packet.data[0] = 'i';
			packet.data[1] = id;

			Server::Get().writeQueue.push(packet);

			++id;
			++it;
			continue;
		}
	}
}