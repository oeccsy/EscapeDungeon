#pragma once

#include "Core.h"

struct Engine_API Packet
{
	SOCKET src;
	char data[100];
};