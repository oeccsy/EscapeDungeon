#include "Server.h"

#include <iostream>

Server* Server::instance = nullptr;

Server::Server()
{
	instance = this;
}

Server::~Server() {}

bool Server::InitSocket()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "Init Winsock Fail" << '\n';
		return false;
	}

	listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Socket Error : " << errCode << '\n';
		return false;
	}

	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	return true;
}

bool Server::Bind()
{
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Bind Error" << '\n';
		return false;
	}

	int result = ::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	if (result == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Bind Error : " << errCode << '\n';
		return false;
	}

	return true;
}

bool Server::Listen()
{
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Listen Error" << '\n';
		return false;
	}

	int backlog = 10;
	int result = ::listen(listenSocket, backlog);
	
	if (result == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Listen Error : " << errCode << '\n';
		return false;
	}

	return true;
}

bool Server::Accept()
{
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Accept Error" << '\n';
		return false;
	}

	fd_set readSetCopy = readSet;
	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int fdNum = select(0, &readSetCopy, 0, 0, &timeout);

	if (fdNum == SOCKET_ERROR)
	{
		std::cout << "Accept Error" << '\n';
		return false;
	}

	if (FD_ISSET(listenSocket, &readSetCopy) == false) return false;

	SOCKADDR_IN clientAddr;
	::memset(&clientAddr, 0, sizeof(clientAddr));
	int addrLen = sizeof(clientAddr);

	SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
	if (clientSocket == INVALID_SOCKET)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Accept Error : " << errCode << '\n';
		return false;
	}

	FD_SET(clientSocket, &readSet);
	clientSockets.push_back(clientSocket);

	/*Packet packet = { };
	packet.src = clientSocket;
	packet.data[0] = 'n';

	readQueue.push(packet);*/

	return true;
}

bool Server::Send(SOCKET clientSocket, char* data, int size)
{
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Send Error" << '\n';
		return false;
	}

	int result = ::send(clientSocket, data, size, 0);

	if (result == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Send Error : " << errCode << '\n';
		return false;
	}

	return true;
}

bool Server::SendAll(char* data, int size)
{
	bool success = true;

	for (auto clientSocket : clientSockets)
	{
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Send Error" << '\n';
			success = false;
			continue;
		}

		int result = ::send(clientSocket, data, size, 0);

		if (result == SOCKET_ERROR)
		{
			int errCode = ::WSAGetLastError();
			std::cout << "Send Error : " << errCode << '\n';
			success = false;
			continue;
		}
	}

	return success;
}

void Server::Recv()
{
	fd_set readSetCopy = readSet;
	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int fdNum = select(0, &readSetCopy, 0, 0, &timeout);

	if (fdNum == SOCKET_ERROR)
	{
		std::cout << "Recv Error" << '\n';
		return;
	}

	if (fdNum == 0) return;

	for (int i = 0; i < (int)readSet.fd_count; ++i)
	{
		SOCKET curSocket = readSet.fd_array[i];

		if (FD_ISSET(curSocket, &readSetCopy) == false) continue;

		Packet packet = { };
		packet.src = curSocket;
		int recvLen = ::recv(curSocket, packet.data, sizeof(packet.data), 0);

		if (recvLen == 0)
		{
			FD_CLR(curSocket, &readSet);
			closesocket(curSocket);
			std::cout << "Client Disconnected" << '\n';
			continue;
		}

		if (recvLen < 0)
		{
			int errCode = ::WSAGetLastError();
			std::cout << "Receive Error : " << errCode << '\n';
			continue;
		}

		readQueue.push(packet);
	}
}

void Server::Close()
{
	::closesocket(listenSocket);
	::WSACleanup();
}

Server& Server::Get()
{
	return *instance;
}