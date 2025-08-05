#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"

class DungeonLevel : public Level
{
	RTTI_DECLARATIONS(DungeonLevel, Level)

public:
	DungeonLevel();
	~DungeonLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void ReadDungeonFile(const char* fileName);

private:
	char dungeon[100][100] = { };
};