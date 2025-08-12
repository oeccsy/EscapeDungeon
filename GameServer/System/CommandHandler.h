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
	Stamina = 8,

	Up = 9,
	Down = 10,
	Left = 11,
	Right = 12
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