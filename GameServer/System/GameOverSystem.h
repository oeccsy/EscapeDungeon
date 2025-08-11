#pragma once

#include "Level/Level.h"

class GameOverSystem
{
public:
	GameOverSystem();
	GameOverSystem(int playerCount);
	~GameOverSystem() = default;

	void CheckGameOver();
	void GameOver();

private:
	int playerCount;
};