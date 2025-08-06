#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	std::cout << "==== 서버 시작 ====" << '\n';

	std::cout << "==== 라이브러리 초기화 ====" << '\n';
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData)) return 0;

	std::cout << "==== 서버 소켓 생성 ====" << '\n';
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Socket ErrorCode : " << errCode << '\n';
		return 0;
	}

	std::cout << "==== 주소 지정 ====" << '\n';
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	std::cout << "==== Bind ====" << '\n';
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Bind Error : " << errCode << '\n';
		return 0;
	}

	std::cout << "==== Listen ====" << '\n';
	const int backlog = 10;
	if (::listen(listenSocket, backlog) == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Listen Error : " << errCode << '\n';
		return 0;
	}

	std::cout << "==== Accept ====" << '\n';
	while (true)
	{
		std::cout << "==== Accept ====" << '\n';

		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			int errCode = ::WSAGetLastError();
			std::cout << "Accept Error : " << errCode << '\n';
			return 0;
		}

		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		std::cout << "Client Connected! IP = " << ipAddress << '\n';

		std::cout << "==== Send/Receive ====" << '\n';
		while (true)
		{
			std::cout << "==== Send/Receive ====" << '\n';
			char recvBuffer[1000];

			int recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

			if (recvLen == 0)
			{
				std::cout << "Client Disconnected" << '\n';
				break;
			}

			if (recvLen < 0)
			{
				int errCode = ::WSAGetLastError();
				std::cout << "Receive Error : " << errCode << '\n';
				return 0;
			}

			std::cout << "Recv Data! Data = " << recvBuffer << '\n';
			std::cout << "Recv Data! Len = " << recvLen << '\n';

			int resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
			if (resultCode == SOCKET_ERROR)
			{
				int errCode = ::WSAGetLastError();
				std::cout << "Send Error : " << errCode << '\n';
				return 0;
			}
		}

		::closesocket(clientSocket);
	}

	::closesocket(listenSocket);
	::WSACleanup();

	std::cin.get();
	return 0;
}