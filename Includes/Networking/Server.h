#pragma once


#include "Core.h"

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <vector>

class Engine_API Server
{
public:
	Server();
	~Server();

	bool InitSocket();
	bool Bind();
	bool Listen();
	bool Accept();
	bool Send(char* data, int size);
	bool Recv();
	void Close();

private:
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKADDR_IN serverAddr;

	std::vector<SOCKET> clientSockets;
	fd_set readSet;
	fd_set writeSet;
};