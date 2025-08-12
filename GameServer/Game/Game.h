#pragma once

#include "Engine.h"
#include "Networking/Server.h"
#include "Utils/Logs.h"

class Game
{
public:
	Game();
	~Game();

	void LoadDungeonLevel(int index);
	void Run();

	static Game& Get();

private:
	Engine engine;
	Server server;
	Logs logs;

	static Game* instance;
};