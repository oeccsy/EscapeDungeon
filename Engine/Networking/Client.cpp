#include "Client.h"

#include <iostream>

Client::Client() { }

Client::~Client() { }

bool Client::InitSocket()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "Init Winsock Fail" << '\n';
		return false;
	}

	clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
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

bool Client::Connect()
{
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Connect Error" << '\n';
		return false;
	}

	int result = ::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	
	if (result == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Connect Error : " << errCode << '\n';
		return false;
	}

	isConnected = true;
	return true;
}

bool Client::Send(char* data, int size)
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

bool Client::Recv(char* buffer, int size)
{
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Recv Error" << '\n';
		return false;
	}

	fd_set readSetCopy = readSet;
	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int fdNum = select(0, &readSetCopy, 0, 0, &timeout);

	if (fdNum == SOCKET_ERROR)
	{
		std::cout << "Recv Error" << '\n';
		return false;
	}

	if (FD_ISSET(clientSocket, &readSetCopy) == false) return false;

	int recvLen = ::recv(clientSocket, buffer, size, 0);

	if (recvLen == 0)
	{
		FD_CLR(clientSocket, &readSet);
		closesocket(clientSocket);
		std::cout << "Server Disconnected" << '\n';
		return true;
	}

	if (recvLen < 0)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Recv ErrorCode : " << errCode << '\n';
		return false;
	}

	return true;
}

void Client::Close()
{
	if(isConnected) ::shutdown(clientSocket, SD_SEND);
	if(clientSocket != INVALID_SOCKET) ::closesocket(clientSocket);
	::WSACleanup();
}
