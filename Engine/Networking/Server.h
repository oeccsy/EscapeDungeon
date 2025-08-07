#pragma once

#include "Core.h"
#include "Packet.h"

#include <vector>
#include <queue>

class Engine_API Server
{
public:
	Server();
	~Server();

	bool InitSocket();
	bool Bind();
	bool Listen();
	bool Accept();

	bool Send(SOCKET socket, char* data, int size);
	bool SendAll(char* data, int size);
	void Recv();

	void Close();

	static Server& Get();

public:
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKADDR_IN serverAddr;

	std::vector<SOCKET> clientSockets;
	fd_set readSet;
	fd_set writeSet;

	std::queue<Packet> packets;

	static Server* instance;
};