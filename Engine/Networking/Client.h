#pragma once

#include "Core.h"
#include "Packet.h"

#include <queue>

class Engine_API Client
{
public:
	Client();
	~Client();

	bool InitSocket();
	bool Connect();
	bool Send(char* data, int size);
	bool Recv(char* buffer, int size);
	void Close();

	static Client& Get();
	
public:
	SOCKET clientSocket = INVALID_SOCKET;
	SOCKADDR_IN serverAddr = { };

	fd_set readSet;

	std::queue<Packet> readQueue;
	std::queue<Packet> writeQueue;

	bool isConnected = false;

	static Client* instance;
};