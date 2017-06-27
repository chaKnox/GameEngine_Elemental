#include "Client.h"


Client::Client()
{
	strcpy_s(Buffer, " ");
}


Client::~Client()
{
	WSACleanup();
}

void Client::Initialize()
{
	WSAStartup(MAKEWORD(2, 2), &Winsock);//start winsock
	if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)//check version
	{
		WSACleanup();
		return;
	}
	//create the socket we want to use
	Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//determine the ip address and the port of the server application
	ZeroMemory(&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;//set the address family
	//InetPton(AF_INET,SERVER_ADDRESS, &ServerAddress.sin_addr.S_un.S_addr);//set the ip address
	ServerAddress.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);
	ServerAddress.sin_port = SERVER_PORT;//set teh port
	//send knock to server
	char Knock = 1;
	sendto(Socket, &Knock, sizeof(char), 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

void Client::Send()
{
	while (Buffer[0] != ' ')
	{
		//scanf_s("%16s",Buffer);//get text from the std input --- manual input
		sendto(Socket, Buffer, 16, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));//send the data
		strcpy_s(Buffer, " ");
		if (Buffer[0] == ' ')//if the buffer is empty break out of the loop
			break;
	}
}
