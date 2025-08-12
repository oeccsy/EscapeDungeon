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
	if (level == nullptr) return;

	ConnectLevel* connectLevel = level->As<ConnectLevel>();
	DungeonLevel* dungeonLevel = level->As<DungeonLevel>();

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
		if (connectLevel) connectLevel->GameStart(command.data[1]);
		break;
	case CommandType::ID:
		if (dungeonLevel) dungeonLevel->SetID(command.data[1]);
		break;
	case CommandType::Position:
		if (dungeonLevel) dungeonLevel->UpdateActorPositionByID(command.data[1], { command.data[2], command.data[3] });
		break;
	case CommandType::Stamina:
		if (dungeonLevel) dungeonLevel->UpdateActorStaminaByID(command.data[1], command.data[2]);
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