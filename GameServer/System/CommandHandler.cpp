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
		if (connectLevel) connectLevel->Connect();
		break;
	case CommandType::Disconnect:
		if (connectLevel) connectLevel->Disconnect();
		break;
	case CommandType::NewPlayer:
		break;
	case CommandType::Ready:
		if (connectLevel) connectLevel->Ready();
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