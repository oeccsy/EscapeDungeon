#include "DungeonLevel.h"

#include "Actor/Player.h"
#include "Actor/Road.h"

#include <iostream>

DungeonLevel::DungeonLevel()
{
	ReadDungeonFile("Map_1.txt");

	AddActor(new Player({ 3, 3 }, this));
}

DungeonLevel::~DungeonLevel() {}

void DungeonLevel::BeginPlay()
{
	super::BeginPlay();
}

void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void DungeonLevel::Render()
{
	super::Render();
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
			}
		}
		std::cout << '\n';
	}

	fclose(file);
}
