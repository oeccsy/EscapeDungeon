#include "DungeonLevel.h"

#include "Engine.h"

#include "Networking//Client.h"
#include "Networking/Packet.h"

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
}

DungeonLevel::~DungeonLevel() {}

void DungeonLevel::BeginPlay()
{
	super::BeginPlay();
}

void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	
	Client& client = Client::Get();

	while (true)
	{
		Packet packet = { };

		bool success = client.Recv(packet.data, sizeof(packet.data));
		if (success == false) break;

		client.readQueue.push(packet);
	}

	while (!client.readQueue.empty())
	{
		Packet packet = client.readQueue.front();
		client.readQueue.pop();

		Actor* actor = nullptr;
		int actorID = -1;
		int posX = -1;
		int posY = -1;

		Player* player = nullptr;

		switch (packet.data[0])
		{
		case 'i':
			ownID = packet.data[1];
			logs.push_back("ID를 부여받았습니다. : " + std::to_string(ownID));

			actor = idToActor[ownID];

			if (actor->As<Player>()) actor->As<Player>()->SetOwner(true);
			if (actor->As<Monster>()) actor->As<Monster>()->SetOwner(true);
			break;
		case 'p':
			actorID = packet.data[1];
			posX = packet.data[2];
			posY = packet.data[3];

			if (idToActor.find(actorID) == idToActor.end()) break;

			actor = idToActor.find(actorID)->second;
			actor->SetPosition({ posX, posY });
			break;
		case 'k':
			actorID = packet.data[1];

			if (idToActor.find(actorID) == idToActor.end()) break;

			player = idToActor.find(actorID)->second->As<Player>();
			player->Die();

			logs.push_back(actorID + " 플레이어가 몬스터의 밥도둑이 되었다.");
			break;
		case 'e':
			int actorID = packet.data[1];

			if (idToActor.find(actorID) == idToActor.end()) break;

			player = idToActor.find(actorID)->second->As<Player>();
			player->Escape();

			logs.push_back(actorID + " 플레이어가 탈출에 성공했다!");
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

	//interactionSystem.ProgressTask(tasks, players, deltaTime);
	//interactionSystem.CheckOpenExit(*this, 1);
	//interactionSystem.EscapePlayer(exits, players);
	//interactionSystem.KillPlayer(monster, players);

	gameOverSystem.CheckGameOver();

	while (!client.writeQueue.empty())
	{
		Packet packet = client.writeQueue.front();
		client.writeQueue.pop();

		client.Send(packet.data, sizeof(packet.data));
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
	char targetPosActor = dungeon[targetPos.y][targetPos.x];
	if (targetPosActor == '#' || targetPosActor == 'T' || targetPosActor == 'E') return true;

	return false;
}

void DungeonLevel::InitUI()
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
}