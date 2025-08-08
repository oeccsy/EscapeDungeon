#include "DungeonLevel.h"

#include "Engine.h"

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
	ReadDungeonFile("Map_1.txt");
	InitUI();

	player = new Player({ 3, 3 }, this);
	monster = new Monster({ 70, 20 }, this);

	AddActor(player);
	AddActor(monster);
	AddActor(new Task({5, 5}));
}

DungeonLevel::~DungeonLevel() {}

void DungeonLevel::BeginPlay()
{
	super::BeginPlay();
}

void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

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
}

void DungeonLevel::Render()
{
	super::Render();

	char playerStaminaText[20] = { };
	
	for (int i = 0; i < Player::MAX_STAMINA; i++)
	{
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
			}
		}
		std::cout << '\n';
	}

	fclose(file);
}
