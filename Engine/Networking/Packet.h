#pragma once

#include "Core.h"

struct Engine_API Packet
{
	SOCKET src;
	SOCKET dest;
	char data[100];
};