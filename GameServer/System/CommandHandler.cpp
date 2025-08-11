#include "CommandHandler.h"

void CommandHandler::Execute(Command command)
{
	switch (static_cast<CommandType>(command.data[0]))
	{
	case CommandType::Connect:

		break;
	case CommandType::Disconnect:

		break;
	case CommandType::Message:

		break;
	case CommandType::Move:

		break;
	case CommandType::Action:
		break;
	default:
		break;
	}
}
