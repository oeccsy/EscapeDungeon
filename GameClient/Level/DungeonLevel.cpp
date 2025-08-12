#include "DungeonLevel.h"

#include "Engine.h"

#include "Networking//Client.h"
#include "Networking/Command.h"


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
	
	uiSystem.InitLogArea();
	
	Logs::Get().AddLog({ "==== 게임 시작 ====" });
}

DungeonLevel::~DungeonLevel() {}

void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	
	Client& client = Client::Get();

	while (true)
	{
		Command command;

		bool success = client.Recv(command.data, sizeof(command.data));
		if (success == false) break;

		client.readQueue.push(command);
	}

	while (!client.readQueue.empty())
	{
		Command command = client.readQueue.front();
		client.readQueue.pop();

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
	}

	interactionSystem.ProgressTask(tasks, players, deltaTime);
	interactionSystem.CheckOpenExit(*this, 1);
	interactionSystem.EscapePlayer(exits, players);
	interactionSystem.KillPlayer(monster, players);

	gameOverSystem.CheckGameOver();

	while (!client.writeQueue.empty())
	{
		Command packet = client.writeQueue.front();
		client.writeQueue.pop();

		client.Send(packet.data, sizeof(packet.data));
	}
}

void DungeonLevel::Render()
{
	super::Render();

	if (idToActor.find(ownID) == idToActor.end()) return;

	Player* player = nullptr;
	Monster* monster = nullptr;

	for (Actor* const actor : actors)
	{
		if (actor->GetActorID() == ownID)
		{
			if (actor->As<Player>()) player = actor->As<Player>();
			if (actor->As<Monster>()) monster = actor->As<Monster>();
		}
	}

	uiSystem.RenderStaminaUI(player);
	uiSystem.RenderStaminaUI(monster);
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
				dungeon[i][j] = '#';
				break;
			case 'M':
				AddActor(new Road({ j, i }));
				AddActor(new Monster({ j, i }, this));
				dungeon[i][j] = '#';
				break;
			case 'T':
				AddActor(new Road({ j, i }));
				AddActor(new Task({ j, i }));
				dungeon[i][j] = '#';
				break;
			case 'E':
				AddActor(new Road({ j, i }));
				AddActor(new Exit({ j, i }));
				dungeon[i][j] = '#';
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

	for (Actor* const actor : addRequestedActors)
	{
		Player* player = actor->As<Player>();
		if (player)
		{
			actor->SetActorID(id);
			idToActor.insert({ id, actor });

			++id;
			continue;
		}

		Monster* monster = actor->As<Monster>();
		if (monster)
		{
			actor->SetActorID(id);
			idToActor.insert({ id, actor });

			++id;
			continue;
		}
	}
}

void DungeonLevel::SetID(int id)
{
	ownID = id;

	char logData[100];
	sprintf_s(logData, sizeof(logData), "ID를 부여받았습니다. : %d", ownID);
	Logs::Get().AddLog({ logData });

	if (idToActor.find(ownID) == idToActor.end()) return;
	Actor* actor = idToActor[ownID];

	if (actor->As<Player>()) actor->As<Player>()->SetOwner(true);
	if (actor->As<Monster>()) actor->As<Monster>()->SetOwner(true);
}

void DungeonLevel::UpdateActorPositionByID(int id, const Vector2& pos)
{
	if (idToActor.find(id) == idToActor.end()) return;
	
	idToActor[id]->SetPosition(pos);
}

void DungeonLevel::UpdateActorStaminaByID(int id, int stamina)
{
	if (idToActor.find(id) == idToActor.end()) return;
	Actor* actor = idToActor[id];
	
	if (actor->As<Player>()) actor->As<Player>()->SetStamina(stamina);
	if (actor->As<Monster>()) actor->As<Monster>()->SetStamina(stamina);
}

void DungeonLevel::KillPlayer(int id)
{
	if (idToActor.find(id) == idToActor.end()) return;

	Player* player = idToActor[id]->As<Player>();
	player->Die();

	char logData[100];
	sprintf_s(logData, sizeof(logData), "플레이어 %d 가 사망했습니다!", id);
	Logs::Get().AddLog(logData);
}

void DungeonLevel::EscapePlayer(int id)
{
	if (idToActor.find(id) == idToActor.end()) return;

	Player* player = idToActor[id]->As<Player>();
	player->Escape();

	char logData[100];
	sprintf_s(logData, sizeof(logData), "플레이어 %d 가 탈출에 성공했다!", id);
	Logs::Get().AddLog(logData);
}
