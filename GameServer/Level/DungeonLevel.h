#pragma once

#include "Core.h"

#include "Level/Level.h"
#include "Interface/IMovable.h"
#include "System/InteractionSystem.h"
#include "System/GameOverSystem.h"
#include "System/UISystem.h"

#include <unordered_map>
#include <vector>
#include <string>

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
	void ReadDungeonFile(const char* fileName);
	void BindActorID();

private:
	char dungeon[100][100] = { };

	std::unordered_map<SOCKET, Actor*> clientToActor;
	std::unordered_map<int, Actor*> idToActor;

	InteractionSystem interactionSystem;
	GameOverSystem gameOverSystem;
	UISystem uiSystem;
};