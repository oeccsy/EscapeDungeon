#include "CommandHandler.h"
#include "Level/ConnectLevel.h"
#include "Level/DungeonLevel.h"
#include "Utils/Logs.h"

#include "Networking/Server.h"

CommandHandler::CommandHandler(Level& level)
{
	this->level = &level;
}

void CommandHandler::Execute(Command command)
{
	ConnectLevel* connectLevel = level->As<ConnectLevel>();

	switch (static_cast<CommandType>(command.data[0]))
	{
	case CommandType::Connect:
		break;
	case CommandType::Disconnect:
		break;
	case CommandType::NewPlayer:
		if (connectLevel) connectLevel->NewPlayerJoined(command.data[1]);
		break;
	case CommandType::Ready:
		break;
	case CommandType::GameStart:
		if (connectLevel) connectLevel->GameStart();
		break;
	case CommandType::ID:
		break;
	case CommandType::Position:
		break;
	case CommandType::Up:

		break;
	case CommandType::Down:

		break;
	case CommandType::Left:

		break;
	case CommandType::Right:

		break;
	default:
		break;
	}
}