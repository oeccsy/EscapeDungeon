#pragma once

#include "Engine.h"
#include "Networking/Client.h"
#include "Utils/Logs.h"

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
	Client client;
	Logs logs;

	static Game* instance;
};