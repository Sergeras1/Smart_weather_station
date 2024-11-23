#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma warning(disable : 4996)

SOCKET Connections[100];
int Counter = 0;

void ClientHandler(int index) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(Connections[index], buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            closesocket(Connections[index]);
            Connections[index] = INVALID_SOCKET;
            std::cout << "Client disconnected!" << std::endl;
            return;
        }

        buffer[bytesReceived] = '\0';  
        for (int i = 0; i < Counter; i++) {
            if (i != index && Connections[i] != INVALID_SOCKET) {
                send(Connections[i], buffer, bytesReceived, 0);
            }
        }
    }
}

int main() {
    const char* serverIP = "192.168.1.153";
    const int serverPort = 1111;

    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 2);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error initializing WinSock" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int addrSize = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr(serverIP);
    addr.sin_port = htons(serverPort);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &addrSize);

        if (newConnection == INVALID_SOCKET) {
            std::cout << "Error accepting connection" << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;
        Connections[i] = newConnection;
        Counter++;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), 0, NULL);
    }

    WSACleanup();
    return 0;
}