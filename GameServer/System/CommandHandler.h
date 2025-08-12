#pragma once

#include "Level/Level.h"
#include "Networking/Command.h"

enum class CommandType : char
{
	Connect = 1,
	Disconnect = 2,
	
	NewPlayer = 3,
	Ready = 4,
	GameStart = 5,

	ID = 6,

	Position = 7,
	Up = 8,
	Down = 9,
	Left = 10,
	Right = 11
};

class CommandHandler
{
public:
	CommandHandler(Level& level);
	~CommandHandler() = default;

	void Execute(Command command);

private:
	Level* level = nullptr;
};