#pragma once

#include "Level/Level.h"
#include "Interface/IMovable.h"
#include "Actor/Player.h"
#include "Gimmick/ExitSystem.h"
#include "Gimmick/GameOverSystem.h"

class DungeonLevel : public Level, public IMovable
{
	RTTI_DECLARATIONS(DungeonLevel, Level)

public:
	DungeonLevel();
	~DungeonLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	virtual bool Movable(const Vector2& targetPos);

private:
	void ReadDungeonFile(const char* fileName);

private:
	char dungeon[100][100] = { };
	Player* player;

	ExitSystem exitSystem;
	GameOverSystem gameOverSystem;
};