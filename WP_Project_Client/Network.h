#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 4000
#define UDP_PORT 3500
#define MAX_BUF_SIZE 100000

bool NetworkInit();
void NetworkUpdate();
void NetworkRelease();


static SOCKET clientSocket;
static SOCKET udpSocket;
static SOCKADDR_IN udpServer;

static char buffer[MAX_BUF_SIZE];