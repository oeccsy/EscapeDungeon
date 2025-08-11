#pragma once

#include "Networking/Command.h"

enum class CommandType :int
{
	Connect = 1,
	Disconnect = 2,
	Message = 3,
	Move = 4,
	Action = 5,
};

class CommandHandler
{
public:
	CommandHandler() = default;
	~CommandHandler() = default;

	void Execute(Command command);

};