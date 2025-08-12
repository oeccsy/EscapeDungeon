#pragma once

#include "Level/Level.h"

class GameOverSystem
{
public:
	GameOverSystem() = default;
	~GameOverSystem() = default;

	void CheckGameOver();
	void GameOver();

public:
	static bool isGameOver;
};