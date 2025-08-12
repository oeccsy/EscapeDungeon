#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void Multiplexing()
{
    std::cout << "==== 서버 시작 ====" << '\n';

    std::cout << "==== 라이브러리 초기화 ====" << '\n';
    WSAData wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData)) return;

    std::cout << "==== 서버 소켓 생성 ====" << '\n';
    SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        int errCode = ::WSAGetLastError();
        std::cout << "Socket ErrorCode : " << errCode << '\n';
        return;
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
        return;
    }

    std::cout << "==== Listen ====" << '\n';
    const int backlog = 10;
    if (::listen(listenSocket, backlog) == SOCKET_ERROR)
    {
        int errCode = ::WSAGetLastError();
        std::cout << "Listen Error : " << errCode << '\n';
        return;
    }


    std::cout << "==== Multiplexing ====" << '\n';

    fd_set readSet;
    fd_set writeSet;

    FD_ZERO(&readSet);
    FD_SET(listenSocket, &readSet);

    while (true)
    {
        fd_set readSetCopy = readSet;
        TIMEVAL timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        int fdNum = select(0, &readSetCopy, 0, 0, &timeout);

        if (fdNum == SOCKET_ERROR)
        {
            std::cout << "Multiplexing Error" << '\n';
            break;
        }

        if (fdNum == 0) continue;

        for (int i = 0; i < readSet.fd_count; ++i)
        {
            if (FD_ISSET(readSet.fd_array[i], &readSetCopy) == false) continue;

            if (readSet.fd_array[i] == listenSocket)
            {
                SOCKADDR_IN clientAddr;
                ::memset(&clientAddr, 0, sizeof(clientAddr));
                int addrLen = sizeof(clientAddr);

                SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
                if (clientSocket == INVALID_SOCKET)
                {
                    int errCode = ::WSAGetLastError();
                    std::cout << "Accept Error : " << errCode << '\n';
                    return;
                }

                FD_SET(clientSocket, &readSet);
            }
            else
            {
                char recvBuffer[100];

                int recvLen = ::recv(readSet.fd_array[i], recvBuffer, sizeof(recvBuffer), 0);

                if (recvLen == 0)
                {
                    FD_CLR(readSet.fd_array[i], &readSet);
                    closesocket(readSetCopy.fd_array[i]);
                    std::cout << "Client Disconnected" << '\n';
                    break;
                }

                if (recvLen < 0)
                {
                    int errCode = ::WSAGetLastError();
                    std::cout << "Receive Error : " << errCode << '\n';
                    return;
                }

                std::cout << "Recv Data! Data = " << recvBuffer << '\n';
                std::cout << "Recv Data! Len = " << recvLen << '\n';

                int resultCode = ::send(readSet.fd_array[i], recvBuffer, recvLen, 0);
                if (resultCode == SOCKET_ERROR)
                {
                    int errCode = ::WSAGetLastError();
                    std::cout << "Send Error : " << errCode << '\n';
                    return;
                }
            }
        }
    }

    ::closesocket(listenSocket);
    ::WSACleanup();

    std::cin.get();
}