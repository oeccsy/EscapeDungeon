#pragma once

#include "Engine.h"
#include "Networking/Server.h"

class Game
{
public:
	Game();
	~Game();

	void LoadDungeonLevel();
	void Run();

	static Game& Get();

private:
	Engine engine;
	Server server;

	static Game* instance;
};