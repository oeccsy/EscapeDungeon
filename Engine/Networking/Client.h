#pragma once

#include "Core.h"

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Engine_API Client
{
public:
	Client();
	~Client();

	bool InitSocket();
	bool Connect();
	bool Send(char* data, int size);
	bool Recv();
	void Close();
	
private:
	SOCKET clientSocket = INVALID_SOCKET;
	SOCKADDR_IN serverAddr;

	bool isConnected = false;
};