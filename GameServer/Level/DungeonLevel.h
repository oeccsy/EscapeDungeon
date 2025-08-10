#pragma once

#include "Level/Level.h"
#include "Interface/IMovable.h"
#include "Gimmick/InteractionSystem.h"
#include "Gimmick/GameOverSystem.h"

class Player;
class Monster;

class DungeonLevel : public Level, public IMovable
{
	RTTI_DECLARATIONS(DungeonLevel, Level)

public:
	DungeonLevel();
	~DungeonLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	virtual bool Movable(const Vector2& targetPos);

private:
	void InitUI();

	void ReadDungeonFile(const char* fileName);
	
	void BindClientID();
	void SpawnActors();

private:
	char dungeon[100][100] = { };
	Player* player;
	Monster* monster;

	InteractionSystem interactionSystem;
	GameOverSystem gameOverSystem;
};