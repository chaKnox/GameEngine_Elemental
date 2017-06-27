#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#define WIN32_LEAN_AND_MEAN
//#define _WINSOCKAPI_
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")//windsock library
//#include <Ws2tcpip.h> 
#include <stdio.h>
#define SERVER_ADDRESS "192.168.1.100"
#define SERVER_PORT 17000

class Client
{
private:
	WSADATA Winsock;//stores info about winsock
	SOCKET Socket;
	sockaddr_in ServerAddress;
	char Buffer[16];
public:
	Client();
	~Client();
	void Initialize();
	void Send();
	void BufferIn(char* buff) { strcpy_s(Buffer, buff); }
	void Receive();
};
#endif // !CLIENT_H
