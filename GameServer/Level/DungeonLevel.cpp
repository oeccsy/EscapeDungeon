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
	ReadDungeonFile("Map_1.txt");
	RenderDungeon();
	BindActorID();
	
	uiSystem.InitLogArea();
	
	Logs::Get().AddLog({ "==== 게임 시작 ====" });
}

DungeonLevel::DungeonLevel(int index) : commandHandler(*this)
{
	char fileName[20];
	sprintf_s(fileName, sizeof(fileName), "Map_%d.txt", index);
	
	ReadDungeonFile(fileName);
	RenderDungeon();
	BindActorID();

	RenderDungeon();
	uiSystem.InitLogArea();

	Logs::Get().AddLog({ "==== 게임 시작 ====" });
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

void DungeonLevel::Move(Actor* actor, Vector2 dir)
{
	if (actor == nullptr) return;
	
	Vector2 prevPos = actor->GetPosition();
	
	if (actor->As<Player>()) actor->As<Player>()->Move(dir);
	if (actor->As<Monster>()) actor->As<Monster>()->Move(dir);
	
	Vector2 afterPos = actor->GetPosition();
	
	if (prevPos != afterPos) RenderDungeon(prevPos.y, prevPos.x);
}

void DungeonLevel::MoveUp(Actor* actor)
{
	Move(actor, { 0, -1 });
}

void DungeonLevel::MoveDown(Actor* actor)
{
	Move(actor, { 0, 1 });
}

void DungeonLevel::MoveLeft(Actor* actor)
{
	Move(actor, { -1, 0});
}

void DungeonLevel::MoveRight(Actor* actor)
{
	Move(actor, { 1, 0 });
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
			case 'P':
				AddActor(new Player({ j, i }, this));
				dungeon[i][j] = '#';
				break;
			case 'M':
				AddActor(new Monster({ j, i }, this));
				dungeon[i][j] = '#';
				break;
			case 'T':
				AddActor(new Task({ j, i }));
				dungeon[i][j] = '#';
				break;
			case 'E':
				AddActor(new Exit({ j, i }));
				dungeon[i][j] = '#';
				break;
			}
		}
		std::cout << '\n';
	}

	fclose(file);
}

void DungeonLevel::RenderDungeon()
{
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			if (dungeon[i][j] != '#') continue;
			
			Utils::SetConsoleTextColor(Color::White);
			Utils::SetConsolePosition(Vector2(j, i));
			std::cout << dungeon[i][j];
		}
	}
}

void DungeonLevel::RenderDungeon(int row, int col)
{
	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(col, row));
	std::cout << dungeon[row][col];
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
