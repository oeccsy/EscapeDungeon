#include "Server.h"

#include <iostream>

Server::Server() {}

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

	clientSockets.push_back(clientSocket);

	return true;
}

bool Server::Send(char* data, int size)
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

	return true;
}

bool Server::Recv()
{
	return true;
}

void Server::Close()
{

}
