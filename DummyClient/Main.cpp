#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	std::cout << "==== 클라 시작 ====" << '\n';

	std::cout << "==== 라이브러리 초기화 ====" << '\n';
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData)) return 0;

	std::cout << "==== 클라 소켓 생성 ====" << '\n';
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
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

	std::cout << "==== Connect ====" << '\n';

	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "Connect ErrorCode : " << errCode << '\n';
		return 0;
	}

	std::cout << "Connected to Server!" << '\n';

	while (true)
	{
		char sendBuffer[100];
		std::cin >> sendBuffer;

		if (strcmp(sendBuffer, "quit") == 0)
		{
			::shutdown(clientSocket, SD_SEND);
			break;
		}

		int resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
		{
			int errCode = ::WSAGetLastError();
			std::cout << "Send ErrorCode : " << errCode << '\n';
			return 0;
		}

		std::cout << "Send Data! Len = " << sizeof(sendBuffer) << '\n';

		 char recvBuffer[1000];
		
		 int recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		 
		 if (recvLen == 0)
		 {
			 std::cout << "Server Disconnected" << '\n';
			 break;
		 }

		 if (recvLen < 0)
		 {
		     int errCode = ::WSAGetLastError();
		     std::cout << "Recv ErrorCode : " << errCode << '\n';
		     return 0;
		 }
		
		 std::cout << "Recv Data! Data = " << recvBuffer << '\n';
		 std::cout << "Recv Data! Len = " << recvLen << '\n';
	}

	::closesocket(clientSocket);
	::WSACleanup();

	std::cin.get();
	std::cin.get();
	return 0;
}