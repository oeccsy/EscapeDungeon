#include "CommandHandler.h"
#include "Level/ConnectLevel.h"
#include "Level/DungeonLevel.h"

CommandHandler::CommandHandler(Level& level)
{
	this->level = &level;
}

void CommandHandler::Execute(Command command)
{
	switch (static_cast<CommandType>(command.data[0]))
	{
	case CommandType::Connect:
		Connect(command);
		break;
	case CommandType::Disconnect:
		Disconnect(command);
		break;
	case CommandType::Ready:
		Ready(command);
		break;
	case CommandType::StartGame:
		StartGame(command);
		break;
	case CommandType::Up:
		Up(command);
		break;
	case CommandType::Down:
		Down(command);
		break;
	case CommandType::Left:
		Left(command);
		break;
	case CommandType::Right:
		Right(command);
		break;
	default:
		break;
	}
}

void CommandHandler::Connect(Command command)
{
	ConnectLevel* connectLevel = level->As<ConnectLevel>();
	if (!connectLevel) return;

	connectLevel->PlayerJoin();
}

void CommandHandler::Disconnect(Command command)
{
}

void CommandHandler::Ready(Command command)
{
}

void CommandHandler::StartGame(Command command)
{
}

void CommandHandler::Up(Command command)
{
}

void CommandHandler::Down(Command command)
{
}

void CommandHandler::Left(Command command)
{
}

void CommandHandler::Right(Command command)
{
}
