#include "DungeonLevel.h"

#include "Engine.h"

#include "Networking/Server.h"
#include "Networking/Packet.h"

#include "Actor/Actor.h"
#include "Actor/Player.h"
#include "Actor/Monster.h"
#include "Actor/Task.h"
#include "Actor/Road.h"
#include "Actor/Exit.h"

#include "Utils/Utils.h"

#include <iostream>
#include <vector>

DungeonLevel::DungeonLevel()
{
	ReadDungeonFile("Map_2.txt");
	BindActorID();
	InitUI();

	logs.push_back("==== 던전 시작 ====");
}

DungeonLevel::~DungeonLevel() {}


void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	Server& server = Server::Get();

	server.Recv();

	while (!server.readQueue.empty())
	{
		Packet packet = server.readQueue.front();
		server.readQueue.pop();

		SOCKET client = packet.src;
		Actor* actor = clientToActor[client];
		if (actor == nullptr) continue;

		Packet sendPacket = { };

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
			
			Packet packet = { };
			packet.data[0] = 'p';
			packet.data[1] = actor->GetActorID();
			packet.data[2] = pos.x;
			packet.data[3] = pos.y;

			server.writeQueue.push(packet);
		}
	}

	
	while (!server.writeQueue.empty())
	{
		Packet packet = server.writeQueue.front();
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

	char playerStaminaText[20] = { };
	
	for (int i = 0; i < Player::MAX_STAMINA; i++)
	{
		if (player == nullptr) break;
		playerStaminaText[i] = (i < player->GetStamina()) ? 'O' : ' ';
		playerStaminaText[i + 1] = '\0';
	}

	Utils::SetConsolePosition(Vector2(100, 1));
	Utils::SetConsoleTextColor(Color::White);
	std::cout << playerStaminaText;

	char monsterStaminaText[20] = { };

	for (int i = 0; i < Monster::MAX_STAMINA; i++)
	{
		monsterStaminaText[i] = (i < monster->GetStamina()) ? 'X' : ' ';
		monsterStaminaText[i + 1] = '\0';
	}

	Utils::SetConsolePosition(Vector2(100, 3));
	Utils::SetConsoleTextColor(Color::White);
	std::cout << monsterStaminaText;

	Utils::SetConsoleTextColor(Color::White);

	for (int i = 1; i <= 20; ++i)
	{
		if (logs.size() < i) break;

		std::string log = logs[logs.size() - i];
		Utils::SetConsolePosition(Vector2(92, 38 - i));
		std::cout << "                    ";
		Utils::SetConsolePosition(Vector2(92, 38 - i));
		std::cout << log;
	}
}

bool DungeonLevel::Movable(const Vector2& targetPos)
{
	return dungeon[targetPos.y][targetPos.x] == '#';
}

void DungeonLevel::InitUI()
{
	Utils::SetConsoleTextColor(Color::White);

	Utils::SetConsolePosition(Vector2(90, 7));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";

	for (int i = 1; i <= 30; ++i)
	{
		Utils::SetConsolePosition(Vector2(90, 38 - i));
		std::cout << "■                                        ■" << '\n';
	}

	Utils::SetConsolePosition(Vector2(90, 38));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
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
				AddActor(player = new Player({ j, i }, this));
				break;
			case 'M':
				AddActor(new Road({ j, i }));
				AddActor(monster = new Monster({ j, i }, this));
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

			Packet packet = { };
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

			Packet packet = { };
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