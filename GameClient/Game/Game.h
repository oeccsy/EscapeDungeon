#pragma once

#include "Engine.h"

class Game
{
public:
	Game();
	~Game();

	void Run();
	
private:
	Engine engine;
};