#pragma once

#include "Core.h"

struct Engine_API Command
{
	SOCKET src = INVALID_SOCKET;
	SOCKET dest = INVALID_SOCKET;
	char data[100];
};