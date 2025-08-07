#pragma once

#include "Core.h"

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